#pragma once
#ifndef _caIterators_
#define _caIterators_

#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <list>
#include <iterator>

namespace caWavelet
{
	template <typename Dty_>
	class caCoor
	{
	public:
		//using difference_type = ptrdiff_t;
		using self_type				= caCoor;
		using size_type				= size_t;
		using dim_type				= Dty_;
		using dim_pointer			= Dty_*;
		using dim_const_pointer		= const Dty_*;
		using dim_reference			= Dty_&;
		using dim_const_reference	= const Dty_&;

	public:
		caCoor(const size_type dSize = 0) : dSize_(dSize)
		{
			this->coor_ = new dim_type[this->dSize_]();
		}

		caCoor(const size_type dSize, const dim_pointer coor) : dSize_(dSize)
		{
			this->coor_ = new dim_type[this->dSize_];
			memcpy(this->coor_, coor, this->dSize_ * sizeof(dim_type));
		}

		caCoor(const self_type& mit) : dSize_(mit.dSize_)
		{
			this->coor_ = new dim_type[this->dSize_];
			memcpy(this->coor_, mit.coor_, mit.dSize_ * sizeof(dim_type));
		}

		~caCoor()
		{
			delete[] this->coor_;
		}

	public:
		bool operator==(const self_type& rhs) const 
		{
			return this->dSize_ == rhs.dSize_ && (memcmp(this->coor_, rhs.coor_, this->dSize * sizeof(dim_type)) == 0);
		}
		bool operator!=(const self_type& rhs) const 
		{
			return this->dSize_ != rhs.dSize_ || (memcmp(this->coor_, rhs.coor_, this->dSize * sizeof(dim_type)) != 0);
		}

		_NODISCARD constexpr size_type size() const noexcept
		{
			return this->dSize_;
		}

		// TODO :: unify function of operator at, [] with caCoorIterator
		_NODISCARD _CONSTEXPR17 dim_reference at(_In_range_(0, dSize_ - 1) size_type pos)
		{
			assert(pos < this->dSize_);
			return this->coor_[pos];
		}

		_NODISCARD constexpr dim_const_reference at(_In_range_(0, dSize_ - 1) size_type pos) const
		{
			assert(pos < this->dSize_);
			return this->coor_[pos];
		}

		_NODISCARD _CONSTEXPR17 dim_reference operator[](_In_range_(0, dSize_ - 1) size_type pos)
			noexcept /* strengthened */
		{
			assert(pos < this->dSize_);
			return this->coor_[pos];
		}

		_NODISCARD constexpr dim_const_reference operator[](_In_range_(0, dSize_ - 1) size_type pos) const
			noexcept /* strengthened */
		{
			assert(pos < this->dSize_);
			return this->coor_[pos];
		}

		_NODISCARD _CONSTEXPR17 dim_pointer data() noexcept
		{
			return this->coor_;
		}

		_NODISCARD _CONSTEXPR17 const dim_pointer data() const noexcept
		{
			return this->coor_;
		}

	protected:
		size_type dSize_;
		dim_pointer coor_;
	};

	// TODO:: Do not inherit caCoor
	template <typename Dty_, typename Ty_>
	class caCoorIterator : public caCoor<Dty_>
	{
	public:
		using self_type				= caCoorIterator<Dty_, Ty_>;
		using size_type				= size_t;
		using dim_type				= Dty_;
		using dim_pointer			= Dty_*;
		using dim_const_pointer		= const Dty_*;
		using dim_reference			= Dty_&;
		using dim_const_reference	= const Dty_&;

		using value_type			= Ty_;
		using value_pointer			= Ty_*;
		using value_const_pointer	= const Ty_*;
		using value_reference		= Ty_&;
		using value_const_reference = const Ty_&;

	public:
		caCoorIterator(value_pointer x, const size_type dSize, dim_const_pointer boundary) :
			caCoor<Dty_>(dSize), ptr_(x), offset_(1)
		{
			this->boundary_ = new dim_type[this->dSize_];
			memcpy(this->boundary_, boundary, this->dSize_ * sizeof(dim_type));

			this->basisDim_ = this->dSize_ - 1;
			this->sP_ = new dim_type[this->dSize_]();
			this->eP_ = this->boundary_;

			this->calcVsize();
		}

