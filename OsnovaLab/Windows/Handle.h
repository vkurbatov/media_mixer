#ifdef WIN32

#ifndef _OL_WINDOWS_HANDLE_H
#define _OL_WINDOWS_HANDLE_H

//stl
#include <memory>

//windows
#include <windows.h>
namespace OsnovaLab
{
	namespace Windows
	{
		class Handle
		{
			HANDLE native_handle_;
		public:

			using HandleShared = std::shared_ptr<Handle>;
			using HandleWeak = std::weak_ptr<Handle>;
			using HandleUnique = std::unique_ptr<Handle>;

			Handle(const HANDLE& native_handle = INVALID_HANDLE_VALUE);
			Handle(Handle&& winHandle);
			HANDLE& operator=(const HANDLE& native_handle);
			Handle& operator=(Handle&& winHandle);
			const bool IsValid() const;
			operator const HANDLE() const;
			~Handle();

			static HandleShared MakeShared(const HANDLE& native_handle);
			static HandleUnique MakeUnique(const HANDLE& native_handle);
		private:
			void close();
		};
	}
}

#endif

#endif