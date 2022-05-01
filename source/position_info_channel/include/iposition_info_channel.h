#pragma once

#include <memory>
#include <string>

enum class ConnectionType : uint8_t{
    Slave = 0,
    Master = 1,
};

class IPositionInfoChannel
{
    public:
        ~IPositionInfoChannel() {};
        virtual void writeJointsQuantity(int jointsQuantity) = 0;
        virtual double read(int jointIndex) = 0;
        virtual void write(int jointIndex, double value) = 0;
};

class PositionInfoChannelFactory
{
    public:
        static std::unique_ptr<IPositionInfoChannel> Create(const std::string& robotId, ConnectionType connectionType);
};
