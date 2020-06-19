#include <util/math.h>

int64_t msdb::intDivCeil(int64_t li, int64_t ri)
{
	return li / ri + (li % ri != 0);
}
