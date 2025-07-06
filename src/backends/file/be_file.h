#pragma once

#include "../backend.h"
#include "../../utils/sha256.h"

#include <optional>
#include <vector>

/**
 * Represents a file based store.
 * Stores valid username and encrypted password combinations in a file.
 */
class BE_File: public IBackend
{
public:
    /**
     * Constructs the backend state and reads the backing file into memory.
     * @param options The relevant `mosquitto_opt` from the broker's config file
     */
    BE_File(const std::map<std::string, std::string>& options);

    /**
     * Verifies a client credentials against the list of valid in-memory ones
     * @param username The username the client passed
     * @param password The password the client passed
     * @param client_id The id associated with the mosquitto client making the connection
     * @return True if the client should be granted access by the broker
     */
    bool authenticate(const std::string& username, const std::string& password, const std::string& client_id);

    /**
     * Reinitializes the backend based on the new configuration options
     * @param options The new configuration options
     * @return True if the reload was successful
     */
    bool reload(const std::map<std::string, std::string>& options);

    /**
     * Checks if a client has access to a topic
     * @param client_id The id associated with the mosquitto client making the connection
     * @param topic The topic the client is trying to access
     * @param access The access level requested
     * @return True if the client should be granted access by the broker
     */
    bool checkAcl(const std::string& client_id, const std::string& topic, int access);

    /**
     * Identifier to use in the broker configuration to use a file-backed list
     * of credentials
     */
    static constexpr const char* kind = "file";

private:
    /**
     * Common initialization logic across initial and reload scenarios
     */
    bool initialize(const std::map<std::string, std::string>& options);

    /**
     * Loads the given path and stores valid credentials listed in it, in memory
     * @param filePath The path to the file containing the credentials
     * @return A vector of valid credentials, if the file was loaded successfully
     */
    std::optional<std::vector<std::pair<std::string, std::string>>> loadFile(const std::string& filePath);

    using Credentials = std::pair<std::string, std::string>;
    std::vector<Credentials> m_credentials;

    bool m_debug_auth {false};
};
