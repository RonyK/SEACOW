#pragma once
#ifndef _MSDB_SINGLETON_H_
#define _MSDB_SINGLETON_H_

namespace msdb
{
	template <typename _derived>
	class singleton
	{
	public:
		static _derived* instance()
		{
			static _derived instance;
			return &instance;
		}

		// Delete copy and move constructor
		singleton(const singleton<_derived>&) = delete;
		singleton(singleton&&) = delete;

		singleton<_derived>& operator=(const singleton<_derived>&) = delete;
		singleton<_derived>& operator=(singleton<_derived>&&) = delete;

	protected:
		virtual bool init() = 0;

	protected:
		singleton() : isInit(false) 
		{
			this->init();
			this->isInit = true;
		}
		virtual ~singleton() {}

		bool isInit;
	};
}

#endif