#include "sim_controller.h"

#include <cstdio>
#include <cstring>
#include <iostream>

#define PI_DIV_2     1.57079632679489661923

SimController::SimController()
{
    this->positionInterface = PositionInfoChannelFactory::Create("simulationPosition", ConnectionType::Slave);
    this->destinationPositions = std::unique_ptr<double[]>(new double[10]);
}

SimController::SimController(const std::string& sharedMemoryId)
{
    this->positionInterface = PositionInfoChannelFactory::Create(sharedMemoryId, ConnectionType::Slave);
    this->destinationPositions = std::unique_ptr<double[]>(new double[10]);
}

void SimController::Init(const mjModel* m, mjData* d) {
    auto jointsQuantity = m->nq;
    this->destinationPositions = std::unique_ptr<double[]>(new double[jointsQuantity]);
    this->positionInterface->writeJointsQuantity(jointsQuantity);
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
    this->destinationPositions[0] = this->positionInterface->read(0);
    std::cout << " Desired position " << this->destinationPositions[0] << "\n";
    std::cout << " Current position " << data->qpos[0] << "\n";
    std::cout << " Delta " << (this->destinationPositions[0] + data->qpos[0]) << "\n";

    data->ctrl[0] = 0.3*(- this->destinationPositions[0] - data->qpos[0]);
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
