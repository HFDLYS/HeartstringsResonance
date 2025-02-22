#include "gemmodelfactory.h"

namespace Graphics {
const int MAX_GEM_STYLE = 2;

const QVector<std::string> STYLE[MAX_GEM_STYLE] =  {
    {   
        "yf.obj",
        "diamond.obj",
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
    int style = GlobalConfig::getInstance().getGemStyle()%MAX_GEM_STYLE;
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_1.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/" + STYLE[style][0]));
    models_.push_back(new Model(func, "./resource/models/gem/" + STYLE[style][1]));
    models_.push_back(new Model(func, "./resource/models/gem/" + STYLE[style][2]));
    models_.push_back(new Model(func, "./resource/models/gem/" + STYLE[style][3]));
    models_.push_back(new Model(func, "./resource/models/gem/blue.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/2.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/8.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/3.obj"));
    
}

Model* GemModelFactory::GetModel(int type) {
    if ((unsigned int)(type) >= models_.size()) {
        return nullptr;
    }
    return models_[type];
}

}  // namespace Hypercube
