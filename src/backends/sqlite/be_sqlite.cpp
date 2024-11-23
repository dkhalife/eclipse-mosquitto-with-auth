#include "be_sqlite.h"

#include <mosquitto.h>

BE_Sqlite::BE_Sqlite(const std::vector<mosquitto_opt>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_Sqlite::kind);
}

bool BE_Sqlite::Authenticate(const std::string& username, const std::string& password)
{
    return false;
}
