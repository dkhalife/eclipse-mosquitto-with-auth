#include "be_passwd.h"

#include <mosquitto.h>

BE_Passwd::BE_Passwd(const std::vector<mosquitto_opt>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_Passwd::kind);
}
