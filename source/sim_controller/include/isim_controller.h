#pragma once

#include <mujoco/mujoco.h>
#include <memory>

class ISimController {
    public:
        virtual ~ISimController() {};
        virtual void Step(const mjModel* m, mjData* d) = 0;
        virtual void Init(const mjModel* m, mjData* d) = 0;
};

class SimControllerFactory {
    public:
        static std::unique_ptr<ISimController> Create();
};
