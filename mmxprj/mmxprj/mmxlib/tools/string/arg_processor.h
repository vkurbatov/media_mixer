#ifndef ARG_PROCESSOR_H
#define ARG_PROCESSOR_H

#include <vector>
#include <map>

namespace mmx
{
	namespace tools
	{
		namespace string
		{
			enum
			{
				ARG_TYPE_NUMERIC,
				ARG_TYPE_REAL,
				ARG_TYPE_STRING,
				ARG_TYPE_BOOL,
				ARG_TYPE_FLAG,
				ARG_TYPE_CUSTOM
			}key_types_t;

			class ArgProcessor
			{

				int		argc_;
				char**	argv_;

			public:
				ArgProcessor(int argc, char* argv[]);
			};
		}
	}
}


#endif // ARG_PROCESSOR_H
