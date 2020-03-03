#pragma once
#ifndef _caWavelet_
#define _caWavelet_

#include <yvals_core.h>
#include <iostream>
#include <vector>
#include <array>
#include <iterator>

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
		
		caWaveletType t_;			// Wavelet Family
		size_t c_;					// Wavelet length
		size_t offset_;

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


	// TODO :: Replace caDataIterator to caCoorSeq
	template <typename Ty_>
	class caDataIterator
	{
	public:
		using selft_type = caDataIterator;

		using value_type = Ty_;
		using value_reference = Ty_&;
		using value_pointer = Ty_*;
		using value_const_pointer = const Ty_*;
		
		typedef std::ptrdiff_t	difference_type;
		typedef std::random_access_iterator_tag iterator_category;

	public:
		caDataIterator(Ty_* x, const size_t offset = 1) : ptr_(x), offset_(offset) {}
		caDataIterator(const caDataIterator& mit) : ptr_(mit.ptr_), offset_(mit.offset_) {}

		bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_; }
		value_reference operator*() { return *ptr_; }
		pointer operator->() { return ptr_; }

		// forward
		caDataIterator& operator++() { ptr_ += offset_; return *this; }
		caDataIterator operator++(int) { caDataIterator tmp(*this); operator++(); return tmp; }
		
		// backward
		caDataIterator& operator--() { ptr_ -= offset_; return +this; }
		caDataIterator operator--(int) { caDataIterator tmp(*this); operator--(); return tmp; }
		
		// random access
		value_reference operator[](size_t x)
		{
			if (x < 0)
			{
				throw std::out_of_range("");
			}

			return ptr_[x * offset_];
		}
		const value_reference operator[](size_t x) const
		{
			if (x < 0)
			{
				throw std::out_of_range("");
			}

			return ptr_[x * offset_];
		}

	private:
		Ty_* ptr_;
		const size_t offset_;
	};

	//int caWaveletTransform(const caWavelet* w, double* data, double* output, size_t length, std::vector<int>* dims);
	int caWaveletEncode(const caWavelet* w, double* data, double* output,
		size_t length, std::vector<int>* dims, size_t level = 1);
	int caWaveletDecode(const caWavelet* w, double* data, double* output,
		size_t length, std::vector<int>* dims);

	void getBandSize(const std::vector<int>& dims, std::vector<int>&output, const size_t level);
}

#endif // _caWavelet_