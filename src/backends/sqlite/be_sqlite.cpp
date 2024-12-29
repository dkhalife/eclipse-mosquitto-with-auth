#include "be_sqlite.h"

#include <mosquitto.h>

BE_Sqlite::BE_Sqlite(const std::map<std::string, std::string>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_Sqlite::kind);
}

bool BE_Sqlite::authenticate(const std::string& username, const std::string& password, const std::string& /*client_id*/)
{
    return false;
}

bool BE_Sqlite::reload(const std::map<std::string, std::string>& options)
{
    return true;
}

bool BE_Sqlite::checkAcl(const std::string& /*client_id*/, const std::string& /*topic*/, int /*access*/)
{
    return false;
}
