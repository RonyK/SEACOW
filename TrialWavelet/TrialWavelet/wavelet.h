#pragma once
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

	template <typename T>
	class caDataIterator
	{
	public:
		typedef caDataIterator self_type;
		typedef T				value_type;
		typedef T&				reference;
		typedef T*				pointer;
		typedef std::ptrdiff_t	difference_type;
		typedef std::random_access_iterator_tag iterator_category;

	public:
		caDataIterator(T* x, const size_t offset = 1) : ptr_(x), offset_(offset) {}
		caDataIterator(const caDataIterator& mit) : ptr_(mit.ptr_), offset_(mit.offset_) {}

		bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_; }
		reference operator*() { return *ptr_; }
		pointer operator->() { return ptr_; }

		// forward
		caDataIterator& operator++() { ptr_ += offset_; return *this; }
		caDataIterator operator++(int) { caDataIterator tmp(*this); operator++(); return tmp; }
		
		// backward
		caDataIterator& operator--() { ptr_ -= offset_; return +this; }
		caDataIterator operator--(int) { caDataIterator tmp(*this); operator--(); return tmp; }
		
		// random access
		caDataIterator& operator[](size_t x)
		{
			if (x < 0)
			{
				throw out_of_range("");
			}

			return ptr_[x * offset_];
		}
		const caDataIterator& operator[](size_t x) const
		{
			if (x < 0)
			{
				throw out_of_range("");
			}

			return ptr_[x * offset_];
		}

	private:
		T* ptr_;
		const size_t offset_;
	};

	//int caWaveletTransform(const caWavelet* w, double* data, double* output, size_t length, std::vector<int>* dims);
	int caWaveletEncode(const caWavelet* w, double* data, double* output,
		size_t length, std::vector<int>* dims, size_t level = 1);
	int caWaveletDecode(const caWavelet* w, double* data, double* output,
		size_t length, std::vector<int>* dims);

}

