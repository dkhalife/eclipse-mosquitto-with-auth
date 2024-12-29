#pragma once

#include <map>
#include <memory>
#include <mosquitto_plugin.h>
#include <string_view>

/**
 * Abstraction for a backend store
 */
class IBackend
{
public:
    virtual ~IBackend(){}

    /**
     * Verifies a client credentials against its own store
     * @param username The username the client passed
     * @param password The password the client passed
     * @param client_id The id associated with the mosquitto client making the connection
     * @return True if the client should be granted access by the broker
     */
    virtual bool authenticate(const std::string& username, const std::string& password, const std::string& client_id) = 0;

    /**
     * Reloads the backend configuration
     * @param options The new configuration options
     * @return True if the reload was successful
     */
    virtual bool reload(const std::map<std::string, std::string>& options) = 0;

    /**
     * Checks if a client has access to a topic
     * @param client_id The id associated with the mosquitto client making the connection
     * @param topic The topic the client is trying to access
     * @param access The access level requested
     * @return True if the client should be granted access by the broker
     */
    virtual bool checkAcl(const std::string& client_id, const std::string& topic, int access) = 0;
};

/**
 * Factory for the various available backends. Abstracts away the implementation types for each backend
 * @param kind The kind of backend to construct
 * @return A pointer to a backend of the requested `kind`, `nullptr` if the backend kind is not supported
 */
std::unique_ptr<IBackend> BackendFactory(std::string_view kind, const std::map<std::string, std::string>& options);
