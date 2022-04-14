#pragma once

#include <memory>
#include <string>

class IPositionInfoChannel
{
    public:
        // virtual ~IPositionInfoChannel() = 0; //TODO use virtual destructor when implemented
        virtual void writeJointsQuantity(int jointsQuantity) {};
        virtual double read(int jointNumber) {return 1.57079632679489661923;};
        virtual void write(int jointNumber, double value) {};
};

class PositionInfoChannelFactory
{
    public:
        static std::unique_ptr<IPositionInfoChannel> Create(const std::string& robotId) {return std::make_unique<IPositionInfoChannel>();};
};
