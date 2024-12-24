#include "rendermanager.h"

#include <iostream>

#include "DEBUG.h"

namespace Graphics {

RenderManager::RenderManager(QWidget *parent) : QOpenGLWidget(parent) {
    // thread
    hypercube_thread_ = new GraphicsThread(this);
    connect(hypercube_thread_, &GraphicsThread::timeout, this, &RenderManager::HypercubeThreadSlot);
    hypercube_thread_->start();
    // camera
    camera_.Position = kCameraPosition;
    center_x = 400;
    center_y = 400;
}

RenderManager::~RenderManager() {
    // thread (no need)
    // stone manager
    delete gem_manager_;
}

GraphicGemManager *RenderManager::GetGemManager() {
    if (gem_manager_ == nullptr) {
        std::cerr << "WARNING!!! StoneManager is nullptr" << std::endl;
    }
    return gem_manager_;
}

void RenderManager::SetXYBoard(int x, int y) {
    int center_x = 100.0 * x / 2;
    int center_y = 100.0 * y / 2;
    this->center_x = center_x;
    this->center_y = center_y;
    int center_z = 125.0 * std::max(x, y);
    camera_.Position = QVector3D(center_x, center_y, center_z);
}

void RenderManager::ShowGem(int type) {
    int nx = 1;
    int ny = 1;
    SetXYBoard(nx, ny);
    GetGemManager()->Init(nx, ny);

    GetGemManager()->Generate(1, 0, 0, type, 0);
    GetGemManager()->setGem(1, type);
    update();
}

void RenderManager::Demo() {
    std::cerr << "Demo imported." << std::endl;
    // 0. some vars
    int nx = 8;
    int ny = 8;

    auto Checker = [&](int va) {
        if (va != 0) std::cerr << "Error" << std::endl;
    };

    // 1. initialize
    Checker(GetGemManager()->Init(nx, ny));

    // 2. generate gemstone
    int tot = 0;
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            Checker(GetGemManager()->Generate(++tot, i, j, rand() % 4 + 1, rand() % 500 + 100));
        }
    }

    // 3. swap
    Checker(GetGemManager()->SwapStone(1, 2));
    Checker(GetGemManager()->SwapStone(3, 4));
    Checker(GetGemManager()->SwapStone(5, 6));
    Checker(GetGemManager()->SwapStone(1, 10));
    Checker(GetGemManager()->SwapStone(12, 12 + 8));

    // 4. remove
    Checker(GetGemManager()->Remove(3, true));
    Checker(GetGemManager()->Remove(4, true));
    // 5. fall
    Checker(GetGemManager()->FallTo(1, 50));
    Checker(GetGemManager()->FallTo(2, 50));
    Checker(GetGemManager()->FallTo(13, 50));
    Checker(GetGemManager()->FallTo(14, 50));

    // 6. random remove
    for (int i = 1; i <= 40; i++) {
        int random = 0;
        while (random == 0 || random == 3 || random == 4) random = rand() % (nx + ny) + 1;

        Checker(GetGemManager()->Remove(random, true));
        
    }
}

void RenderManager::SetBlinnPhong(bool enable) { shader_blinn_phong_ = enable; }

void RenderManager::SetRenderMode(int mode) { shader_render_mode_ = mode; }

void RenderManager::SetLightSource(int source) { shader_light_source_ = source; }

void RenderManager::SetHDRExposure(float exposure) { shader_hdr_exposure_ = exposure; }

void RenderManager::initializeGL() {
    // GL Functions
    initializeOpenGLFunctions();
    
    std::cerr << "OpenGL Version: " << glGetString(GL_VERSION);

    // Gem Manager
    gem_manager_ = new GraphicGemManager(QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext()));

    // Shader Program
    shader_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/common.vert");
    shader_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/common.frag");
    bool success = shader_program_.link();
    if (!success) qDebug() << "InitializeGL Error: " << shader_program_.log();

    // Shader Toy Program 
    shader_toy_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shader_toy.vert");
    // shader_toy_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shader_toy_tllfRX.frag");

    shader_toy_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shader_toy_tllfRX.frag");
    success = shader_toy_program_.link();
    if (!success) qDebug() << "InitializeGL Error: " << shader_toy_program_.log();

    // Background Model
    background = new Model(QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext()),
                           "./resource/models/basic/cube.obj");

    // start time
    start_time = QTime::currentTime();
}

void RenderManager::paintGL() {
    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    view = camera_.GetViewMatrix();
    projection.perspective(camera_.Zoom, float(width() / height()), 0.1f, 4000.f);

    glClearColor(kBackgroundColor.x(), kBackgroundColor.y(), kBackgroundColor.z(), kBackgroundColor.w());
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // shader toy program
    shader_toy_program_.bind();
    shader_toy_program_.setUniformValue("iTime", QTime::currentTime().msecsSinceStartOfDay() / 2000.f);
    shader_toy_program_.setUniformValue("iResolution", width(), height());
    model.setToIdentity();
    model.translate(center_x, center_y, -6000.f);
    shader_toy_program_.setUniformValue("model", model);
    shader_toy_program_.setUniformValue("view", view);
    shader_toy_program_.setUniformValue("projection", projection);

    background->Draw(shader_toy_program_);

    // shader program
    shader_program_.bind();
    shader_program_.setUniformValue("viewPos", camera_.GetPosition());

    shader_program_.setUniformValue("numberOfLights", 2);


    shader_program_.setUniformValue("lights[0].type", 1);  // 平行光光源
    shader_program_.setUniformValue("lights[0].ambient", 0.2f, 0.2f, 0.2f);
    shader_program_.setUniformValue("lights[0].diffuse", 0.5f, 0.5f, 0.5f);
    shader_program_.setUniformValue("lights[0].specular", 0.5f, 0.5f, 0.5f);
    shader_program_.setUniformValue("lights[0].direction", 0.0f, 0.5f, -0.3f);

    shader_program_.setUniformValue("lights[1].type", 0);  // 点光源
    shader_program_.setUniformValue("lights[1].ambient", 0.2f, 0.2f, 0.2f);
    shader_program_.setUniformValue("lights[1].diffuse", 0.6f, 0.6f, 0.6f);
    shader_program_.setUniformValue("lights[1].specular", 0.9f, 0.9f, 0.9f);
    shader_program_.setUniformValue("lights[1].position", 400.0f, 900.0f, 300.0f);

    shader_program_.setUniformValue("material.shininess", 32.0f);
    shader_program_.setUniformValue("view", view);
    shader_program_.setUniformValue("projection", projection);

    shader_program_.setUniformValue("onlySpecular", false);

    shader_program_.setUniformValue("enableBlinnPhong", shader_blinn_phong_);
    shader_program_.setUniformValue("renderMode", shader_render_mode_);
    shader_program_.setUniformValue("lightSource", shader_light_source_);
    shader_program_.setUniformValue("hdrExposure", shader_hdr_exposure_);

    if (gem_manager_ != nullptr) gem_manager_->Draw(shader_program_);
}

void RenderManager::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void RenderManager::wheelEvent(QWheelEvent *event) { camera_.ProcessMouseScroll(event->angleDelta().y() / 90); }


void RenderManager::HypercubeThreadSlot() {
    GetGemManager()->Update();
    update();
}

}  // namespace Hypercube
