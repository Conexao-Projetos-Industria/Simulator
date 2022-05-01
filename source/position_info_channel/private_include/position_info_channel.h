#include "iposition_info_channel.h"

#include <boost/interprocess/sync/interprocess_mutex.hpp>

struct ChannelStructure
{
    boost::interprocess::interprocess_mutex mutex;

    int numberOfJoints;
    double jointPosition[50];
};

class PositionInfoChannel : public IPositionInfoChannel
{
    ConnectionType mConnectionType;
    std::string mRobotName;
    ChannelStructure* mData;
    public:
        PositionInfoChannel(std::string const& robotName, ConnectionType connectionType);
        ~PositionInfoChannel(); //TODO use virtual destructor when implemented
        virtual void writeJointsQuantity(int jointsQuantity);
        virtual double read(int jointIndex);
        virtual void write(int jointIndex, double value);
};
