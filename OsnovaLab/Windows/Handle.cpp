#ifdef WIN32

#include "OsnovaLab\Windows\Handle.h"

namespace OsnovaLab
{
	namespace Windows
	{
		Handle::Handle(const HANDLE& native_handle) : native_handle_(native_handle)
		{

		}

		Handle::Handle(Handle && winHandle) : native_handle_(std::move(winHandle.native_handle_))
		{
			winHandle.native_handle_ = INVALID_HANDLE_VALUE;
		}

		Handle::~Handle()
		{
			close();
		}

		HANDLE& Handle::operator=(const HANDLE & native_handle)
		{
			close();
			return native_handle_ = native_handle;
		}

		Handle& Handle::operator=(Handle && winHandle)
		{
			close();

			native_handle_ = winHandle.native_handle_;
			winHandle.native_handle_ = INVALID_HANDLE_VALUE;
			return *this;
		}

		const bool Handle::IsValid() const
		{
			return (size_t)native_handle_ >= 0;
		}

		Handle::operator const HANDLE() const
		{
			return native_handle_;
		}

		void Handle::close()
		{
			if (IsValid())
			{
				CloseHandle(native_handle_);
				native_handle_ = INVALID_HANDLE_VALUE;
			}
		}

		Handle::HandleShared Handle::MakeShared(const HANDLE& native_handle)
		{
			return std::make_shared<Handle>(native_handle);
		}

		Handle::HandleUnique Handle::MakeUnique(const HANDLE& native_handle)
		{
			return std::make_unique<Handle>(native_handle);
		}
	}
}

#endif