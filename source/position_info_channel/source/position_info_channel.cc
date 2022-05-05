#include "position_info_channel.h"
#include <boost/interprocess/sync/interprocess_mutex.hpp>

namespace bi = boost::interprocess;

PositionInfoChannel::PositionInfoChannel(std::string const& robotName, ConnectionType connectionType) {
    this->mConnectionType = connectionType;
    this->mRobotName = robotName;

    if(connectionType == ConnectionType::Slave){
        this->mSharedMemoryObject = bi::shared_memory_object(bi::open_or_create
                                                             ,this->mRobotName.c_str()
                                                             ,bi::read_write
                                                            );

        this->mSharedMemoryObject.truncate(sizeof(ChannelHeaderStructure) + 50*sizeof(ChannelBlockStructure));

        this->mMappedRegion = bi::mapped_region(this->mSharedMemoryObject, bi::read_write);

        void* addr = mMappedRegion.get_address();

        this->mHeader = new (addr) ChannelHeaderStructure;

        //Advance addr by size of ChannelStructure
        this->mData = (ChannelBlockStructure*)( ((ChannelHeaderStructure*)this->mHeader) + 1);

        //TODO(giuliano) Should have other logic here. Not just allways write 50.
        //The architectural correct solution would probrably be specialization of
        //the PositionInfoChannel class to be created only after the robot is defined,
        //and it be handle at the factory level. The objetc should be created only
        //when suficient info is available, idealy no incomplete states.
        //Same for truncate.
        this->writeJointsQuantity(50);
    }
    else if(connectionType == ConnectionType::Master){
        this->mSharedMemoryObject = bi::shared_memory_object(bi::open_only
                                                             ,this->mRobotName.c_str()
                                                             ,bi::read_write
                                                            );

        this->mMappedRegion = bi::mapped_region(this->mSharedMemoryObject, bi::read_write);

        //Shared memory is created and initialized by the slave only.
        this->mHeader = static_cast<ChannelHeaderStructure*>(this->mMappedRegion.get_address());

        //Advance addr by size of ChannelStructure
        this->mData = (ChannelBlockStructure*)( ((ChannelHeaderStructure*)this->mHeader) + 1);
    }
}

PositionInfoChannel::~PositionInfoChannel(){
    if(this->mConnectionType == ConnectionType::Slave){
        try{
            bi::shared_memory_object::remove(this->mRobotName.c_str());
        }catch(...){}
    }
}

void PositionInfoChannel::writeJointsQuantity(uint16_t jointsQuantity) {
    bi::scoped_lock<bi::interprocess_mutex> lock(this->mHeader->mutex);

    if(jointsQuantity > 50) {
        throw "Number of joints should be at maximum 50";
    }

    this->mHeader->mNumberOfJoints = jointsQuantity;
}

uint16_t PositionInfoChannel::readJointsQuantity() {
    bi::scoped_lock<bi::interprocess_mutex> lock(this->mHeader->mutex);
    return this->mHeader->mNumberOfJoints;
}

double PositionInfoChannel::read(uint16_t jointIndex) {
    bi::scoped_lock<bi::interprocess_mutex> lock(this->mHeader->mutex);

    if(jointIndex >= this->mHeader->mNumberOfJoints){
        throw "Specified joint index greater than number of joints";
    }

    return this->mData[jointIndex].mPosition;
    return 0;
}

void PositionInfoChannel::write(uint16_t jointIndex, double value) {
    bi::scoped_lock<bi::interprocess_mutex> lock(this->mHeader->mutex);

    if(jointIndex >= this->mHeader->mNumberOfJoints){
        throw "Specified joint index greater than number of joints";
    }

    this->mData[jointIndex].mPosition = value;
}
