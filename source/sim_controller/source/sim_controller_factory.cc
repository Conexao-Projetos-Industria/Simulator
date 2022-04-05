#include "isim_controller.h"
#include "sim_controller.h"

std::unique_ptr<ISimController> SimControllerFactory::Create() {
    return std::make_unique<SimController>();
}