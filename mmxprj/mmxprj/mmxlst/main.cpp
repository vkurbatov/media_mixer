
/* Сервис-слушатель UDP портов. Читает
 *
 *
 *
 *
 */

#include "mmxlib/logs/log.h"
#include "listener.h"

#define DEFAULT_LEVEL_LOG mmx::logs::L_DEBUG

//echo “hello” > /dev/udp/127.0.0.1/5000

int main(int argc, char* argv[])
{
    mmx::logs::log_init("/tmp/mmx_lst", DEFAULT_LEVEL_LOG, false);

    mmx::net::PortSet ports;

    mmxlst::Listener listener(0, 1, ports);

    ports.Set(5000);
    ports.Set(5002);
    ports.Set(5004);

    int rc = listener.Execute();

    return rc;
}

