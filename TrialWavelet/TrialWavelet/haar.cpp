#include <cmath>

namespace caWavelet
{
	namespace haar
	{
		static const double h2[2] =
		{
			1/sqrt(2), 1/sqrt(2)
		};
		static const double g2[2] =
		{
			1/sqrt(2), -1/sqrt(2)
		};
	}
	
	void caGetHaarWavelet(const double **h_0, const double **g_0, 
						  const double **h_1, const double **g_1,
						  size_t* c, size_t* offset)
	{
		*h_0 = haar::h2;
		*h_1 = haar::h2;
		*g_0 = haar::g2;
		*g_1 = haar::g2;

		*c = 2;
		*offset = 1;
	}
}