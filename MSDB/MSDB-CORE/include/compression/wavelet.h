#pragma once
#ifndef _MSDB_WAVELET_H_
#define _MSDB_WAVELET_H_

#include <util/coordinate.h>
#include <iostream>
#include <vector>
#include <memory>

namespace msdb
{
class wavelet;
using pWavelet = std::shared_ptr<wavelet>;

	// Wavelet Family
	enum class waveletType
	{
		None,
		Haar,
		HaarSimple
	};

	class wavelet
	{
	public:
		const double* h_0;
		const double* g_0;
		const double* g_1;
		const double* h_1;

		waveletType t_;			// Wavelet Family
		size_t c_;				// Wavelet length
		size_t offset_;

	public:
		wavelet();
		wavelet(const char* name, size_t k = 0);
		wavelet(waveletType t, size_t k = 0);
		wavelet(const double* h_0, const double* g_0, const double* h_1, const double* g_1,
				size_t c, size_t offset, waveletType t = waveletType::None);
		~wavelet();

	private:
		static waveletType getWaveletType(const char* name);
		void init(const double* h_0, const double* g_0, const double* h_1, const double* g_1,
				  size_t c, size_t offset, waveletType t);
	};

	//int waveletEncode(const wavelet* w, const double* data, double* output,
	//					size_t length, std::vector<int>* dims, size_t level = 1);

	template <typename _Dty>
	void getWaveletLevelDims(std::vector<_Dty>& output, const std::vector<_Dty>& dims, 
							 const size_t level)
	{
		assert(level + 1 != 0);

		double factor = pow(1 / 2, level);
		for (auto d : dims)
		{
			output.push_back(ceil(d * factor));
		}
	}

	template <typename _Dty>
	size_t getDataLength(const size_t dSize, const _Dty* sP, const _Dty*eP)
	{
		size_t length = 1;
		for (size_t d = 0; d < dSize; d++)
		{
			length *= (eP[d] - sP[d]);
		}
		return length;
	}

	//template<class _Dty, class _Ty>
	//int waveletDecode(_Ty* output, const _Ty* data, const wavelet* w,
	//				  size_t length, std::vector<_Dty>& dims, size_t level)
	//{
	//	double* temp = new double[length];
	//	memcpy(temp, data, sizeof(double) * length);
	//	// init output stream as a zero.
	//	memset(output, 0, sizeof(double) * length);

	//	std::vector<_Dty> levelSize, sP(dims.size(), 0), eP;
	//	getWaveletLevelDims<_Dty>(levelSize, dims, level);
	//	eP = levelSize;

	//	coorRangeIterator<int, double> iit(temp, dims.size(), dims.data(), sP.data(), eP.data());
	//	coorRangeIterator<int, double> oit(output, dims.size(), dims.data(), sP.data(), eP.data());

	//	size_t levelDataLength = getDataLength(dims.size(), sP.data(), eP.data());
	//	std::cout << "dataLength: " << levelDataLength << std::endl;

	//	for (int d = 0; d < dims.size(); d++)
	//	{
	//		size_t half = eP[d] >> 1;
	//		size_t rows = levelDataLength / (static_cast<size_t>(eP[d]) - static_cast<size_t>(sP[d]));
	//		std::cout << sP[d] << ", " << eP[d] << ", half: " << half << ", rows: " << rows << std::endl;

	//		iit.setBasisDim(d);
	//		iit.moveToStart();
	//		oit.setBasisDim(d);
	//		oit.moveToStart();

	//		for (size_t r = 0; r < rows; r++)
	//		{
	//			for (int i = sP[d]; i < half; ++i)
	//			{
	//				_Ty left = iit[0];
	//				_Ty right = iit[half];
	//				for (int j = 0; (j < w->c_) && (i + j < eP[d]); j++)
	//				{
	//					oit[j] += w->h_1[j] * left + w->g_1[j] * right;
 //					}

	//				oit += 2;
	//				++iit;
	//			}
	//			std::cout << "-----" << std::endl;
	//			iit += half;	// go to next row
	//		}

	//		for (int i = 0; i < length; i++)
	//		{
	//			std::cout << output[i] << ", ";

	//			if (length % eP[d] == 0)
	//			{
	//				std::cout << "/";
	//			}
	//		}
	//		std::cout << std::endl;

	//		memcpy(temp, output, sizeof(double) * length);
	//		memset(output, 0, sizeof(double) * length);
	//	}

	//	memcpy(output, temp, sizeof(double) * length);
	//	delete[] temp;

	//	return 0;
	//}
}

#endif