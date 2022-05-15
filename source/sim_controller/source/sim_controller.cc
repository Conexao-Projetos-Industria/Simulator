#include "sim_controller.h"

#include <cstdio>
#include <cstring>
#include <iostream>

#define PI_DIV_2     1.57079632679489661923

SimController::SimController()
{
    this->mPositionInterface = PositionInfoChannelFactory::Create("simulationPosition", ConnectionType::Slave);
    this->BaseInit();
}

SimController::SimController(const std::string& sharedMemoryId)
{
    this->mPositionInterface = PositionInfoChannelFactory::Create(sharedMemoryId, ConnectionType::Slave);
    this->BaseInit();
}

void SimController::BaseInit() {
    this->mDestinationPositions = std::unique_ptr<double[]>(new double[10]);
    this->mJointParameters = std::unique_ptr<JointControlParameters[]>(new JointControlParameters[10]);
}

void SimController::Init(const mjModel* m, mjData* d) {
    this->mPositionInterface->writeJointsQuantity(m->nq);
    std::cout << "Number of joints " << this->mPositionInterface->readJointsQuantity()<< std::endl;
    // for(uint16_t i = 0; i < this->positionInterface->readJointsQuantity(); ++i) {
    //     this->positionInterface->write(i, PI_DIV_2);
    // }
    this->mPositionInterface->write(0, 10);
    this->mJointParameters[0].p = 0.4;

    this->mPositionInterface->write(1, 0);
    this->mJointParameters[1].p = 2.5;

    this->mPositionInterface->write(2, PI_DIV_2);
    this->mJointParameters[2].p = 1;

    this->mPositionInterface->write(3, 0);
    this->mJointParameters[3].p = 1.5;

    // this->mPositionInterface->write(4, 0);
    // this->mWeigths[4].p = 0.6;

    // auto jointsQuantity = m->nq;
    // this->destinationPositions = std::unique_ptr<double[]>(new double[jointsQuantity]);
    // this->positionInterface->writeJointsQuantity(jointsQuantity);
    //   std::cout << "Joints : " << std::endl;
    //   for (int i = 0; i < m->nq ; ++i)
    //   {
    //     std::cout << "\t" << i << " ";

    //     int j =0;
    //     char c;
    //     do
    //     {
    //       c = m->names[m->name_jntadr[0]+(j++)];
    //       std::cout << c;
    //     }while(c);
    //     // for(char c = m->names[m->name_jntadr[0]+(j)]; c; ++j)
    //     // while(m->names[m->name_jntadr[0]+(i)])
    //     // {
    //       // std::cout << c;
    //     // }
    //     std::cout << std::endl;
    //   }

    //   std::cout << " nq number of joints " <<  m->nq << "\n";
    //   std::cout << " nu number of actuators " << m->nu << std::endl;
    //   std::cout << " qpos0 inicial position joint 0 " << m->qpos0[0] << std::endl;
}

void SimController::Step(const mjModel* m, mjData* data) {
    for(uint16_t i = 0; i < this->mPositionInterface->readJointsQuantity(); ++i) {
        // this->destinationPositions[i] = this->positionInterface->read(i);
        // std::cout << "Joint " << i << "\n";
        // std::cout << "\tDesired position " << this->destinationPositions[i] << "\n";
        // std::cout << "\tCurrent position " << data->qpos[i] << "\n";
        // std::cout << "\tDelta " << (this->destinationPositions[i] + data->qpos[i]) << "\n";

        //Actual joint control.
        data->ctrl[i] = this->mJointParameters[i].p * (- this->mPositionInterface->read(i) - data->qpos[i]);
    }
}

void SimController::PrintContactPoints(const mjData* d) {
    //   // std::cout << d->ncon << std::endl;
    //   for(int i = 0; i < d->ncon; ++i)
    //   {
    //     std::cout << "intencity " << d->contact->dist << std::endl;
    //     std::cout << "pos x:" << d->contact->pos[0] << " y:" << d->contact->pos[1] << " z:" << d->contact->pos[2] << std::endl;
    //     std::cout << "frame  x:" << d->contact->frame[0] << " y:" << d->contact->frame[1] << " z:" << d->contact->frame[2] << std::endl;
    //   }
}
