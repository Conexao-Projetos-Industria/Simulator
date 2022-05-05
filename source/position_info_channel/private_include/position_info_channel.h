#include "iposition_info_channel.h"

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

struct ChannelHeaderStructure
{
    boost::interprocess::interprocess_mutex mutex;

    uint16_t mNumberOfJoints;
};

struct ChannelBlockStructure
{
    double mPosition;
};
class PositionInfoChannel : public IPositionInfoChannel
{
    ConnectionType mConnectionType;
    std::string mRobotName;
    ChannelHeaderStructure* mHeader;
    ChannelBlockStructure* mData;

    boost::interprocess::shared_memory_object mSharedMemoryObject;
    boost::interprocess::mapped_region mMappedRegion;
    public:
        PositionInfoChannel(std::string const& robotName, ConnectionType connectionType);
        ~PositionInfoChannel();
        virtual void writeJointsQuantity(uint16_t jointsQuantity);
        virtual uint16_t readJointsQuantity();
        virtual double read(uint16_t jointIndex);
        virtual void write(uint16_t jointIndex, double value);
};
