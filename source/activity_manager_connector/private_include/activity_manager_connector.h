#include "iactivity_manager_connector.h"

class ActivityManagerConnector :public IActivityManagerConnector{
    public:
        // virtual ~IActivityManagerConnector() = 0; //TODO use virtual destructor when implemented
        virtual void Publish(const std::string& content, const std::string& key);
};
