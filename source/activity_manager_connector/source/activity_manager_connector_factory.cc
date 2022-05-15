#include "iactivity_manager_connector.h"
#include "activity_manager_connector.h"

#include <iostream>

std::unique_ptr<IActivityManagerConnector> ActivityManagerConnectorFactory::Create(const std::string& activityManagerURL,
                                                                                   const std::string& manifestPath,
                                                                                   std::function<void(const std::string&, const std::string&)> parserCallback,
                                                                                   std::string const& messageOnConnect)
{
    std::unique_ptr<IActivityManagerConnector> connector;
    try
    {
        connector = std::make_unique<ActivityManagerConnector>(activityManagerURL, manifestPath, parserCallback, messageOnConnect);
    }
    catch(const std::exception& e)
    {
        std::cerr << "[SimControllerFactory]" << e.what() << '\n';
    }
    return connector;
}
