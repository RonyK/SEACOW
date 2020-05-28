#include <util/singleton.h>

namespace msdb
{
	template <typename _derived>
	_derived& singleton<_derived>::instance()
	{
		static _derived instance;
		return instance;
	}
}