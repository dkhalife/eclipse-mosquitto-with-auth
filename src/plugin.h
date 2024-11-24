#pragma once

#include "backends/backend.h"

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
    Plugin(mosquitto_plugin_id_t* identifier, std::vector<mosquitto_opt> options);
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

    static int onEvent(int event_id, void* event_data, void* user_data) noexcept;

private:
    /**
     * Initializes the backends in the order they were listed in the configuration file. The first one
     * listed is the first responder, and so on.
     */
    void initializeBackends() noexcept;
    void registerEvents() noexcept;
    void unregisterEvents() noexcept;

    std::vector<mosquitto_opt> m_options;
    mosquitto_plugin_id_t* m_identifier;

    std::vector<std::unique_ptr<IBackend>> m_backends;
};
