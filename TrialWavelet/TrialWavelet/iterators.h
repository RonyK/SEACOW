#pragma once
#ifndef _caIterators_
#define _caIterators_

#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <list>
#include <iterator>
#include "exceptions.h"
#include "gtest/gtest_prod.h"

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
			this->dims_ = new dim_type[this->dSize_];
			memcpy(this->dims_, boundary, this->dSize_ * sizeof(dim_type));

			this->basisDim_ = this->dSize_ - 1;
			this->sP_ = new dim_type[this->dSize_]();
			this->eP_ = this->dims_;

			this->calcVsize();
		}

		caCoorIterator(const self_type& mit) : caCoor<Dty_>(mit), ptr_(mit.ptr_), offset_(mit.offset_), 
			basisDim_(mit.basisDim_), vSize_(mit.vSize_)
		{
			this->dims_ = new dim_type[mit.dSize_];
			memcpy(this->dims_, mit.dims_, mit.dSize_ * sizeof(dim_type));
			
			this->sP_ = new dim_type[mit.dSize_];
			memcpy(this->sP_, mit.sP_, mit.dSize_ * sizeof(dim_type));

			this->eP_ = this->dims_;
		}

		~caCoorIterator()
		{
			delete[] this->dims_;
			delete[] this->sP_;
		}

		void setBasisDim(_In_range_(0, dSize_ - 1) const unsigned int dim)
		{
			if (this->dSize_ <= dim)
			{
				throw std::out_of_range("caCoorIterator::setBasisDim - too large dim");
			}
			
			this->offset_ = this->getDimOffset(dim);
			this->basisDim_ = dim;
		}

		// Infinite loop warning
		// if all eP_ is 0
		virtual void next(const unsigned int dim)
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

		virtual void prev(const unsigned int dim)
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
		void moveToStart()
		{
			this->moveTo(caCoor<Dty_>(this->dSize_, this->sP_));
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
		_NODISCARD value_reference operator[](size_type pos)
		{
			if (pos < 0 || pos >= this->vSize_)
			{
				throw std::out_of_range("caCoorIterator [] out of range");
			}

			return ptr_[this->posToSeq(pos)];
		}
		_NODISCARD const caCoorIterator& operator[](size_type pos) const
		{
			if (pos < 0 || pos >= this->vSize_)
			{
				throw std::out_of_range("caCoorIterator [] out of range");
			}

			return ptr_[this->posToSeq(pos)];
		}

	protected:
		virtual dim_type moveTo(const caCoor<Dty_>& coor)
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
				offset *= this->dims_[i];
			}
		}

		size_type calcVsize()
		{
			size_type size = this->dims_[0];
			for (int i = 1; i < this->dSize_; i++)
			{
				size *= this->dims_[i];
			}

			this->vSize_ = size;
			return size;
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
				offset *= this->dims_[i];
			}

			//std::cout << "dim : " << dim << ", getOffset: " << offset << std::endl;
			return offset;
		}

		virtual size_t posToSeq(const size_type pos)
		{
			size_type left = pos * this->offset_;
			size_type seq = 0;
			size_type offset = 1;
			for (int d = this->basisDim_; d >= 0; d--)
			{
				seq += (left % this->dims_[d]) * offset;
				left = left / this->dims_[d];
				offset *= this->dims_[d];
			}

			for (int d = this->dSize_ - 1; d > this->basisDim_; d--)
			{
				seq += (left % this->dims_[d]) * offset;
				left = left / this->dims_[d];
				offset *= this->dims_[d];
			}

			return seq;
		}

	protected:
		Ty_* ptr_;				// current pointer
		size_type offset_;		// offset in sequence pointer for current dimension
		size_type vSize_;		// number of elements
		unsigned int basisDim_;	// current dimension
		dim_pointer dims_;		// original array dimension size
		dim_pointer sP_;		// range start point
		dim_pointer eP_;		// range end point
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
				if (this->dims_[i] < this->eP_[i])
				{
					return false;
				}
			}

			return false;
		}

	};

	template <typename Dty_, typename Ty_>
	class caWTIterator : public caCoorIterator<Dty_, Ty_>
	{
	public:
		using self_type = caWTIterator<Dty_, Ty_>;
		using size_type = size_t;
		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_reference = const Dty_&;

		using value_type = Ty_;
		using value_pointer = Ty_*;
		using value_const_pointer = const Ty_*;
		using value_reference = Ty_&;
		using value_const_reference = const Ty_&;

	public: 
		caWTIterator(value_pointer x, const size_type dSize, dim_const_pointer boundary) :
			caCoorIterator<Dty_, Ty_>(x, dSize, boundary)
		{
			this->ptrBegin_ = x;
			this->curLevel_ = 0;
			this->maxLevel_ = 0;
			/* curBandNum_
			 *      0   1
			 *   ¦£¦¡¦¨¦¡¦¤
			 * 0 ¦¢ 0¦¢ 1¦¢
			 *   ¦§¦¡¦«¦¡¦©
			 * 1 ¦¢ 2¦¢ 3¦¢
			 *   ¦¦¦¡¦ª¦¡¦¥
			 */
			this->curBandNum_ = 0;

			this->bandDims_ = new dim_type[this->dSize_];
			memcpy(this->bandDims_, this->dims_, sizeof(dim_type) * this->dSize_);

			this->bandSize_ = new size_type[1];
			this->bandSize_[0] = 1;
			for (size_type d = 0; d < this->dSize_; d++)
			{
				this->bandSize_[0] *= this->bandDims_[d];
			}

			this->bsP_ = new Dty_[this->dSize_];
			this->beP_ = new Dty_[this->dSize_];
			memset(this->bsP_, 0, sizeof(dim_type) * this->dSize_);
			memcpy(this->beP_, this->bandDims_, sizeof(dim_type) * this->dSize_);
		}

		caWTIterator(const self_type& mit) : caCoorIterator<Dty_, Ty_>(mit)
		{
			memcpy(this->bandDims_, mit.bandDims_, sizeof(dim_type) * this->dSize_);
			memcpy(this->bsP_, mit.bsP_, sizeof(dim_type) * this->dSize_);
			memcpy(this->beP_, mit.beP_, sizeof(dim_type) * this->dSize_);
			memcpy(this->bandSize_, mit.bandSize_, sizeof(size_type) * (this->maxLevel_ + 1));

			this->maxLevel_ = mit.maxLevel_;

			this->curLevel_ = mit.curLevel_;
			this->curBandNum_ = mit.curBandNum_;

			this->ptrBegin_ = mit.ptrBegin_;
		}

		~caWTIterator()
		{
			delete[] this->bandDims_;
			delete[] this->bandSize_;
			delete[] this->bsP_;
			delete[] this->beP_;
		}

	public:
		virtual void next(const unsigned int dim)
		{
			if (this->coor_[dim] + 1 < this->beP_[dim])
			{
				this->coor_[dim]++;
				this->ptr_++;
			} else
			{
				if ((dim > 0 && this->basisDim_ == dim - 1) || (dim == 0 && this->basisDim_ == this->dSize_ - 1))
				{
					// If reach to the end of the current band
					// Then move to next band
					this->moveToNextBand();
				} else
				{
					if (dim > 0)
					{
						this->next(dim - 1);
					} else
					{
						this->next(this->dSize_ - 1);
					}
					this->ptr_++;
					this->coor_[dim] = this->bsP_[dim];
				}
			}
		}

		void setMaxLevel(size_type maxLevel)
		{
			this->maxLevel_ = maxLevel;

			this->calcBandDims(maxLevel);
			this->calcBandSize(maxLevel);
			this->calcBandBoundary(maxLevel);
		}

		void setCurLevel(size_type level)
		{
			assert(level <= this->maxLevel_);

			this->curLevel_ = level;
			this->curBandNum_ = 0;
			this->calcBandBoundary(this->curLevel_, this->curBandNum_);
		}

		void setCurBand(size_type band)
		{
			assert(band != 0 || (band == 0 && this->curLevel_ == this->maxLevel_));
			assert(band < pow(2, this->dSize_));

			this->curBandNum_ = band;
			this->calcBandBoundary(this->curLevel_, band);
		}

	protected:
		void moveToNextBand()
		{
			if (this->curBandNum_ + 1 >= pow(2, this->dSize_))
			{
				if (this->curLevel_ < 0)
				{
					return;
				}

				// Except maxLevel, curBandNum starts from 1.
				this->curBandNum_ = 1;
				this->curLevel_--;
			} else
			{
				this->curBandNum_++;
			}

			this->calcBandBoundary();
			this->moveTo(caCoor<Dty_>(this->dSize_, this->bsP_));
		}

		void moveToPrevBand()
		{
			// Band num 0 only exists in maxLevel.
			// In other levels, band num starts from 1.
			if (this->curBandNum_ <= 1)
			{
				if (this->curLevel_ >= this->maxLevel_)
				{
					this->curBandNum_ = 0;
					return;
				}

				this->curBandNum_ = pow(2, this->dSize_) - 1;
				this->curLevel_++;
			} else
			{
				this->curBandNum_--;
			}

			this->calcBandBoundary();
			this->moveTo(caCoor<Dty_>(this->dSize_, this->beP_));
		}

		void calcBandBoundary(size_type level = -1, size_type bandNum = -1)
		{
			if (level == -1) level = this->curLevel_;
			if (bandNum == -1) bandNum = this->curBandNum_;

			dim_pointer dims = this->getBandDims(this->curLevel_);
			memset(this->bsP_, 0, sizeof(dim_type) * this->dSize_);
			memset(this->beP_, 0, sizeof(dim_type) * this->dSize_);

			for (size_type d = this->dSize_ - 1; d + 1 > 0; d--)
			{
				if (bandNum & 0x1)
				{
					this->bsP_[d] = dims[d];
					this->beP_[d] = dims[d] << 1;
				} else
				{
					this->beP_[d] = dims[d];
				}
			}
		}

		// Band dimensions
		void calcBandDims(size_type level)
		{
			assert(level <= this->maxLevel_);

			dim_pointer temp = new dim_type[this->dSize_ * (level + 1)];
			
			// Band dim for level 0 is half of data dims
			for (size_type d = 0; d < this->dSize_; d++)
			{
				temp[d] = this->dims_[d] >> 1;
			}

			// Rests are half of previous band dim
			for (size_type l = 1; l <= level; l++)
			{
				for (size_type d = 0; d < this->dSize_; d++)
				{
					temp[this->dSize_ * l + d] = (temp[this->dSize_ * (l - 1) + d] >> 1);
				}
			}

			// Replace bandDims
			delete[] this->bandDims_;
			this->bandDims_ = temp;
		}

		dim_pointer getBandDims(size_type level)
		{
			assert(level <= this->maxLevel_);
			return this->bandDims_ + (this->dSize_ * level);
		}

		// Number of element in a band
		void calcBandSize(size_type level)
		{
			assert(level <= this->maxLevel_);

			size_type* temp = new size_type[level + 1];
			for (size_type l = 0; l <= level; l++)
			{
				dim_type* curBandDim = this->getBandDims(l);
				temp[l] = 1;
				for (size_type d = 0; d < this->dSize_; d++)
				{
					temp[l] *= curBandDim[d];
				}
			}

			delete[] this->bandSize_;
			this->bandSize_ = temp;
		}

		size_type getBandSize(size_type level)
		{
			assert(level <= this->maxLevel_);
			return this->bandSize_[level];
		}

		virtual dim_type moveTo(const caCoor<Dty_>& coor)
		{
			if (this->dSize_ != coor.size())
			{
				throw std::exception("moveTo - different dimension size");
			}

			// Find which level
			size_type level;
			dim_type* levelBoundary;
			for (level = this->maxLevel_; level > 0; level--)
			{
				levelBoundary = this->getBandDims(level - 1);
				bool flag = true;
				for (int d = this->dSize_ - 1; d >= 0; d--)
				{
					if (coor[d] > levelBoundary[d])
					{
						flag = false;
						break;
					}
				}

				if (flag)
				{
					break;
				}
			}

			// Set current ptr, level, bandnum
			this->setCurLevel(level);
			this->ptr_ = this->ptrBegin_;
			size_type bandNum = 0;
			size_type offset = 1;
			dim_type* curBandDim = this->getBandDims(level);
			for (int d = this->dSize_ - 1; d >= 0; d--)
			{
				this->coor_[d] = coor[d];
				bool flag = size_type(coor[d] / curBandDim[d]);

				size_type innerCoor = coor[d];
				if (flag)
				{
					innerCoor -= curBandDim[d];
				}

				this->ptr_ += offset * innerCoor;

				offset *= curBandDim[d];
				bandNum |= flag << d;
			}
			this->curBandNum_ = bandNum;
			this->ptr_ += this->getBandSize(level) * this->curBandNum_;
		}

		virtual size_t posToSeq(const size_type pos)
		{
			size_type seq = 0;
			
			// TODO :: Calculate seq
			throw NotImplemented();

			return seq;
		}

	protected:
		dim_pointer bandDims_;
		dim_pointer bsP_;
		dim_pointer beP_;
		size_type* bandSize_;
		size_type maxLevel_;
		
		size_type curLevel_;
		size_type curBandNum_;

		value_pointer ptrBegin_;

	private:
		FRIEND_TEST(caIterators, caWTIteratorMoveTo);

	};
}

#endif // _caIterators_
