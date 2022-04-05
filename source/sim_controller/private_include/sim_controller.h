#pragma once

#include "isim_controller.h"

class SimController : public ISimController {
    public:
        virtual void Step(const mjModel* m, mjData* d);
        virtual void Init(const mjModel* m, mjData* d);
    private:
        void PrintContactPoints(const mjData* d);
};