		caCoorIterator(const self_type& mit) : caCoor<Dty_>(mit), ptr_(mit.ptr_), offset_(mit.offset_), 
			basisDim_(mit.basisDim_), vSize_(mit.vSize_)
		{
			this->boundary_ = new dim_type[mit.dSize_];
			memcpy(this->boundary_, mit.boundary_, mit.dSize_ * sizeof(dim_type));
			
			this->sP_ = new dim_type[mit.dSize_];
			memcpy(this->sP_, mit.sP_, mit.dSize_ * sizeof(dim_type));

			this->eP_ = this->boundary_;
		}

		~caCoorIterator()
		{
			delete[] this->boundary_;
			delete[] this->sP_;
		}

		void setBasisDimension(_In_range_(0, dSize_ - 1) const unsigned int dim)
		{
			if (this->dSize_ <= dim)
			{
				throw std::out_of_range("caCoorIterator::setBasisDimension - too large dim");
			}
			
			this->offset_ = this->getDimOffset(dim);
			this->basisDim_ = dim;
		}
		size_t getDimOffset(_In_range_(0, dSize_ - 1) const unsigned int dim)
		{
			if (dim == this->basisDim_)
			{
				return this->offset_;
			}

			size_t offset = 1;
			for (unsigned int i = this->dSize_ - 1; i > dim; i--)
			{
				offset *= this->boundary_[i];
			}

			//std::cout << "dim : " << dim << ", getOffset: " << offset << std::endl;
			return offset;
		}

		size_t posToSeq(const size_type pos)
		{
			size_type left = pos * this->offset_;
			size_type seq = 0;
			size_type offset = 1;
			for (int d = this->basisDim_; d >= 0; d--)
			{
				seq += (left % this->boundary_[d]) * offset;
				left = left / this->boundary_[d];
				offset *= this->boundary_[d];
			}

			for (int d = this->dSize_ - 1; d > this->basisDim_; d--)
			{
				seq += (left % this->boundary_[d]) * offset;
				left = left / this->boundary_[d];
				offset *= this->boundary_[d];
			}

			return seq;
		}


		// Infinite loop warning
		// if all eP_ is 0
		void next(const unsigned int dim)
		{
			if (this->coor_[dim] + 1 < this->eP_[dim])
			{
				this->coor_[dim]++;
				this->ptr_ += this->getDimOffset(dim);
			} else
			{
				if (dim > 0)
				{
					if (this->basisDim_ == dim - 1)
					{
						//throw std::out_of_range("caCoorIterator next error: out range");
						//this->ptr_++;
						//this->coor_[dim] += 1;
						return;
					}
					this->next(dim - 1);
				} else
				{
					if (this->basisDim_ == this->dSize_ - 1)
					{
						// TODO::Throw exception or forward next?
						//
						//throw std::out_of_range("caCoorIterator next error: out range");
						//
						// or
						//
						//this->ptr_++;
						//this->coor_[dim] += 1;
						return;
					}
					this->next(this->dSize_ - 1);
				}
				this->ptr_ -= this->getDimOffset(dim) * (this->coor_[dim] - this->sP_[dim]);
				this->coor_[dim] = this->sP_[dim];
			}
		}
		void prev(const unsigned int dim)
		{
			const size_t offset = this->getDimOffset(dim);

			if (this->coor_[dim] > this->sP_[dim])
			{
				this->coor_[dim]--;
				this->ptr_ -= offset;
			} else
			{
				if (dim + 1 < this->dSize_)
				{
					if (this->basisDim_ == dim + 1)
					{
						//throw std::out_of_range("caCoorIterator prev error: out range");
						return;
					}
					this->prev(dim + 1);
				} else
				{
					if (this->basisDim_ == 0)
					{
						//throw std::out_of_range("caCoorIterator prev error: out range");
						return;
					}
					this->prev(0);
				}
				this->ptr_ += this->getDimOffset(dim) * (this->eP_[dim] - 1 - this->coor_[dim]);
				this->coor_[dim] = this->eP_[dim] - 1;
			}
		}
		caCoor<Dty_> coor() { return caCoor<Dty_>(*this); }

		bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_; }
		value_reference operator*() { return *ptr_; }
		value_pointer operator->() { return ptr_; }
		
