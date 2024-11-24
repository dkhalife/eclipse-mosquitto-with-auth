#include "plugin.h"

#include <map>
#include <mosquitto.h>
#include <mosquitto_broker.h>
#include <mosquitto_plugin.h>

/**
 * The broker will attempt to call this function immediately after loading the plugin to check it is a supported plugin
 * version.
 * @param supported_version_count The number of items in @param supported_versions
 * @param supported_versions An array of the supported versions by the host
 * @return the supported interface version.
 */
int mosquitto_plugin_version(int supported_version_count, const int* supported_versions)
{
    mosquitto_log_printf(MOSQ_LOG_INFO, "*** auth-plugin: startup");

    for (int i = 0; i < supported_version_count; ++i)
    {
        if (supported_versions[i] == MOSQ_PLUGIN_VERSION)
        {
            mosquitto_log_printf(MOSQ_LOG_INFO, "*** auth-plugin: required broker API version is supported");
            return MOSQ_PLUGIN_VERSION;
        }
    }

    mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: required broker API version is not supported");
    return -1;
}

/**
 * Called after the plugin has been loaded and `mosquitto_plugin_version` has been called. Used to initialize plugin
 * state
 * @param identifier This is a pointer to an opaque structure which you must save and use when registering/unregistering
 * callbacks.
 * @param userdata The pointer set here will be passed to the other plugin functions.
 * @param options Pointer to an array of struct mosquitto_opt, which provides the plugin options defined in the
 * configuration file.
 * @param option_count The number of elements in the opts array.
 * @return This function returns 0 to indicate success
 */
int mosquitto_plugin_init(mosquitto_plugin_id_t* identifier, void** userdata, struct mosquitto_opt* options,
                          int option_count)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: init");

    std::map<const char*, const char*, KeysEqual> opts;
    for (int i = 0; i < option_count; ++i)
    {
        const char* key = options[i].key;
        const char* value = options[i].value;
        opts[key] = value;
    }

    *userdata = new Plugin(identifier, std::move(opts));

    return 0;
}

/**
 * Called when the broker is shutting down.  This will only ever be called once per plugin.
 * @param userdata The pointer provided in `mosquitto_plugin_init`.
 * @param options Pointer to an array of struct mosquitto_opt, which provides the plugin options defined in the
 * configuration file.
 * @param option_count The number of elements in the opts array.
 * @return This function returns 0 to indicate success
 */
int mosquitto_plugin_cleanup(void* userdata, struct mosquitto_opt* options, int option_count)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: cleanup");

    Plugin* self = reinterpret_cast<Plugin*>(userdata);
    delete self;

    return 0;
}
