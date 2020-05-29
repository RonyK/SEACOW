#pragma once
#ifndef _MSDB_WAVELET_H_
#define _MSDB_WAVELET_H_

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

	int waveletEncode(const wavelet* w, double* data, double* output,
						size_t length, std::vector<int>* dims, size_t level = 1);
	int waveletDecode(const wavelet* w, double* data, double* output,
						size_t length, std::vector<int>* dims);

	void getWaveletBandSize(const std::vector<int>& dims, std::vector<int>& output, const size_t level);
}

#endif