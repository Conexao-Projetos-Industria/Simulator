#include <functional>
#include <memory>
#include <string>

class IActivityManagerConnector{
    public:
        // virtual ~IActivityManagerConnector() = 0; //TODO use virtual destructor when implemented
        virtual void Publish(const std::string& content, const std::string& key) {};
};

class ActivityManagerConnectorFactory
{
    public:
        static std::unique_ptr<IActivityManagerConnector> Create(const std::string& activityManagerURL,
                                                                 const std::string& manifestPath,
                                                                 std::function<void(const std::string&, const std::string&)> parserCallback)
    {
        return std::make_unique<IActivityManagerConnector>();
    };
};
