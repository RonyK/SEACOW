#include "mmt.h"

namespace caWavelet
{
	unsigned long long calcMaxLimit(bit_cnt_type bits)
	{
		return  ~((unsigned long long)1 & 0) >> (sizeof(unsigned long long) * CHAR_BIT - bits);
	}

	unsigned long long calcMinLimit(bit_cnt_type bits)
	{
		return (unsigned long long)1 << (bits - 1);
	}
}