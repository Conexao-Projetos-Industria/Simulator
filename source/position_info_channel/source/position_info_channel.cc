#include "position_info_channel.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

PositionInfoChannel::PositionInfoChannel(std::string const& robotName, ConnectionType connectionType) {
    this->mConnectionType = connectionType;
    this->mRobotName = robotName;

    if(connectionType == ConnectionType::Slave){
        shared_memory_object shm_obj (open_or_create               //open or create
                                      ,this->mRobotName.c_str()    //name
                                      ,read_only                    //read-only mode
                                      );

        shm_obj.truncate(10000);

        mapped_region region(shm_obj, read_write);

        void* addr = region.get_address();

        this->mData = new (addr) ChannelStructure;

        //TODO(giuliano) Should have other logic here. Not just allways write 50.
        this->writeJointsQuantity(50);
    }
    else if(connectionType == ConnectionType::Master){
        shared_memory_object shm_obj (open_only                    //only open
                                      ,this->mRobotName.c_str()    //name
                                      ,read_write                   //read-write mode
                                      );

        mapped_region region(shm_obj, read_write);

        this->mData = static_cast<ChannelStructure*>(region.get_address());
    }
}

PositionInfoChannel::~PositionInfoChannel(){
    if(this->mConnectionType == ConnectionType::Slave){
        try{
            shared_memory_object::remove(this->mRobotName.c_str());
        }catch(...){}
    }
}

void PositionInfoChannel::writeJointsQuantity(int jointsQuantity) {
    scoped_lock<interprocess_mutex> lock(this->mData->mutex);

    if(jointsQuantity > 50) {
        throw "Number of joints should be at maximum 50";
    }

    this->mData->numberOfJoints = jointsQuantity;
}

double PositionInfoChannel::read(int jointIndex) {
    scoped_lock<interprocess_mutex> lock(this->mData->mutex);

    if(jointIndex >= this->mData->numberOfJoints){
        throw "Specified joint index greater than number of joints";
    }

    return this->mData->jointPosition[jointIndex];
}

void PositionInfoChannel::write(int jointIndex, double value) {
    scoped_lock<interprocess_mutex> lock(this->mData->mutex);

    if(jointIndex >= this->mData->numberOfJoints){
        throw "Specified joint index greater than number of joints";
    }

    this->mData->jointPosition[jointIndex] = value;
}
