#pragma once
#include <vector>
#include <array>

namespace caWavelet
{
	// Wavelet Family
	enum class caWaveletType
	{
		None,
		Haar,
		Daubechies
	};

	class caWavelet
	{
	public:
		const double* h_0;
		const double* g_0;
		const double* g_1;
		const double* h_1;
		
		caWaveletType t;			// Wavelet Family
		size_t c;					// Wavelet length
		size_t offset;

	public:
		caWavelet();
		caWavelet(const char* name, size_t k = 0);
		caWavelet(caWaveletType t, size_t k = 0);
		caWavelet(const double* h_0, const double* g_0, const double* h_1, const double* g_1,
			size_t c, size_t offset, caWaveletType t = caWaveletType::None);
		~caWavelet();

	private:
		static caWaveletType getWaveletType(const char* name);
		void init(const double* h_0, const double* g_0, const double* h_1, const double* g_1,
			size_t c, size_t offset, caWaveletType t);
	};

	//int caWaveletTransform(const caWavelet* w, double* data, double* output, size_t length, std::vector<int>* dims);
	int caWaveletDecode(const caWavelet* w, double* data, double* output, size_t length, std::vector<int>* dims);
	//int caWaveletEncode(const caWavelet* w, double* data, double* output, size_t length, std::vector<int>* dims, size_t level);
	int caWaveletEncode(const caWavelet* w, double* data, double* output, size_t length, std::vector<int>* dims);
}

