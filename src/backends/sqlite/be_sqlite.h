#pragma once

#include "../backend.h"

/**
 * Represents a SQLite backed store.
 */
class BE_Sqlite: public IBackend
{
public:
    /**
     * Constructs the backend state and connects to the SQLite database. Creates
     * one if necessary.
     * @param options The relevant `mosquitto_opt` from the broker's config file
     */
    BE_Sqlite(const std::map<std::string, std::string>& options);

    /**
     * Verifies a client credentials against the SQLite store
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
     * Identifier to use in the broker configuration to connect to a SQLite database
     */
    static constexpr const char* kind = "sqlite";
};
