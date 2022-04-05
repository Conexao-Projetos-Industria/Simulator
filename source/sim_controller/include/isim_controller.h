#pragma once

#include "mujoco.h"
#include <memory>

class ISimController {
    public:
        virtual void Step(const mjModel* m, mjData* d) = 0;
        virtual void Init(const mjModel* m, mjData* d) = 0;
};

class SimControllerFactory {
    public:
        static std::unique_ptr<ISimController> Create();
};