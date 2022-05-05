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
        virtual void writeJointsQuantity(uint16_t jointsQuantity) = 0;
        virtual uint16_t readJointsQuantity() = 0;
        virtual double read(uint16_t jointIndex) = 0;
        virtual void write(uint16_t jointIndex, double value) = 0;
};

class PositionInfoChannelFactory
{
    public:
        static std::unique_ptr<IPositionInfoChannel> Create(const std::string& robotId, ConnectionType connectionType);
};
