#include <util/math.h>

namespace msdb
{
int64_t intDivCeil(int64_t li, int64_t ri)
{
	return li / ri + (li % ri != 0);
}

unsigned long long calcMaxLimit(bit_cnt_type bits)
{
	return  ~((unsigned long long)1 & 0) >> (sizeof(unsigned long long) * CHAR_BIT - bits);
}

unsigned long long calcMinLimit(bit_cnt_type bits)
{
	return (unsigned long long)1 << (bits - 1);
}
}
