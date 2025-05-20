#include <esp_log.h>
#include <Core/Logger/Logger.h>

void registerEspLogger()
{
    esp_log_set_vprintf(Logger::customLogWrite);
}