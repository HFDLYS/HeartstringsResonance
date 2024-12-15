#include "gemmodelfactory.h"

namespace Graphics {

GemModelFactory::GemModelFactory(QOpenGLFunctions_3_3_Core* func) {
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_1.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/love.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/yf.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/flower.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/diamond.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/blue.obj"));
    models_.push_back(new Model(func, "./resource/models/gem/gemstone_6.obj"));
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
