#include "iposition_info_channel.h"
#include "position_info_channel.h"

std::unique_ptr<IPositionInfoChannel>
PositionInfoChannelFactory::Create(const std::string& robotId, ConnectionType connectionType) {
    return std::make_unique<PositionInfoChannel>(robotId, connectionType);
}
