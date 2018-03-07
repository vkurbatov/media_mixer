//stl
#include <algorithm>

//osnovalab
#include "OsnovaLab/Sockets/AbstractChannel.h"

namespace OsnovaLab
{
	namespace Sockets
	{
		AbstractChannel::AbstractChannel()
		{

		}

		AbstractChannel::~AbstractChannel()
		{
			for (auto& c : channels_)
			{
				if (c != nullptr)
				{
					delete c;
				}
			}
			channels_.clear();
		}

		IConnection* AbstractChannel::operator[] (socket_size_t index)
		{
			return getChannel(index);
		}

		const socket_size_t AbstractChannel::Count() const
		{
			return channels_.size();
		}

		socket_result_t AbstractChannel::addChannel(IConnection* cinnection)
		{
			socket_result_t rc = E_INVARG;
			
			if (cinnection != nullptr && !containChannel(cinnection))
			{
				channels_.push_back(cinnection);
				rc = channels_.size();
			}

			return rc;
		}

		socket_result_t AbstractChannel::removeChannel(IConnection* cinnection)
		{
			socket_result_t rc = E_INVARG;

			if (cinnection != nullptr && containChannel(cinnection))
			{
				channels_.remove(cinnection);
				delete cinnection;
				rc = channels_.size();
			}

			return rc;
		}

		IConnection* AbstractChannel::getChannel(socket_size_t index)
		{

			if (index >= 0 && index < channels_.size())
			{
				auto it = channels_.begin();
				std::advance(it, index);
				return *it;
			}
			return nullptr;
		}

		bool AbstractChannel::containChannel(IConnection* connection)
		{
			return connection != nullptr && std::find(channels_.begin(), channels_.end(), connection) != channels_.end();
		}
	}
}
