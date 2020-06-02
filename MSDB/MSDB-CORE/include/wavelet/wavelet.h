#pragma once
#ifndef _MSDB_WAVELET_H_
#define _MSDB_WAVELET_H_

#include <util/coordinate.h>
#include <util/waveletIterator.h>
#include <iostream>
#include <vector>

namespace msdb
{
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
		size_t c_;					// Wavelet length
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

	template <typename _Dty, typename _Ty>
	class waveletTransform
	{
		using size_type = size_t;

		using dim_type = _Dty;
		using dims_type = std::vector<_Dty>;

		using value_type = _Ty;
		using value_point = _Ty*;
		using value_const_pointer = const _Ty*;

	public:
		waveletTransform(wavelet w);

		void setDims(const dims_type& dims)
		{
			this->_dims = dims;
			this->_bandSize.clear();
		}

		//int encode(const );

		int decode();

	protected:
		wavelet _w;
		dims_type _dims;
		std::vector<dims_type> _bandSize;
	};

	int waveletEncode(const wavelet* w, const double* data, double* output,
						size_t length, std::vector<int>* dims, size_t level = 1);

	template <typename _Dty>
	void getWaveletLevelDims(std::vector<_Dty>& output, const std::vector<_Dty>& dims, const size_t level)
	{
		assert(level + 1 != 0);

		double factor = pow(1 / 2, level);
		for (auto d : dims)
		{
			output.push_back(ceil(d * factor));
		}
	}

	template<class _Dty, class _Ty>
	int waveletDecode(_Ty* output, const _Ty* data, const wavelet* w,
					  size_t length, std::vector<_Dty>& dims, size_t level)
	{
		double* temp = new double[length];
		memcpy(temp, data, sizeof(double) * length);
		// init output stream as a zero.
		memset(output, 0, sizeof(double) * length);

		std::vector<_Dty> levelSize, sP(dims.size(), 0), eP;
		getWaveletLevelDims<_Dty>(levelSize, dims, level);
		eP = levelSize;

		coorRangeIterator<int, double> iit(temp, dims.size(), dims.data(), sP.data(), eP.data());
		coorRangeIterator<int, double> oit(output, dims.size(), dims.data(), sP.data(), eP.data());

		size_t levelDataLength = 1;
		for (int d = 0; d < dims.size(); d++)
		{
			levelDataLength *= (static_cast<size_t>(eP[d]) + 1);
			size_t half = eP[d] >> 1;
			size_t rows = levelDataLength / (static_cast<size_t>(eP[d]) - static_cast<size_t>(sP[d]) + 1);

			iit.setBasisDim(d);
			iit.moveToStart();
			oit.setBasisDim(d);
			oit.moveToStart();

			for (size_t r = 0; r < rows; r++)
			{
				for (int i = sP[d]; i < half; ++i)
				{
					_Ty left = iit[0];
					_Ty right = iit[half];
					for (int j = 0; (j < w->c_) && (i + j < eP[d]); j++)
					{
						
						oit[j] += w->h_1[j] * left + w->g_1[j] * right;
						std::cout << w->h_1[j] << " x " << left << " + " << w->g_1[j] << " x " << right << std::endl;
 					}

					oit += 2;
					++iit;
				}
			}

			memcpy(temp, output, sizeof(double) * length);

			for (int i = 0; i < length; i++)
			{
				std::cout << output[i] << ", ";

				if (length % eP[d] == 0)
				{
					std::cout << "/";
				}
			}

			std::cout << std::endl;
		}

		delete[] temp;

		return 0;
	}
}

#endif