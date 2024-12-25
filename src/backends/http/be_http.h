#pragma once

#include "../backend.h"

/**
 * Represents an HTTP backed store.
 * Requires an existing HTTP server.
 */
class BE_Http: public IBackend
{
public:
    /**
     * Constructs the backend state
     * @param options The relevant `mosquitto_opt` from the broker's config file
     */
    BE_Http(const std::map<std::string, std::string>& options);

    /**
     * Verifies a client credentials against the Http store
     * @param username The username the client passed
     * @param password The password the client passed
     * @param client_id The id associated with the mosquitto client making the connection
     * @return True if the client should be granted access by the broker
     */
    bool authenticate(const std::string& username, const std::string& password, const std::string& client_id);

    /**
     * Identifier to use in the broker configuration to connect to an Http backend
     */
    static constexpr const char* kind = "http";

private:
    void setupBaseUri(const std::map<std::string, std::string>& options) noexcept;
    void setupSubpaths(const std::map<std::string, std::string>& options) noexcept;

    std::string m_base_uri;
    std::string m_auth_path = "/auth";
    std::string m_acl_path = "/acl";
};
