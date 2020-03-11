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
		using self_type = caCoor;
		using size_type = size_t;
		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_reference = const Dty_&;

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

		self_type& operator++()
		{
			this->coor_[this->dSize_ - 1];
			return *this;
		}
		self_type operator++(int)
		{
			caCoor<Dty_> tmp(*this);
			operator++();
			return tmp;
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
		using self_type = caCoorIterator<Dty_, Ty_>;
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
			const Dty_ offset = this->getDimOffset(dim);
			if (this->coor_[dim] + 1 < this->eP_[dim])
			{
				this->coor_[dim]++;
				this->ptr_ += offset;
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

				this->moveDimCoor(dim, this->sP_[dim], offset);
			}
		}

		virtual void prev(const unsigned int dim)
		{
			const Dty_ offset = this->getDimOffset(dim);
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

				this->moveDimCoor(dim, this->eP_[dim] - 1, offset);
			}
		}

		virtual void moveTo(const caCoor<Dty_>& coor)
		{
			assert(this->dSize_ == coor.size());

			size_type offset = 1;
			for (Dty_ d = this->dSize_ - 1; d != (Dty_)-1; d--)
			{
				//if (coor[i] >= this->coor_[i])
				//{
				//	this->ptr_ += (coor[i] - this->coor_[i]) * offset;
				//}
				//else 
				//{
				//	this->ptr_ -= (this->coor_[i] - coor[i]) * offset;
				//}
				//this->coor_[i] = coor[i];

				this->moveDimCoor(d, coor[d], offset);
				offset *= this->dims_[d];
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

		inline void moveDimCoor(const size_type dim, const Dty_ coor, const Dty_ offset)
		{
			if (coor >= this->coor_[dim])
			{
				this->ptr_ += (coor - this->coor_[dim]) * offset;
			}
			else
			{
				this->ptr_ -= (this->coor_[dim] - coor) * offset;
			}
			this->coor_[dim] = coor;
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
		using self_type = caCoorRangeIterator;
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
		caWTIterator(value_pointer x, const size_type dSize, dim_const_pointer boundary, size_type maxLevel = 0) :
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
			this->curBand_ = 0;

			// NOTE::BandDims and Band Size will be reassigned in setMaxLevel function
			this->bandDims_ = new dim_type[1];
			this->bandSize_ = new size_type[1];

			this->bsP_ = new Dty_[this->dSize_];
			this->beP_ = new Dty_[this->dSize_];
			memset(this->bsP_, 0, sizeof(dim_type) * this->dSize_);
			memcpy(this->beP_, this->bandDims_, sizeof(dim_type) * this->dSize_);

			this->setMaxLevel(maxLevel);
		}

		caWTIterator(const self_type& mit) : caCoorIterator<Dty_, Ty_>(mit)
		{
			memcpy(this->bandDims_, mit.bandDims_, sizeof(dim_type) * this->dSize_);
			memcpy(this->bsP_, mit.bsP_, sizeof(dim_type) * this->dSize_);
			memcpy(this->beP_, mit.beP_, sizeof(dim_type) * this->dSize_);
			memcpy(this->bandSize_, mit.bandSize_, sizeof(size_type) * (this->maxLevel_ + 1));

			this->maxLevel_ = mit.maxLevel_;

			this->curLevel_ = mit.curLevel_;
			this->curBand_ = mit.curBand_;

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
			const Dty_ offset = this->getDimOffset(dim);
			if (this->coor_[dim] + 1 < std::min(this->beP_[dim], this->eP_[dim]))
			{
				this->coor_[dim]++;
				this->ptr_ += offset;
			} else
			{
				if (dim > 0)
				{
					if (this->basisDim_ == dim - 1)
					{
						this->moveToNextBand();
						return;
					}
					this->next(dim - 1);
				}
				else
				{
					if (this->basisDim_ == this->dSize_ - 1)
					{
						this->moveToNextBand();
						return;
					}
					this->next(this->dSize_ - 1);
				}

				this->moveDimCoor(dim, std::max(this->bsP_[dim], this->sP_[dim]), offset);

				/////
				//if ((dim > 0 && this->basisDim_ == dim - 1) || (dim == 0 && this->basisDim_ == this->dSize_ - 1))
				//{
				//	// If reach to the end of the current band
				//	// Then move to next band
				//	this->moveToNextBand();
				//} else
				//{
				//	if (dim > 0)
				//	{
				//		this->next(dim - 1);
				//	} else
				//	{
				//		this->next(this->dSize_ - 1);
				//	}
				//	//this->ptr_++;
				//	this->coor_[dim] = std::max(this->bsP_[dim], this->sP_[dim]);
				//}
			}
		}

		virtual void moveTo(const caCoor<Dty_>& coor)
		{
			if (this->dSize_ != coor.size())
			{
				throw std::exception("moveTo - different dimension size");
			}

			// Find which level and band
			size_type level = this->findLevel(coor);
			size_type band = this->findBand(coor, level);

			// Set current level, band, ptr.
			this->setCurLevel(level);
			this->setCurBand(band);
			this->ptr_ = this->ptrBegin_;
			this->ptr_ += this->getBandSize(level) * this->curBand_;
			this->ptr_ += this->posToSeq(coor);

			this->setCurCoor(coor);
		}

		void setMaxLevel(size_type maxLevel)
		{
			this->maxLevel_ = maxLevel;

			this->calcBandDims();
			this->calcBandSize();
		}

		void setCurLevel(size_type level, bool adjustCoor = false)
		{
			if (level > this->maxLevel_)
			{
				std::cout << level << " / " << this->maxLevel_ << std::endl;
			}
			assert(level <= this->maxLevel_);

			this->curLevel_ = level;

			if (adjustCoor)
			{
				if (level == this->maxLevel_)
				{
					this->setCurBand(0);
				} else
				{
					// band 0 exists in a maxLevel only.
					this->setCurBand(1);
				}
			}
		}

		void setCurBand(size_type band, bool adjustCoor = false)
		{
			assert(band != 0 || (band == 0 && this->curLevel_ == this->maxLevel_));
			assert(band < pow(2, this->dSize_));

			this->curBand_ = band;
			this->calcBandBoundary();

			if (adjustCoor && !isInside(*this, this->bsP_, this->beP_))
			{
				this->moveTo(caCoor<Dty_>(this->dSize_, this->bsP_));
			}
		}

		void setCurCoor(const caCoor<Dty_>& coor)
		{
			for (dim_type d = 0; d < this->dSize_; d++)
			{
				this->coor_[d] = coor[d];
			}
		}

		_NODISCARD size_type curBand()
		{
			return this->curBand_;
		}

		_NODISCARD size_type curLevel()
		{
			return this->curLevel_;
		}

		bool isInside(const caCoor<Dty_>& coor, dim_type* sP, dim_type* eP)
		{
			for (size_type d = 0; d < coor.size(); d++)
			{
				if (coor[d] < sP[d] || eP[d] <= coor[d])
				{
					return false;
				}
			}

			return true;
		}

		// Move to first child of the current coordinate
		void moveToChild()
		{
			assert(this->curLevel_ > 0);
			assert(this->curBand_ != 0 || (this->curBand_ == 0 && this->curLevel_ == this->maxLevel_));
			
			caCoor<Dty_> newCoor(this);
			if (this->curBand_ == 0)
			{
				size_type odd = 0;
				for (size_type d = this->dSize_ - 1; d + 1 > 0; d--)
				{
					newCoor[d] = ((int)this->coor[d] / 2) * 2;
					if (this->coor[d] & 0x1)
					{
						newCoor[d] += this->bandSize_[d];
						odd++;
					}
				}
				assert(odd != 0);	// some coor[d] is odd
			} else
			{
				// Other level bands
				for (size_type d = this->dSize_ - 1; d + 1 > 0; d--)
				{
					newCoor[d] = this->coor[d] * 2;
				}
			}

			this->moveTo(newCoor);
		}

		void moveToNextSibling()
		{
			size_type cur = 0;
			for (size_type d = 0; d < this->dSize_; d++)
			{
				cur += (this->coor[d] % 2);

				if (d != this->dSize_ - 1)
					cur *= 2;
			}
			size_type next = cur + 1;
			assert(next != pow(2, this->dSize_));

			caCoor<Dty_> newCoor(this);
			for (size_type d = this->dSize_ - 1; d + 1 > 0; d--)
			{
				newCoor[d] = ((next % 2) - (cur % 2));
				newCoor[d] += this->coor[d];

				next /= 2;
				cur /= 2;
			}

			this->moveTo(newCoor);
		}

		void moveToChildBand()
		{
			assert(this->curBand_ != 0);
			assert(this->curLevel_ > 0);
			
			this->setCurLevel(this->curLevel_ - 1);
			this->setCurBand(this->curBand_, true);
		}

	protected:
		void moveToNextBand()
		{
			if (this->curBand_ + 1 >= pow(2, this->dSize_))
			{
				if (this->curLevel_ == 0)
				{
					return;
				}

				// Except maxLevel, curBandNum starts from 1.
				this->setCurLevel(this->curLevel_ - 1);
				this->setCurBand(1);
			} else
			{
				this->setCurBand(this->curBand_ + 1);
			}

			this->moveTo(caCoor<Dty_>(this->dSize_, this->bsP_));
		}

		void moveToPrevBand()
		{
			// Band num 0 only exists in maxLevel.
			// In other levels, band num starts from 1.
			if (this->curBand_ <= 1)
			{
				if (this->curLevel_ >= this->maxLevel_)
				{
					this->curBand_ = 0;
					return;
				}

				this->curBand_ = pow(2, this->dSize_) - 1;
				this->curLevel_++;
			} else
			{
				this->curBand_--;
			}

			this->moveTo(caCoor<Dty_>(this->dSize_, this->beP_));
		}

		inline void moveDimCoor(const size_type dim, const Dty_ coor, const Dty_ offset)
		{
			if (coor >= this->coor_[dim])
			{
				this->ptr_ += (coor - this->coor_[dim]) * offset;
			}
			else
			{
				this->ptr_ -= (this->coor_[dim] - coor) * offset;
			}
			this->coor_[dim] = coor;
		}

		size_t getDimOffset(_In_range_(0, dSize_ - 1) const unsigned int dim)
		{
			if (dim == this->basisDim_)
			{
				return this->offset_;
			}

			size_t offset = 1;
			dim_type* curBandDims = this->getBandDims(this->curLevel_);
			for (unsigned int i = this->dSize_ - 1; i > dim; i--)
			{
				offset *= curBandDims[i];
			}

			//std::cout << "dim : " << dim << ", getOffset: " << offset << std::endl;
			return offset;
		}

		void calcBandBoundary()
		{
			dim_pointer dims = this->getBandDims(this->curLevel_);
			memset(this->bsP_, 0, sizeof(dim_type) * this->dSize_);
			memset(this->beP_, 0, sizeof(dim_type) * this->dSize_);


			for (size_type d = 0; d < this->dSize_; d++)
			{
				if ((this->curBand_ >> (this->dSize_ - 1 - d)) & 0x1)
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
		void calcBandDims()
		{
			dim_pointer temp = new dim_type[this->dSize_ * (this->maxLevel_ + 1)];
			
			// Band dim for level 0 is half of data dims
			for (size_type d = 0; d < this->dSize_; d++)
			{
				temp[d] = this->dims_[d] >> 1;
			}

			// Rests are half of previous band dim
			for (size_type l = 1; l <= this->maxLevel_; l++)
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
		void calcBandSize()
		{
			size_type* temp = new size_type[this->maxLevel_ + 1];
			for (size_type l = 0; l <= this->maxLevel_; l++)
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

		// TODO::erase posToSeq func (here and parent)
		virtual size_t posToSeq(const size_type pos)
		{
			size_type seq = 0;
			
			throw NotImplemented();

			return seq;
		}

		virtual size_type posToSeq(const caCoor<Dty_>& coor)
		{
			size_type seq = 0;
			size_type offset = 1;
			dim_type* curBandDim = this->getBandDims(this->curLevel_);

			for (int d = this->dSize_ - 1; d >= 0; d--)
			{
				if (size_type(coor[d]) / size_type(curBandDim[d]))
				{
					seq += offset * (size_type)(coor[d] - curBandDim[d]);
				} else
				{
					seq += offset * (size_type)coor[d];
				}

				offset *= curBandDim[d];
			}

			return seq;
		}

		virtual size_type findLevel(const caCoor<Dty_>& coor)
		{
			size_type level = 0;
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
			return level;
		}

		virtual size_type findBand(const caCoor<Dty_>& coor, size_type level)
		{
			size_type band = 0;
			dim_type* curBandDim = this->getBandDims(level);
			for (int d = this->dSize_ - 1; d >= 0; d--)
			{
				band |= (size_type(coor[d]) / size_type(curBandDim[d])) << (this->dSize_ - 1 - d);
			}
			return band;
		}

	protected:
		dim_pointer bandDims_;
		dim_pointer bsP_;
		dim_pointer beP_;
		size_type* bandSize_;
		size_type maxLevel_;
		
		size_type curLevel_;
		size_type curBand_;

		value_pointer ptrBegin_;

	private:
		FRIEND_TEST(caIterators, caWTIteratorMoveTo);
		FRIEND_TEST(caIterators, caWTIteratorCalcBandDim);
	};

	// Cannot over band boundaries and level boundaries
	template <typename Dty_, typename Ty_>
	class caWTRangeIterator : public caWTIterator<Dty_, Ty_>
	{
		using self_type = caWTRangeIterator<Dty_, Ty_>;
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
		caWTRangeIterator(value_pointer x, const size_type dSize, dim_const_pointer boundary,
			dim_const_pointer sP, dim_const_pointer eP, size_type maxLevel = 0) :
			caWTIterator<Dty_, Ty_>(x, dSize, boundary, maxLevel)
		{
			this->eP_ = new Dty_[dSize];
			memcpy(this->sP_, sP, dSize * sizeof(dim_type));
			memcpy(this->eP_, eP, dSize * sizeof(dim_type));

			this->moveToStart();
		}

		caWTRangeIterator(const self_type& mit) : caWTIterator<Dty_, Ty_>(mit)
		{
			this->eP_ = new Dty_[mit.dSize_];
			memcpy(this->eP_, mit.eP_, mit.dSize_ * sizeof(dim_type));
		}

		~caWTRangeIterator()
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
}

#endif // _caIterators_
