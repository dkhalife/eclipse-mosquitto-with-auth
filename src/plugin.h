#pragma once

#include "backends/backend.h"

#include <map>
#include <mosquitto_plugin.h>
#include <vector>

/**
 * Represents the global state of the plugin.
 * Enforced as a singleton due to the initialization and lifecycle of
 * mosquitto plugin and its integration in the init/uninit functions.
 */
class Plugin
{
public:
    /**
     * Constructs the global state
     * @param identifier Serves as a unique id for this plugin when interacting with the broker
     * @param options Holds the list of directives of type `mosquitto_opt` from the broker's configuration file
     */
    Plugin(mosquitto_plugin_id_t* identifier, const mosquitto_opt* options, int option_count);
    /**
     * Cleans up any global state
     */
    ~Plugin();

    /**
     * Handles the basic authentication flow by allowing each registered backend to authenticate a client
     * given a username and password combination. As long as one backend accepts the credentials, the
     * authentication is considered successful. The backends are assigned priority based on the order they
     * are listed in the configuration file. The first one listed is the first responder, and so on.
     * @param event_data The raw data packet sent from the broker
     * @return MOSQ_ERR_SUCCESS for successful combination, MOSQ_ERR_AUTH otherwise
     */
    int onBasicAuth(const mosquitto_evt_basic_auth& event_data) noexcept;

    /**
     * Handles the reload event by reloading the configuration of each backend
     * @param event_data The raw data packet sent from the broker
     * @return MOSQ_ERR_SUCCESS for successful reload, MOSQ_ERR_UNKNOWN otherwise
     */
    int onReload(const mosquitto_evt_reload& event_data) noexcept;

    /**
     * Handles the ACL check event by allowing each registered backend to check if a client has access to a
     * @param event_data The raw data packet sent from the broker
     * @return MOSQ_ERR_SUCCESS for successful access, MOSQ_ERR_ACL_DENIED otherwise
     */
    int onAclCheck(const mosquitto_evt_acl_check& event_data) noexcept;

    /**
     * Handles the broker events by dispatching them to the appropriate event handler
     * @param event_id The event id
     * @param event_data The raw data packet sent from the broker
     * @param user_data The current Plugin instance
     * @return The response from the underlying event handler
     */
    static int onEvent(int event_id, void* event_data, void* user_data) noexcept;

private:
    /**
     * Initializes the backends in the order they were listed in the configuration file. The first one
     * listed is the first responder, and so on.
     */
    void initializeBackends() noexcept;

    /**
     * Registers the plugin's event handlers with the broker
     */
    void registerEvents() noexcept;

    /**
     * Unregisters the plugin's event handlers with the broker
     */
    void unregisterEvents() noexcept;

    /**
     * Loads the options from the broker's configuration file
     * @param options The list of directives from the broker's configuration file
     * @param option_count The number of directives in the list
     * @return A map of the loaded options
     */
    std::map<std::string, std::string> loadOptions(const mosquitto_opt* options, int option_count) const noexcept;

    std::map<std::string, std::string> m_options;
    mosquitto_plugin_id_t* m_identifier;

    std::vector<std::unique_ptr<IBackend>> m_backends;
};
