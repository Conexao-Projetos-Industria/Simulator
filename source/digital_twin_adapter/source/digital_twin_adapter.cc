
#include "iposition_info_channel.h"
#include "iactivity_manager_connector.h"
#include <string>
#include <sstream>
#include <memory>
#include <functional>

#include <iostream>

class DigitalTwinParser
{
    std::unique_ptr<IActivityManagerConnector> mActivityManagerConnector;
    std::unique_ptr<IPositionInfoChannel> mPositionInfoChannel;
    public:
        DigitalTwinParser(const std::string& activityManagerURL, const std::string& robotId){
            this->mPositionInfoChannel = PositionInfoChannelFactory::Create("simulationPosition", ConnectionType::Master);

            auto parserCallback = std::bind(&DigitalTwinParser::Parse, this, std::placeholders::_1, std::placeholders::_2);
            this->mActivityManagerConnector = ActivityManagerConnectorFactory::Create(activityManagerURL, "resources/manifest.json", parserCallback, "MensagemAoConnectar");
        };

    private:
        void Parse(const std::string& message, const std::string& key){\
            std::cout << "[Digital Twin] message:" << message << "  key: " << key <<std::endl;
            if(key == "positionUpdate"){
                uint16_t joint;
                double position;

                std::stringstream ss(message);
                ss >> joint >> position;
                this->PublishPosition(joint, position);
            }
        };

        void PublishPosition(uint16_t joint, double position){
            // int jointNumber = 0;
            // double position = 0.0;

            // std::stringstream (received) >> jointNumber >> position;
            this->mPositionInfoChannel->write(joint, position);
        };
};

int main()
{
    auto digitalTwinAdapter = DigitalTwinParser("192.168.0.8:9091", "RobotId");

    while(true){;}
    // auto positionInterface = PositionInfoChannelFactory::Create("simulationPosition", ConnectionType::Master);

    // std::cout << "Number of joints " << positionInterface->readJointsQuantity()<< std::endl;

    // std::cout << "l - list all joint desired positions" << std::endl;
    // std::cout << "w - write a position" << std::endl;
    // std::cout << "help - show help" << std::endl;
    // std::cout << "e - exit" << std::endl;

    // std::string userCommand = "";
    // while(userCommand != "e") {
    //     std::cin >> userCommand;
    //     if(userCommand == "l") {
    //         for(uint16_t i = 0; i < positionInterface->readJointsQuantity(); ++i) {
    //             std::cout << "At " << i << ": " << positionInterface->read(i) << std::endl;
    //         }
    //     }
    //     else if(userCommand == "help") {
    //         std::cout << "l - list all joint desired positions" << std::endl;
    //         std::cout << "w - write a position" << std::endl;
    //         std::cout << "help - show help" << std::endl;
    //         std::cout << "e - exit" << std::endl;
    //     }
    //     else if(userCommand == "w") {
    //         int16_t joint;
    //         double position;
    //         std::cout << "What joint?" << std::endl;
    //         std::cin >> joint;
    //         std::cout << "What position?" << std::endl;
    //         std::cin >> position;

    //         positionInterface->write(joint, position);
    //     }
    //     else {
    //         std::cout << "Command not recognized" << std::endl;
    //     }
    // }

}
