#pragma once

#include "../backend.h"

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
    BE_File(const std::vector<mosquitto_opt>& options);

    /**
     * Verifies a client credentials against the list of valid in-memory ones
     * @return True if the client should be granted access by the broker
     */
    bool authenticate(const std::string& username, const std::string& password);

    /**
     * Identifier to use in the broker configuration to use a file-backed list
     * of credentials
     */
    static constexpr const char* kind = "file";
};
