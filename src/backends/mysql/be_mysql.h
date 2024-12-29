#pragma once

#include "../backend.h"

/**
 * Represents a MySQL backed store.
 * Requires an existing MySQL host.
 */
class BE_Mysql: public IBackend
{
public:
    /**
     * Constructs the backend state and connects to the MySQL service given
     * @param options The relevant `mosquitto_opt` from the broker's config file
     */
    BE_Mysql(const std::map<std::string, std::string>& options);

    /**
     * Verifies a client credentials against the MySQL store
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
     * Identifier to use in the broker configuration to connect to a MySQL service
     */
    static constexpr const char* kind = "mysql";
};
