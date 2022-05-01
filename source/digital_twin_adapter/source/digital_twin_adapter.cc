
#include "iposition_info_channel.h"
#include "iactivity_manager_connector.h"
#include <string>
#include <sstream>
#include <memory>
#include <functional>

class DigitalTwinParser
{
    std::unique_ptr<IActivityManagerConnector> activityManagerConnector;
    std::unique_ptr<IPositionInfoChannel> positionInfoChannel;
    public:
        DigitalTwinParser(const std::string& activityManagerURL, const std::string& robotId){
            this->positionInfoChannel = PositionInfoChannelFactory::Create(robotId, ConnectionType::Master);

            auto parserCallback = std::bind(&DigitalTwinParser::Parse, this, std::placeholders::_1, std::placeholders::_2);
            this->activityManagerConnector = ActivityManagerConnectorFactory::Create(activityManagerURL, "resources/manifest.json", parserCallback);
        };

    private:
        void Parse(const std::string& received, const std::string& key){
            if(key == "0"){
                this->PublishPosition(received);
            }
        };

        void PublishPosition(const std::string& received){
            int jointNumber = 0;
            double position = 0.0;

            std::stringstream (received) >> jointNumber >> position;
            positionInfoChannel->write(jointNumber, position);
        };
};

int main()
{
    auto digitalTwinAdapter = DigitalTwinParser("192.168.0.1:9091", "RobotId");
}
