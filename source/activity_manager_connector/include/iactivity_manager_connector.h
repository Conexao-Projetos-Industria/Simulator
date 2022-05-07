#pragma once

#include <functional>
#include <memory>
#include <string>

class IActivityManagerConnector{
    public:
        virtual void Publish(const std::string& content, const std::string& key) = 0;
};

class ActivityManagerConnectorFactory
{
    public:
        static std::unique_ptr<IActivityManagerConnector> Create(const std::string& activityManagerURL,
                                                                 const std::string& manifestPath,
                                                                 std::function<void(const std::string&, const std::string&)> parserCallback,
                                                                 std::string const& messageOnConnect = "");
};
