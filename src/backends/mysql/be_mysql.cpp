#include "be_mysql.h"

#include <mosquitto.h>

BE_Mysql::BE_Mysql(const std::map<const char*, const char*, KeysEqual>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_Mysql::kind);
}

bool BE_Mysql::authenticate(const std::string& username, const std::string& password)
{
    return false;
}
