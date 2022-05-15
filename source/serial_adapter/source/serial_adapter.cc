
#include "iposition_info_channel.h"
#include "iactivity_manager_connector.h"
#include <string>
#include <sstream>
#include <memory>
#include <functional>

#include <iostream>

class SerialAdapter
{
    std::unique_ptr<IActivityManagerConnector> mActivityManagerConnector;
    public:
        SerialAdapter(const std::string& activityManagerURL, const std::string& robotId){
            // this->positionInfoChannel = PositionInfoChannelFactory::Create(robotId, ConnectionType::Master);

            auto parserCallback = std::bind(&SerialAdapter::Parse, this, std::placeholders::_1, std::placeholders::_2);
            this->mActivityManagerConnector = ActivityManagerConnectorFactory::Create(activityManagerURL, "resources/manifest.json", parserCallback, "MensagemAoConnectar");
        };

        void PublishPosition(uint16_t joint, double position){
            std::cout << "[SerialAdapter]PublishPosition" << std::endl;
            std::stringstream ss;
            ss << joint << " "  << position;
            this->mActivityManagerConnector->Publish(ss.str(), "positionUpdate");
        };
    private:
        void Parse(const std::string& received, const std::string& key){\
            std::cout << "[Serial Adapter] :" << received << std::endl;
            // if(key == "0"){
            //     this->PublishPosition(received);
            // }
        };
};

int main(int argc, char* argv[])
{
    std::string address;
    if (argc != 2) {
        std::cout << "You can pass the Activity Manager URL as argument.\n Example:\n .\\digital_twin_adapter.exe \"192.168.0.17:9091\"";
        std::cout << "\nWhat is the URL the Activity Manager is hosted on? (ex.: 192.168.0.17:9091)\n";
        std::cin >> address;
    } else {
        address = std::string(argv[1]);
    }

    auto serialAdapter = SerialAdapter(address, "RobotId");

    // std::cout << "l - list all joint desired positions" << std::endl;
    std::cout << "w - write a position" << std::endl;
    std::cout << "help - show help" << std::endl;
    std::cout << "e - exit" << std::endl;

    std::string userCommand = "";
    while(userCommand != "e") {
        std::cin >> userCommand;
        // if(userCommand == "l") {
        //     for(uint16_t i = 0; i < positionInterface->readJointsQuantity(); ++i) {
        //         std::cout << "At " << i << ": " << positionInterface->read(i) << std::endl;
        //     }
        // }
        if(userCommand == "help") {
            std::cout << "l - list all joint desired positions" << std::endl;
            std::cout << "w - write a position" << std::endl;
            std::cout << "help - show help" << std::endl;
            std::cout << "e - exit" << std::endl;
        }
        else if(userCommand == "w") {
            int16_t joint;
            double position;
            std::cout << "What joint?" << std::endl;
            std::cin >> joint;
            std::cout << "What position?" << std::endl;
            std::cin >> position;

            serialAdapter.PublishPosition(joint, position);
        }
        else {
            std::cout << "Command not recognized" << std::endl;
        }
    }

}
