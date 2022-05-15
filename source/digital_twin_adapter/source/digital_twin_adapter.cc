
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

    auto digitalTwinAdapter = DigitalTwinParser(address, "RobotId");

    while(true){;}
}
