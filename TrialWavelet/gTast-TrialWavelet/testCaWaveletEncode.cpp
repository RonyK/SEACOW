#include "pch.h"
#include "../TrialWavelet/wavelet.h"
#include <cmath>
#include <vector>

#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )
TEST(caWaveletEncode, caWaveletHaarInit)
{
	msdb::wavelet w("Haar");

	EXPECT_EQ(w.t_, msdb::waveletType::Haar);

	EXPECT_EQ(w.c_, 2);
	EXPECT_EQ(w.offset_, 1);

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

TEST(caWaveletEncode, caWaveletHaar_1D_Encode)
{
	msdb::wavelet w("Haar");
	double data[] = {1,2,3,4,5,6,7,8};
	
	double* output = (double*)malloc(sizeof(data));
	std::vector<int> dims = { sizeof(data) / sizeof(double) };
	
	msdb::caWaveletEncode(&w, data, output, sizeof(data) / sizeof(double), &dims);

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

TEST(caWaveletEncode, caWaveletHaar_2D_Encode)
{
	EXPECT_TRUE(true);
	msdb::wavelet w("Haar");
	double data[][4] = {{9,7,6,2}, {5,3,4,4}, {8,2,4,0}, {6,0,2,2} };

	double* output = (double*)malloc(sizeof(data));
	std::vector<int> dims = { sizeof(data) / sizeof(double) / 4, 4 };

	msdb::caWaveletEncode(&w, (double*)data, output, sizeof(data) / sizeof(double), &dims);

	double expectedOutput[] = {
		12, 8, 2, 2,  8, 4, 6, 2,  4, 0, 0, 2,  2, 0, 0, 2
	};

	for (int i = 0; i < sizeof(data) / sizeof(double); i++)
	{
		EXPECT_EQ(ROUNDING(output[i], 6), ROUNDING(expectedOutput[i], 6));
	}

	free(output);
}

TEST(caWaveletEncode, caWaveletHaarSimple_2D_Encode)
{
	EXPECT_TRUE(true);
	msdb::wavelet w("HaarSimple");
	double data[][4] = { {9,7,6,2}, {5,3,4,4}, {8,2,4,0}, {6,0,2,2} };

	double* output = (double*)malloc(sizeof(data));
	std::vector<int> dims = { sizeof(data) / sizeof(double) / 4, 4 };

	msdb::caWaveletEncode(&w, (double*)data, output, sizeof(data) / sizeof(double), &dims);

	double expectedOutput[] = {
		6, 4, 1, 1,  4, 2, 3, 1,  2, 0, 0, 1,  1, 0, 0, 1
	};

	for (int i = 0; i < sizeof(data) / sizeof(double); i++)
	{
		EXPECT_EQ(ROUNDING(output[i], 6), ROUNDING(expectedOutput[i], 6));
	}

	free(output);
}
