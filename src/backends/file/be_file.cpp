#include "be_file.h"

#include <mosquitto.h>

BE_File::BE_File(const std::vector<mosquitto_opt>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_File::kind);
}

bool BE_File::authenticate(const std::string& username, const std::string& password)
{
    mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: username %s with password %s", username.c_str(), password.c_str());
    return false;
}
