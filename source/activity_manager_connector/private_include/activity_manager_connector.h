#include "iactivity_manager_connector.h"

class ActivityManagerConnector :public IActivityManagerConnector{
        std::string mTopic;
        std::string mToken;

        void AquireToken(const std::string& activityManagerURL);
    public:
        ActivityManagerConnector(const std::string& activityManagerURL,
                                 const std::string& manifestPath,
                                 std::function<void(const std::string&, const std::string&)> parserCallback);
        // virtual ~IActivityManagerConnector() = 0; //TODO use virtual destructor when implemented
        virtual void Publish(const std::string& content, const std::string& key);
};
