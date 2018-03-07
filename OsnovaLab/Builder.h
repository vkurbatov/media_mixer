#ifndef _OL_BUILDER_H
#define _OL_BUILDER_H


#include "OsnovaLab/Logger/AbstractLoggerFactory.h"

namespace OsnovaLab
{
	class Builder
	{
	public:
		static Logger::ILogger* CreateScreenLogger(const std::string& title);
		static Logger::ILogger* CreateFileLogger(const std::string& title, const std::string& fileName);
	protected:
        static Logger::ILogger* BuildLogger(const Logger::AbstractLoggerFactory& loggerFactory, const std::string& title, const std::string& deviceName = "");
	};
}

#endif
