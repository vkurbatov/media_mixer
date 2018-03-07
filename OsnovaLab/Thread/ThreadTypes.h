#ifndef _OL_THREAD_TYPES_H
#define _OL_THREAD_TYPES_H

namespace OsnovaLab
{
	namespace Thread
	{
		using timeout_type = long;
		using signal_type = int;
		using thread_id_type = int;
        using thread_result_t = int;
        using thread_size_t = long;

		const static timeout_type TIMEOUT_INFINITE = -1;
		const static timeout_type TIMEOUT_NULL = 0;

		const static signal_type SIGNAL_ANY = 0;
		const static signal_type SIGNAL_E_INVALID = -1;
		const static signal_type SIGNAL_E_NO_SLOTS = -2;
		const static signal_type SIGNAL_E_TIMEOUT = -100;

	}
}

#endif
