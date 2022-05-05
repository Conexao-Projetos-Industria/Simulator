#pragma once

#include "isim_controller.h"
#include "iposition_info_channel.h"
#include <string>
#include <memory>

struct JointControlWeigths {
    uint16_t p;
};
class SimController : public ISimController {
    public:
        SimController(const std::string& sharedMemoryId);
        SimController();
        virtual ~SimController() {};
        virtual void Step(const mjModel* m, mjData* d);
        virtual void Init(const mjModel* m, mjData* d);
    private:
        std::unique_ptr<double[]> mDestinationPositions;
        std::unique_ptr<IPositionInfoChannel> mPositionInterface;
        std::unique_ptr<JointControlWeigths[]> mWeigths;
        void BaseInit();
        void PrintContactPoints(const mjData* d);
};
