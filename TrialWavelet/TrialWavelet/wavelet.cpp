#include <cstring>
#include <cassert>
#include <cmath>
#include "iterators.h"
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
		// copy input data
		double* temp = new double[length];
		memcpy(temp, data, sizeof(double) * length);
		// init output stream as a zero.
		memset(output, 0, sizeof(double) * length);
		std::vector<int> boundary(*dims), sP(dims->size(), 0), eP(*dims);

		caCoorRangeIterator<int, double> iit(temp, eP.size(), dims->data(), sP.data(), eP.data());
		caCoorRangeIterator<int, double> oit(output, eP.size(), dims->data(), sP.data(), eP.data());

		size_t seq = 0;
		size_t numbers = 1;
		for (int i = 0; i < dims->size(); i++)
		{
			numbers *= (eP[i] - sP[i] + 1);
		}

		for (int d = eP.size() - 1; d >= 0; d--)
		{
			std::cout << "D: " << d << std::endl;
			size_t half = eP[d] >> 1;
			size_t rows = numbers / (eP[d] - sP[d] + 1);

			iit.setBasisDim(d);
			iit.moveToStart();
			oit.setBasisDim(d);
			oit.moveToStart();

			for (int r = 0; r < rows; r++)
			{
				for (int i = sP[d]; i < eP[d]; i += 2)
				{
					double h = 0, g = 0;

					for (int j = 0; (j < w->c_) && (i + j < eP[d]); j++)
					{
						h += w->h_0[j] * iit[j];
						g += w->g_0[j] * iit[j];
					}

					oit[0] = h;
					oit[half] = g;
					
					iit += 2;
					oit++;
				}
				oit += half;
			}

			memcpy(temp, output, sizeof(double) * length);

			for (int i = 0; i < length; i++)
			{
				std::cout << output[i] << ", ";

				if (length % eP[d] == 0)
				{
					std::cout << "/" << std::endl;
				}
			}
		}

		return 0;
	}

	int caWaveletDecode(const caWavelet* w, double* data, double* output,
		size_t length, std::vector<int>* dims)
	{
		return 0;
	}

	template <typename Dty_>
	void getBandSize(std::vector<Dty_>& output, const std::vector<Dty_>& dims, const size_t level)
	{
		double factor = pow(1 / 2, level);
		for (auto d : dims)
		{
			output.push_back(ceil(d * factor));
		}
	}
}
