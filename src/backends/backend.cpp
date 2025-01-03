#include "backend.h"
#include "file/be_file.h"
#include "http/be_http.h"
#include "mysql/be_mysql.h"
#include "sqlite/be_sqlite.h"

#include <mosquitto.h>

std::unique_ptr<IBackend> BackendFactory(std::string_view kind, const std::map<std::string, std::string>& options)
{
    if (kind == BE_File::kind) {
        return std::make_unique<BE_File>(options);
    }

    if (kind == BE_Http::kind) {
        return std::make_unique<BE_Http>(options);
    }

    if (kind == BE_Mysql::kind) {
        return std::make_unique<BE_Mysql>(options);
    }

    if (kind == BE_Sqlite::kind) {
        return std::make_unique<BE_Sqlite>(options);
    }

    mosquitto_log_printf(MOSQ_LOG_ERR, "*** auth-plugin: backend %s is not supported", kind.data());
    return nullptr;
}
