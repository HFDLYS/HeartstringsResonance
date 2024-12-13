#include <QApplication>
#include <ctime>
#include <iostream>
#include <random>

#include "./windows/mainwindow.h"

int main(int argc, char *argv[]) {
    srand(time(0));
    QSurfaceFormat format;
    format.setVersion(3, 3); // 请求 OpenGL 3.3
    format.setProfile(QSurfaceFormat::CoreProfile); // 核心模式
    format.setDepthBufferSize(24); // 深度缓冲区大小
    format.setAlphaBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication a(argc, argv);
    MainWindow m;
    m.show();
    return a.exec();
}
