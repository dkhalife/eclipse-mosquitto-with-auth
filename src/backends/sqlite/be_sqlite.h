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
    BE_Sqlite(const std::vector<mosquitto_opt>& options);

    /**
     * Verifies a client credentials against the SQLite store
     * @return True if the client should be granted access by the broker
     */
    bool authenticate(const std::string& username, const std::string& password);

    /**
     * Identifier to use in the broker configuration to connect to a SQLite database
     */
    static constexpr const char* kind = "sqlite";
};
