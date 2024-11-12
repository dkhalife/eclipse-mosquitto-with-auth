#include <mosquitto.h>
#include <mosquitto_broker.h>
#include <mosquitto_plugin.h>

int mosquitto_auth_plugin_version(void)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: startup");
    return MOSQ_PLUGIN_VERSION;
}

int mosquitto_auth_plugin_init(void** userdata, struct mosquitto_opt* auth_opts, int auth_opt_count)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: init");
    return 0;
}

int mosquitto_auth_plugin_cleanup(void* userdata, struct mosquitto_opt* auth_opts, int auth_opt_count)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: cleanup");
    return 0;
}

int mosquitto_auth_unpwd_check(void* userdata, struct mosquitto* client, const char* username, const char* password)
{
    if (!username || !*username || !password || !*password)
    {
        return MOSQ_ERR_AUTH;
    }

    mosquitto_log_printf(MOSQ_LOG_DEBUG, "mosquitto_auth_unpwd_check(%s)", (username) ? username : "<nil>");
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "Pmosquitto_auth_unpwd_check(%s)", (password) ? password : "<nil>");

    return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_security_init(void* userdata, struct mosquitto_opt* auth_opts, int auth_opt_count, bool reload)
{
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_security_cleanup(void* userdata, struct mosquitto_opt* auth_opts, int auth_opt_count, bool reload)
{
    return MOSQ_ERR_SUCCESS;
}

int mosquitto_auth_acl_check(void* userdata, int access, struct mosquitto* client, const struct mosquitto_acl_msg* msg)
{
    return MOSQ_ERR_SUCCESS;
}
