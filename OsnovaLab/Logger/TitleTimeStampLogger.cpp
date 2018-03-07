
#include "OsnovaLab/Logger/TitleTimeStampLogger.h"
#include "OsnovaLab/Utils/Strings/StringUtils.h"
#include "OsnovaLab/Builder.h"


namespace OsnovaLab
{
	namespace Logger
	{
		TitleTimeStampLogger::TitleTimeStampLogger(IO::IDeviceIO * deviceIO, Thread::IMutex * mutex, const std::string& title) :
			StandartDeviceLogger(deviceIO, mutex),
			title_(title)
		{

		}

		TitleTimeStampLogger::~TitleTimeStampLogger()
		{

		}

		std::stringstream& TitleTimeStampLogger::Title(std::stringstream& sstream)
		{

			sstream << Utils::Strings::TimeToString() << " " << title_;
			return sstream;
		}


	}
}
