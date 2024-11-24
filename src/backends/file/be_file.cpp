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
    std::string line, username, password;
    int lineNb = 1;
    while (getline(file, line))
    {
        std::stringstream lineStream(line);

        if (getline(lineStream, username, ':') && getline(lineStream, password))
        {
            m_credentials.emplace_back(make_pair(username, password));
        }
        else
        {
            mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: line %i is malformed, skipping it", lineNb);
        }

        ++lineNb;
    }
}

bool BE_File::authenticate(const std::string& username, const std::string& password)
{
    for (const auto& item: m_credentials)
    {
        if (item.first == username && item.second == password)
        {
            return true;
        }
    }

    return false;
}
