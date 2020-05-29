#pragma once
#ifndef _MSDB_COORDINATE_H_
#define _MSDB_COORDINATE_H_

#include <iostream>

namespace msdb
{
	template <typename Dty_>
	class coordinate
	{
	public:
		//using difference_type = ptrdiff_t;
		using self_type = coordinate;
		using size_type = size_t;
		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_reference = const Dty_&;

	public:
		coordinate(const size_type dSize = 0) : dSize_(dSize)
		{
			this->coor_ = new dim_type[this->dSize_]();
		}

		coordinate(const size_type dSize, const dim_pointer coor) : dSize_(dSize)
		{
			this->coor_ = new dim_type[this->dSize_];
			memcpy(this->coor_, coor, this->dSize_ * sizeof(dim_type));
		}

		coordinate(const self_type& mit) : dSize_(mit.dSize_)
		{
			this->coor_ = new dim_type[this->dSize_];
			memcpy(this->coor_, mit.coor_, mit.dSize_ * sizeof(dim_type));
		}

		~coordinate()
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

		self_type& operator=(const self_type& mit)
		{
			delete[] this->coor_;
			this->dSize_ = mit.dSize_;
			this->coor_ = new dim_type[mit.dSize_];
			memcpy(this->coor_, mit.coor_, mit.dSize_ * sizeof(dim_type));

			return *this;
		}

		self_type& operator++()
		{
			this->coor_[this->dSize_ - 1];
			return *this;
		}
		self_type operator++(int)
		{
			coordinate<Dty_> tmp(*this);
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
	class coorIterator : public coordinate<Dty_>
	{
	public:
		using self_type = coorIterator<Dty_, Ty_>;
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
		coorIterator(value_pointer x, const size_type dSize, dim_const_pointer boundary) :
			coordinate<Dty_>(dSize), ptr_(x), offset_(1)
		{
			this->dims_ = new dim_type[this->dSize_];
			memcpy(this->dims_, boundary, this->dSize_ * sizeof(dim_type));

			this->basisDim_ = this->dSize_ - 1;
			this->sP_ = new dim_type[this->dSize_]();
			this->eP_ = this->dims_;

			this->calcVsize();
		}

		coorIterator(const self_type& mit) : coordinate<Dty_>(mit), ptr_(mit.ptr_), offset_(mit.offset_),
			basisDim_(mit.basisDim_), vSize_(mit.vSize_)
		{
			this->dims_ = new dim_type[mit.dSize_];
			memcpy(this->dims_, mit.dims_, mit.dSize_ * sizeof(dim_type));

			this->sP_ = new dim_type[mit.dSize_];
			memcpy(this->sP_, mit.sP_, mit.dSize_ * sizeof(dim_type));

			this->eP_ = this->dims_;
		}

		~coorIterator()
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
		virtual void next(const size_type dim)
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

		virtual void prev(const size_type dim)
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

		virtual void moveTo(const coordinate<Dty_>& coor)
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
			this->moveTo(coordinate<Dty_>(this->dSize_, this->sP_));
		}

		coordinate<Dty_> coor() { return coordinate<Dty_>(*this); }

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
			coorIterator<Dty_, Ty_> tmp(*this);
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
		_NODISCARD const coorIterator& operator[](size_type pos) const
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
			} else
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
			for (size_type i = this->dSize_ - 1; i > dim; i--)
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
	class coorRangeIterator : public coorIterator<Dty_, Ty_>
	{
		using self_type = coorRangeIterator;
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
		coorRangeIterator(value_pointer x, const size_type dSize, dim_const_pointer boundary,
						  dim_const_pointer sP, dim_const_pointer eP) :
			coorIterator<Dty_, Ty_>(x, dSize, boundary)
		{
			this->eP_ = new Dty_[dSize];
			memcpy(this->sP_, sP, dSize * sizeof(dim_type));
			memcpy(this->eP_, eP, dSize * sizeof(dim_type));

			this->moveToStart();
		}

		coorRangeIterator(const coorRangeIterator& mit) : coorIterator<Dty_, Ty_>(mit)
		{
			this->eP_ = new Dty_[mit.dSize_];
			memcpy(this->eP_, mit.eP_, mit.dSize_ * sizeof(dim_type));
		}

		~coorRangeIterator()
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

#endif