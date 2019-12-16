#pragma once

namespace caWavelet
{
	void caGetHaarWavelet(const double** h_0, const double** g_0,
						  const double** h_1, const double** g_1,
						  size_t* c, size_t* offset);
}