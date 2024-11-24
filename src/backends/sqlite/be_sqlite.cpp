#include "be_sqlite.h"

#include <mosquitto.h>

BE_Sqlite::BE_Sqlite(const std::map<const char*, const char*, KeysEqual>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_Sqlite::kind);
}

bool BE_Sqlite::authenticate(const std::string& username, const std::string& password)
{
    return false;
}
