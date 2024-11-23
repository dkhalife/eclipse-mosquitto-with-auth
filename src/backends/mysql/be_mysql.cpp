#include "be_mysql.h"

#include <mosquitto.h>

BE_Mysql::BE_Mysql(const std::vector<mosquitto_opt>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_Mysql::kind);
}

bool BE_Mysql::Authenticate(const std::string& username, const std::string& password)
{
    return false;
}
