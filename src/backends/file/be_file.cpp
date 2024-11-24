#include "be_file.h"

#include <fstream>
#include <mosquitto.h>
#include <sstream>
#include <string>

constexpr const char* c_file_opt_key = "creds_file";

BE_File::BE_File(const std::map<const char*, const char*, KeysEqual>& options)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: backend %s initializing", BE_File::kind);

    if (options.count(c_file_opt_key) == 0)
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: required config `%s` is missing", c_file_opt_key);
        return;
    }

    const char* credentialsFilePath = options.at(c_file_opt_key);
    loadFile(credentialsFilePath);
}

void BE_File::loadFile(const char* filePath)
{
    mosquitto_log_printf(MOSQ_LOG_DEBUG, "*** auth-plugin: loading credentials from `%s`", filePath);

    auto file = std::ifstream(filePath);
    if (file.eof())
    {
        mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: file not found: `%s`", filePath);
        return;;
    }

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
