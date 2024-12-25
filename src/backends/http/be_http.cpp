#include "be_http.h"

#include <mosquitto.h>

BE_Http::BE_Http(const std::map<std::string, std::string>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_Http::kind);
}

bool BE_Http::authenticate(const std::string& username, const std::string& password)
{
    return false;
}
