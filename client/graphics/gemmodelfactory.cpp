#include "gemmodelfactory.h"

namespace Graphics {

GemModelFactory::GemModelFactory(QOpenGLFunctions_3_3_Core* func) {
    models_.push_back(new Model(func, "/Users/hfdlys/project/2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_1.obj"));
    models_.push_back(new Model(func, "/Users/hfdlys/project/2022-BJTU-DataStructure-FinalProject/resource/models/gem/hou.obj"));
    models_.push_back(new Model(func, "/Users/hfdlys/project/2022-BJTU-DataStructure-FinalProject/resource/models/gem/zft.obj"));
    models_.push_back(new Model(func, "/Users/hfdlys/project/2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_3.obj"));
    models_.push_back(new Model(func, "/Users/hfdlys/project/2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_4.obj"));
    models_.push_back(new Model(func, "/Users/hfdlys/project/2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_5.obj"));
    models_.push_back(new Model(func, "/Users/hfdlys/project/2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_6.obj"));
    models_.push_back(new Model(func, "/Users/hfdlys/project/2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_7.obj"));
    models_.push_back(new Model(func, "/Users/hfdlys/project/2022-BJTU-DataStructure-FinalProject/resource/models/gem/gemstone_8.obj"));
    
}

Model* GemModelFactory::GetModel(int type) {
    if ((unsigned int)(type) >= models_.size()) {
        return nullptr;
    }
    return models_[type];
}

}  // namespace Hypercube
