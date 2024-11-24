#pragma once

#include "../keysEquals.h"

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
     * @return True if the client should be granted access by the broker
     */
    virtual bool authenticate(const std::string& username, const std::string& password) = 0;
};

/**
 * Factory for the various available backends. Abstracts away the implementation types for each backend
 * @param kind The kind of backend to construct
 * @return A pointer to a backend of the requested `kind`, `nullptr` if the backend kind is not supported
 */
std::unique_ptr<IBackend> BackendFactory(std::string_view kind, const std::map<const char*, const char*, KeysEqual>& options);
