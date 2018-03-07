#include "OsnovaLab/Builder.h"
#include "OsnovaLab/Logger/ScreenLoggerFactory.h"
#include "OsnovaLab/Logger/FileLoggerFactory.h"

namespace OsnovaLab
{
	Logger::ILogger* Builder::CreateScreenLogger(const std::string& title)
	{

		return Builder::BuildLogger(Logger::ScreenLoggerFactory(), title);
	}

	Logger::ILogger* Builder::CreateFileLogger(const std::string& title, const std::string& fileName)
	{
		return Builder::BuildLogger(Logger::FileLoggerFactory(), title, fileName);
	}

    Logger::ILogger* Builder::BuildLogger(const Logger::AbstractLoggerFactory& loggerFactory, const std::string& title, const std::string& deviceName)
	{
		return loggerFactory.CreateLogger(
			loggerFactory.CreateIO(loggerFactory.CreateIOMutex(), deviceName),
			loggerFactory.CreateLoggerMutex(),
			title
			);
	}
}
