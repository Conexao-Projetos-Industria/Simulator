#include "isim_controller.h"
#include "sim_controller.h"

#include <iostream>

std::unique_ptr<ISimController> SimControllerFactory::Create() {
    std::unique_ptr<ISimController> controller;
    try
    {
        controller = std::make_unique<SimController>();
    }
    catch(const std::exception& e)
    {
        std::cerr << "[SimControllerFactory]" << e.what() << '\n';
    }
    return controller;
}
