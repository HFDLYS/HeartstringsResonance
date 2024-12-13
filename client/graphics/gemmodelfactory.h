#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QOpenGLFunctions_3_3_Core>

#include "model.h"

namespace Graphics {

class GemModelFactory {
   public:
    GemModelFactory(QOpenGLFunctions_3_3_Core* func);

    Model* GetModel(int type);

   private:
    std::vector<Model*> models_;
};

}  // namespace Hypercube

#endif  // MODELMANAGER_H
