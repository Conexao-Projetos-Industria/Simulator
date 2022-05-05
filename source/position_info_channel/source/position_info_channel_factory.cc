#include "iposition_info_channel.h"
#include "position_info_channel.h"

#include <iostream>

std::unique_ptr<IPositionInfoChannel>
PositionInfoChannelFactory::Create(const std::string& robotId, ConnectionType connectionType) {
    std::unique_ptr<IPositionInfoChannel> channel;
    try
    {
        channel = std::make_unique<PositionInfoChannel>(robotId, connectionType);
    }
    catch(const std::exception& e)
    {
        std::cerr << "[PositionInfoChannelFactory]" << e.what() << '\n';
    }
    return channel;
}
