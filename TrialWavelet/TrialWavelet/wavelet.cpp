#include <cstring>
#include <cassert>
#include <cmath>
#include "wavelet.h"
#include "haar.h"

namespace caWavelet
{
	caWavelet::caWavelet()
		: h_0(NULL), g_0(NULL), h_1(NULL), g_1(NULL), c_(0), offset_(0), t_(caWaveletType::None)
	{

	}

	caWavelet::caWavelet(const char* name, size_t k)
		: caWavelet(this->getWaveletType(name), k)
	{

	}

	caWavelet::caWavelet(caWaveletType t, size_t k)
	{
		switch (t)
		{
		case caWaveletType::Haar:
		default:
			const double* h_0, *g_0, *h_1, *g_1;
			size_t c, offset;

			caGetHaarWavelet(&h_0, &g_0, &h_1, &g_1, &c, &offset);
			this->init(h_0, g_0, h_1, g_1, c, offset, t);
			break;

		}
	}

	//:h_0(h_0), g_0(g_0), h_1(h_1), g_1(g_1), c(c), offset(offset), t(t)
	caWavelet::caWavelet(const double* h_0, const double* g_0, const double* h_1, const double* g_1,
		size_t c, size_t offset, caWaveletType t)
	{
		this->init(h_0, g_0, h_1, g_1, c, offset, t);
	}

	caWavelet::~caWavelet()
	{

	}

	caWaveletType caWavelet::getWaveletType(const char* name)
	{
		caWaveletType t = caWaveletType::None;

		if (strcmp(name, "Haar") == 0)
		{
			t = caWaveletType::Haar;
		}
		else if (strcmp(name, "Daubechies") == 0)
		{
			t = caWaveletType::Daubechies;
		}

		return t;
	}

	void caWavelet::init(const double* h_0, const double* g_0, 
		const double* h_1, const double* g_1, 
		size_t c, size_t offset, caWaveletType t)
	{
		this->h_0 = h_0;
		this->g_0 = g_0;
		this->h_1 = h_1;
		this->g_1 = g_1;
		
		this->c_ = c;
		this->offset_ = offset;
		this->t_ = t;
	}

	int caWaveletEncode(const caWavelet* w, double* data, double* output, 
		size_t length, std::vector<int>* dims, size_t level)
	{
		// init output stream as a zero.
		memset(output, 0, sizeof(double) * length);

		size_t offset = 1;
		for (auto d : *dims)
		{
			size_t half = d >> 1;
			size_t end = d - 1;

			caDataIterator<double> it(data, offset);

			for (int i = 0, ii = 0; ii < length; i++, ii += 2)
			{
				double h = 0, g = 0;

				for (int j = 0; j < w->c_; j++)
				{
					h += w->h_0[j] * data[end & (ii + j)];
					g += w->g_0[j] * data[end & (ii + j)];
				}

				output[i] = h;
				output[i + half] = g;
			}

			offset *= d;
		}

		return 0;
	}

	int caWaveletDecode(const caWavelet* w, double* data, double* output,
		size_t length, std::vector<int>* dims)
	{
		return 0;
	}

	template <typename Dty_>
	void getBandSize(const std::vector<Dty_>& dims, std::vector<Dty_>& output, const size_t level)
	{
		double factor = pow(1 / 2, level);
		for (auto d : dims)
		{
			output.push_back(ceil(d * factor));
		}
	}
}
