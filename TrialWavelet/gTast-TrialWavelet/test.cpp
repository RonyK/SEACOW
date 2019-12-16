#include "pch.h"
#include "../TrialWavelet/wavelet.h"
#include <cmath>
#include <vector>

#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )

TEST(csWaveletEncode, caWaveletHaarInit)
{
	caWavelet::caWavelet w("Haar");

	EXPECT_EQ(w.t, caWavelet::caWaveletType::Haar);

	EXPECT_EQ(w.c, 2);
	EXPECT_EQ(w.offset, 1);

	EXPECT_TRUE(w.h_0 != NULL);
	EXPECT_EQ(w.h_0[0], 1/sqrt(2));
	EXPECT_EQ(w.h_0[1], 1/sqrt(2));
	
	EXPECT_TRUE(w.g_0 != NULL);
	EXPECT_EQ(w.g_0[0], 1/sqrt(2));
	EXPECT_EQ(w.g_0[1], -1/sqrt(2));

	EXPECT_TRUE(w.h_1 != NULL);
	EXPECT_EQ(w.h_1[0], 1/sqrt(2));
	EXPECT_EQ(w.h_1[1], 1/sqrt(2));

	EXPECT_TRUE(w.g_1 != NULL);
	EXPECT_EQ(w.g_1[0], 1/sqrt(2));
	EXPECT_EQ(w.g_1[1], -1/sqrt(2));
}

TEST(csWaveletEncode, caWaveletHaarEncode)
{
	EXPECT_TRUE(true);
	caWavelet::caWavelet w("Haar");
	double data[] = {1,2,3,4,5,6,7,8};
	
	double* output = (double*)malloc(sizeof(data));
	std::vector<int> dims = { sizeof(data) / sizeof(double) };
	
	caWavelet::caWaveletEncode(&w, data, output, sizeof(data) / sizeof(double), &dims);

	double expectedOutput[] = { 
		2.12132034, 4.94974747, 7.77817459, 10.60660172, 
		-0.70710678, -0.70710678, -0.70710678, -0.70710678 
	};

	for (unsigned int i = 0; i < sizeof(data) / sizeof(double); i++)
	{
		EXPECT_EQ(ROUNDING(output[i], 6), ROUNDING(expectedOutput[i], 6));
	}
	
	free(output);
}
