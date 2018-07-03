#include "defines.h"
#include <signal.h>
#include <stdlib.h>
#include "mmxlib/logs/log.h"

void finish(int e_code)
{
	mmx::logs::logI("@Exit with code = %d!\n\n", e_code);
	mmx::logs::log_init();
	exit(e_code);
}

void sig_handler(int sig)
{

	static volatile int e_code = -1;

	if (e_code < 0)
	{

		switch (sig)
		{
			case SIGPIPE:

				mmx::logs::logE("SIGPIPE");

				break;
			case SIGIO:

				mmx::logs::logE("SIGIO");

				break;
			case SIGTERM:
			case SIGQUIT:

				mmx::logs::logC("Close by terminal");

				e_code = 1;

				break;

			case SIGSEGV:

				e_code = 2;

				mmx::logs::logC("@SIGSEGV");

				break;

			case SIGHUP:

				mmx::logs::logE("SIGHUP");

				break;

			default:

				e_code = 3;

				break;
		}

		if (e_code >= 0)
		{
			finish(e_code);
		}
	}
}

void sig_set()
{

	signal(SIGHUP, sig_handler);
	signal(SIGPIPE, sig_handler);
	signal(SIGTERM, sig_handler);
	signal(SIGQUIT, sig_handler);
	//signal(SIGSEGV, sig_handler);
}

