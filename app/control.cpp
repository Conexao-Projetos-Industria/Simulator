#define _USE_MATH_DEFINES
#include "mujoco.h"

#include <cstdio>
#include <cstring>
#include <iostream>

void print_contact(const mjData* d)
{
  // std::cout << d->ncon << std::endl;
  for(int i = 0; i < d->ncon; ++i)
  {
    std::cout << "intencity " << d->contact->dist << std::endl;
    std::cout << "pos x:" << d->contact->pos[0] << " y:" << d->contact->pos[1] << " z:" << d->contact->pos[2] << std::endl;
    std::cout << "frame  x:" << d->contact->frame[0] << " y:" << d->contact->frame[1] << " z:" << d->contact->frame[2] << std::endl;
  }
}

void custom_init(const mjModel* m, mjData* d)
{
  std::cout << "Joints : " << std::endl;
  for (int i = 0; i < m->nq ; ++i)
  {
    std::cout << "\t" << i << " ";

    int j =0;
    char c;
    do
    {
      c = m->names[m->name_jntadr[0]+(j++)];
      std::cout << c;
    }while(c);
    // for(char c = m->names[m->name_jntadr[0]+(j)]; c; ++j)
    // while(m->names[m->name_jntadr[0]+(i)])
    // {
      // std::cout << c;
    // }
    std::cout << std::endl;
  }

  std::cout << " nq number of joints " <<  m->nq << "\n";
  std::cout << " nu number of actuators " << m->nu << std::endl;
  std::cout << " qpos0 inicial position joint 0 " << m->qpos0[0] << std::endl;
}

// extern mjfGeneric mjcb_control;
void mycontroller(const mjModel* model, mjData* data)	
{
  std::cout << " Desired position " << M_PI_2 << "\n";
  std::cout << " Current position " << data->qpos[0] << "\n";
  std::cout << " Delta " << (M_PI_2 + data->qpos[0]) << "\n";

  data->ctrl[0] = 0.3*(- M_PI_2 - data->qpos[0]);
}