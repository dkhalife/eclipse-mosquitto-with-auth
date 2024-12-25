#include "be_file.h"

#include <filesystem>
#include <fstream>
#include <mosquitto.h>
#include <sstream>
#include <string>

constexpr const char* c_file_opt_key = "creds_file";

BE_File::BE_File(const std::map<std::string, std::string>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_File::kind);

    if (options.count(c_file_opt_key) == 0)
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: required config `%s` is missing", c_file_opt_key);
        return;
    }

    const std::string& credentialsFilePath = options.at(c_file_opt_key);
    loadFile(credentialsFilePath);
}

void BE_File::loadFile(const std::string& filePath)
{
    if (!std::filesystem::exists(filePath))
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: file not found: `%s`", filePath.c_str());
        return;
    }

    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: loading credentials from `%s`", filePath.c_str());

    auto file = std::ifstream(filePath);
    std::string line;
    int lineNb = 1;
    while (getline(file, line))
    {
        int iSep = line.find_last_of("::");
        if (iSep == std::string::npos)
        {
            mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: line %i is malformed, skipping it", lineNb);
            continue;
        }

        std::string username = line.substr(0, iSep - 1); // -1 brings us back to the beginning of the separator
        size_t remaining = line.size() - username.size() - 3U; // 2 chars for the separator and 1 for end of line
        std::string password = line.substr(iSep + 1, remaining);
        m_credentials.emplace_back(make_pair(std::move(username), std::move(password)));

        ++lineNb;
    }

    mosquitto_log_printf(MOSQ_LOG_INFO, "*** auth-plugin: loaded %i credentials from `%s`", m_credentials.size(), filePath.c_str());
}

bool BE_File::authenticate(const std::string& username, const std::string& password)
{
    SHA256 hasher;
    std::string input_hash = hasher(password);
    for (const auto& item: m_credentials)
    {
        if (item.first == username && item.second == input_hash)
        {
            return true;
        }
    }

    return false;
}