		// forward
		self_type& operator++()
		{
			this->next(this->basisDim_);
			return *this;
		}
		self_type operator++(int)
		{
			caCoorIterator<Dty_, Ty_> tmp(*this);
			operator++();
			return tmp;
		}
		self_type& operator+=(const int& rhs)
		{
			for (int i = 0; i < rhs; i++)
			{
				operator++();
			}
			return *this;
		}

		// backward
		self_type& operator--()
		{
			this->prev(this->basisDim_);
			return +this;
		}
		self_type operator--(int)
		{
			self_type tmp(*this);
			operator--();
			return tmp;
		}
		self_type& operator-=(const int& rhs)
		{
			for (int i = 0; i < rhs; i++)
			{
				operator--();
			}
			return *this;
		}

		// random access
		value_reference operator[](size_type pos)
		{
			if (pos < 0 || pos >= this->vSize_)
			{
				throw std::out_of_range("caCoorIterator [] out of range");
			}

			return ptr_[this->posToSeq(pos)];
		}
		const caCoorIterator& operator[](size_type pos) const
		{
			if (pos < 0 || pos >= this->vSize_)
			{
				throw std::out_of_range("caCoorIterator [] out of range");
			}

			return ptr_[this->posToSeq(pos)];
		}

		void moveToStart()
		{
			this->moveTo(caCoor<Dty_>(this->dSize_, this->sP_));
		}
	protected:
		dim_type moveTo(const caCoor<Dty_>& coor)
		{
			if (this->dSize_ != coor.size())
			{
				throw std::exception("moveTo - different dimension size");
			}

			size_type offset = 1;
			for (int i = this->dSize_ - 1; i >= 0; i--)
			{
				this->ptr_ += (coor[i] - this->coor_[i]) * offset;
				this->coor_[i] = coor[i];
				offset *= this->boundary_[i];
			}
		}
		size_type calcVsize()
		{
			size_type size = this->boundary_[0];
			for (int i = 1; i < this->dSize_; i++)
			{
				size *= this->boundary_[i];
			}

			this->vSize_ = size;
			return size;
		}

	protected:
		Ty_* ptr_;
		size_type offset_;
		size_type vSize_;
		unsigned int basisDim_;
		dim_pointer boundary_;
		dim_pointer sP_;
		dim_pointer eP_;
	};

	// TODO:: Do not inherit caCoorSeq
	template <typename Dty_, typename Ty_>
	class caCoorRangeIterator : public caCoorIterator<Dty_, Ty_>
	{
		using self_type				= caCoorRangeIterator;
		using size_type				= size_t;
		using dim_type				= Dty_;
		using dim_pointer			= Dty_*;
		using dim_const_pointer		= const Dty_*;
		using dim_reference			= Dty_&;
		using dim_const_reference	= const Dty_&;

		using value_type			= Ty_;
		using value_pointer			= Ty_*;
		using value_const_pointer	= const Ty_*;
		using value_reference		= Ty_&;
		using value_const_reference = const Ty_&;

	public:
		caCoorRangeIterator(value_pointer x, const size_type dSize, dim_const_pointer boundary,
			dim_const_pointer sP, dim_const_pointer eP) :
			caCoorIterator<Dty_, Ty_>(x, dSize, boundary)
		{
			this->eP_ = new Dty_[dSize];
			memcpy(this->sP_, sP, dSize * sizeof(dim_type));
			memcpy(this->eP_, eP, dSize * sizeof(dim_type));

			this->moveToStart();
		}

		caCoorRangeIterator(const caCoorRangeIterator& mit) : caCoorIterator<Dty_, Ty_>(mit)
		{
			this->eP_ = new Dty_[mit.dSize_];
			memcpy(this->eP_, mit.eP_, mit.dSize_ * sizeof(dim_type));
		}

		~caCoorRangeIterator()
		{
			delete[] this->eP_;
		}
		
	protected:
		// Check if sP_ and eP_ has proper range.
		bool checkSpEp()
		{
			for (int i = 0; i < this->dSize_; i++)
			{
				if (this->sP_[i] > this->eP_[i])
				{
					return false;
				}
			}

			return true;
		}

		// Check if eP_ and boundary_ has proper range.
		bool checkEpBoundary()
		{
			for (int i = 0; i < this->dSize_; i++)
			{
				if (this->boundary_[i] < this->eP_[i])
				{
					return false;
				}
			}

			return false;
		}

	};
}

#endif // _caIterators_
