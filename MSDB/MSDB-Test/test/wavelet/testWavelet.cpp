#include <pch.h>
#include <compression/wavelet.h>
#include <cmath>
#include <vector>

namespace msdb
{
#define ROUNDING(x, dig)	( floor((x) * pow(double(10), dig) + 0.5f) / pow(double(10), dig) )
	TEST(waveletEncode, waveletHaarInit)
	{
		wavelet w("Haar");

		EXPECT_EQ(w.t_, msdb::waveletType::Haar);

		EXPECT_EQ(w.c_, 2);
		EXPECT_EQ(w.offset_, 1);

		EXPECT_TRUE(w.h_0 != NULL);
		EXPECT_EQ(w.h_0[0], 1 / sqrt(2));
		EXPECT_EQ(w.h_0[1], 1 / sqrt(2));

		EXPECT_TRUE(w.g_0 != NULL);
		EXPECT_EQ(w.g_0[0], 1 / sqrt(2));
		EXPECT_EQ(w.g_0[1], -1 / sqrt(2));

		EXPECT_TRUE(w.h_1 != NULL);
		EXPECT_EQ(w.h_1[0], 1 / sqrt(2));
		EXPECT_EQ(w.h_1[1], 1 / sqrt(2));

		EXPECT_TRUE(w.g_1 != NULL);
		EXPECT_EQ(w.g_1[0], 1 / sqrt(2));
		EXPECT_EQ(w.g_1[1], -1 / sqrt(2));
	}

	TEST(waveletEncode, waveletHaar_1D)
	{
		wavelet w("Haar");
		double data[] = { 1,2,3,4,5,6,7,8 };
		double expectedEncoding[] = {
			2.12132034, 4.94974747, 7.77817459, 10.60660172,
			-0.70710678, -0.70710678, -0.70710678, -0.70710678
		};
		std::vector<int> dims = { sizeof(data) / sizeof(double) };

		//////////////////////////////
		// Encoding
		double* oEncoding = (double*)malloc(sizeof(data));
		waveletEncode(&w, data, oEncoding, sizeof(data) / sizeof(double), &dims);
		for (unsigned int i = 0; i < sizeof(data) / sizeof(double); i++)
		{
			EXPECT_EQ(ROUNDING(oEncoding[i], 6), ROUNDING(expectedEncoding[i], 6));
		}

		//////////////////////////////
		// Decoding
		double* oDecoding = (double*)malloc(sizeof(data));
		waveletDecode(oDecoding, oEncoding, &w, sizeof(data) / sizeof(double), dims, 0);
		for (size_t i = 0; i < sizeof(data) / sizeof(double); i++)
		{
			std::cout << oDecoding[i] << " <-> " << data[i] << std::endl;
			EXPECT_EQ(ROUNDING(oDecoding[i], 6), data[i]);
		}

		free(oEncoding);
		free(oDecoding);
	}

	TEST(waveletEncode, waveletHaar_2D)
	{
		EXPECT_TRUE(true);
		wavelet w("Haar");
		double data[][4] = { {9,7,6,2}, {5,3,4,4}, {8,2,4,0}, {6,0,2,2} };
		double expectedOutput[] = {
					12, 8, 2, 2,  8, 4, 6, 2,  4, 0, 0, 2,  2, 0, 0, 2
		};
		std::vector<int> dims = { sizeof(data) / sizeof(double) / 4, 4 };

		//////////////////////////////
		// Encoding
		double* oEncoding = (double*)malloc(sizeof(data));
		waveletEncode(&w, (double*)data, oEncoding, sizeof(data) / sizeof(double), &dims);
		for (int i = 0; i < sizeof(data) / sizeof(double); i++)
		{
			EXPECT_EQ(ROUNDING(oEncoding[i], 6), ROUNDING(expectedOutput[i], 6));
		}

		std::cout << "####################\nDecoding" << std::endl;
		//////////////////////////////
		// Decoding
		double* oDecoding = (double*)malloc(sizeof(data));
		double* seqData = (double*)(data);
		waveletDecode(oDecoding, oEncoding, &w, sizeof(data) / sizeof(double), dims, 0);
		for (int i = 0; i < sizeof(data) / sizeof(double); i++)
		{
			std::cout << oDecoding[i] << " <-> " << seqData[i] << std::endl;
			EXPECT_EQ(ROUNDING(oDecoding[i], 6), seqData[i]);
		}

		free(oEncoding);
		free(oDecoding);
	}

	TEST(waveletEncode, waveletHaarSimple_2D)
	{
		EXPECT_TRUE(true);
		wavelet w("HaarSimple");
		double data[][4] = { {9,7,6,2}, {5,3,4,4}, {8,2,4,0}, {6,0,2,2} };
		double expectedOutput[] = {
			6, 4, 1, 1,  4, 2, 3, 1,  2, 0, 0, 1,  1, 0, 0, 1
		};
		std::vector<int> dims = { sizeof(data) / sizeof(double) / 4, 4 };

		//////////////////////////////
		// Encoding
		double* oEncoding = (double*)malloc(sizeof(data));
		waveletEncode(&w, (double*)data, oEncoding, sizeof(data) / sizeof(double), &dims);
		for (int i = 0; i < sizeof(data) / sizeof(double); i++)
		{
			EXPECT_EQ(ROUNDING(oEncoding[i], 6), ROUNDING(expectedOutput[i], 6));
		}

		std::cout << "####################\nDecoding" << std::endl;
		//////////////////////////////
		// Decoding
		double* oDecoding = (double*)malloc(sizeof(data));
		double* seqData = (double*)(data);
		waveletDecode(oDecoding, oEncoding, &w, sizeof(data) / sizeof(double), dims, 0);
		for (int i = 0; i < sizeof(data) / sizeof(double); i++)
		{
			std::cout << oDecoding[i] << " <-> " << seqData[i] << std::endl;
			EXPECT_EQ(ROUNDING(oDecoding[i], 6), seqData[i]);
		}

		free(oEncoding);
		free(oDecoding);
	}
}
