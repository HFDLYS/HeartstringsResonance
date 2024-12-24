#include "gemmodelfactory.h"

namespace Graphics {
const int MAX_STYLE = 2;

const QVector<std::string> STYLE[MAX_STYLE] =  {
    {
        "diamond.obj",
        "yf.obj",
        "love.obj",
        "flower.obj",
    },
    {
        "yuyu.obj",
        "doge.obj",
        "hachimi.obj",
        "pig.obj",
    }
};

GemModelFactory::GemModelFactory(QOpenGLFunctions_3_3_Core* func) {
    int style = rand() % MAX_STYLE;
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_1.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/" + STYLE[style][0]));
    models_.push_back(new Model(func, "./resource/models/gem/" + STYLE[style][1]));
    models_.push_back(new Model(func, "./resource/models/gem/" + STYLE[style][2]));
    models_.push_back(new Model(func, "./resource/models/gem/" + STYLE[style][3]));
    models_.push_back(new Model(func, "./resource/models/gem/blue.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_2.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_7.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_8.obj"));
    
}

Model* GemModelFactory::GetModel(int type) {
    if ((unsigned int)(type) >= models_.size()) {
        return nullptr;
    }
    return models_[type];
}

}  // namespace Hypercube
