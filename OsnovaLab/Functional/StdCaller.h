#ifndef _OL_STD_CALLER_H
#define _OL_STD_CALLER_H

#include <functional>
#include "OsnovaLab/Functional/Caller.h"

namespace OsnovaLab
{
	namespace Functional
	{
		template<typename T = void>
		class StdCaller : public ICaller<T>
		{		
			using function_type = std::function<T()>;
			function_type fn_;
		public:
			template<class _Fn, class... _Args>
			StdCaller(_Fn&& fn, _Args&&... args) : fn_(std::bind(fn, args...))
			{

			}

			StdCaller(StdCaller<T>&& stdCaller) : fn_(std::move(stdCaller.fn_))
			{
				stdCaller.fn_ = nullptr;
			}

			StdCaller(const StdCaller<T>& stdCaller) : fn_(stdCaller.fn_)
			{
		
			}
			/*

			StdCaller(function_type& fn) : fn_(fn)
			{

			}

			StdCaller(function_type&& fn) : fn_(std::move(fn))
			{

			}

			StdCaller<T>& operator = (const StdCaller<T>& stdCaller)
			{
				fn_ = stdCaller.fn_;
				return *this;
			}


			StdCaller<T>& operator = (StdCaller<T>&& stdCaller)
			{
				fn_ = std::move(stdCaller.fn_);
				stdCaller.fn_ = nullptr;
				return *this;
			}

			function_type& operator = (function_type& fn)
			{
				return fn_ = fn;
			}


			function_type& operator = (function_type&& fn)
			{
				return fn_ = std::move(fn);
			}
			*/
			
			~StdCaller()
			{

			}

			virtual ICaller<T>* Clone()
			{
				return new StdCaller(*this);
			}

			const function_type GetFunc() const
			{
				return fn_;
			}

			virtual T operator ()()
			{
				if (fn_ != nullptr)
					return fn_();
				else
					return T();
			}
		};
	}
}

#endif
