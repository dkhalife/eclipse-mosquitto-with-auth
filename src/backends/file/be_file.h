#pragma once

#include "../backend.h"

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
    BE_File(const std::map<const char*, const char*, KeysEqual>& options);

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

private:
    /**
     * Loads the given path and stores valid credentials listed in it, in memory
     * @param filePath The path to the file containing the credentials
     */
    void loadFile(const char* filePath);

    using Credentials = std::pair<std::string, std::string>;
    std::vector<Credentials> m_credentials;
};
