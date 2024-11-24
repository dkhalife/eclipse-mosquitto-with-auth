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
    BE_Mysql(const std::map<const char*, const char*, KeysEqual>& options);

    /**
     * Verifies a client credentials against the MySQL store
     * @return True if the client should be granted access by the broker
     */
    bool authenticate(const std::string& username, const std::string& password);

    /**
     * Identifier to use in the broker configuration to connect to a MySQL service
     */
    static constexpr const char* kind = "mysql";
};
