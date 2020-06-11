#pragma once
#ifndef _MSDB_COORDINATE_H_
#define _MSDB_COORDINATE_H_

#include <util/element.h>
#include <system/exceptions.h>
#include <array/dimensionId.h>
#include <iostream>
#include <cassert>

namespace msdb
{
typedef int64_t		position_t;
typedef int64_t		value_t;

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
			this->coor_[this->dSize_ - 1]++;
			return *this;
		}
		self_type operator++(int)
		{
			coordinate<Dty_> tmp(*this);
			operator++();
			return tmp;
		}

		_NODISCARD inline constexpr size_type size() const noexcept
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

	protected:
		size_type dSize_;
		dim_pointer coor_;
	};

	template <typename Dty_>
	class coordinateIterator
	{
	public:
		using self_type = coordinateIterator<Dty_>;
		using size_type = size_t;
		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_reference = const Dty_&;

		using coordinate_type = coordinate<Dty_>;

	public:
		coordinateIterator(const size_type dSize, dim_const_pointer dims)
			: coor_(dSize), end_(false)
		{
			this->dims_ = new dim_type[this->dSize()];
			memcpy(this->dims_, dims, this->dSize() * sizeof(dim_type));

			this->basisDim_ = this->dSize() - 1;
			this->sP_ = new dim_type[this->dSize()]();
			this->eP_ = this->dims_;
		}
		
		coordinateIterator(const self_type& mit)
			: coor_(mit), basisDim_(mit.basisDim_), end_(mit.end_)
		{
			this->dims_ = new dim_type[mit.dSize()];
			memcpy(this->dims_, mit.dims_, mit.dSize() * sizeof(dim_type));

			this->sP_ = new dim_type[mit.dSize()];
			memcpy(this->sP_, mit.sP_, mit.dSize() * sizeof(dim_type));

			this->eP_ = this->dims_;
		}

		~coordinateIterator()
		{
			delete[] this->dims_;
			delete[] this->sP_;
		}

	public:
		virtual void setBasisDim(_In_range_(0, dSize() - 1) const unsigned int dim)
		{
			if (this->dSize() <= dim)
			{
				_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_OUT_OF_RANGE, "setBasisDim - too large dim"));
			}

			this->basisDim_ = dim;
		}

		_NODISCARD inline size_type dSize()
		{
			return this->coor_.size();
		}

		_NODISCARD inline coordinate_type coor() { return this->coor_; }

		_NODISCARD inline bool end() { return this->end_; }
		_NODISCARD inline bool front() { return this->front_; }

		virtual void next()
		{
			if (this->end_)		return;

			dimensionId curDim = this->basisDim_;
			do
			{
				if (this->coor_[curDim] + 1 < this->eP_[curDim])
				{
					// move next and return
					this->coor_[curDim]++;
					return;
				}

				// move current dimension coordinate to sP_
				this->coor_[curDim] = this->sP_[curDim];

				if (curDim != 0)		--curDim;
				else				curDim = this->dSize() - 1;

			} while (curDim != this->basisDim_);

			this->end_ = true;
		}
		virtual void prev()
		{
			if (this->front_)		return;

			dimensionId curDim = this->basisDim_;
			do
			{
				if (this->coor_[curDim] > this->sP_[curDim])
				{
					// move next and return
					this->coor_[curDim]--;
					return;
				}

				// move current dimension coordinate to sP_
				this->coor_[curDim] = this->eP_[curDim] - 1;

				curDim = (++curDim) % this->dSize();
			} while (curDim != this->basisDim_);

			this->front_ = true;
		}

		virtual void moveTo(const coordinate_type& coor)
		{
			assert(this->dSize() == coor.size());
			this->coor_ = coor;
		}
		virtual void moveToStart()
		{
			this->moveTo(coordinate<Dty_>(this->dSize(), this->sP_));
		}

		// forward
		self_type& operator++()
		{
			this->next();
			return *this;
		}
		self_type operator++(int)
		{
			self_type tmp(*this);
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
			this->prev();
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

	protected:
		coordinate_type coor_;		// coordinate
		dimensionId basisDim_;		// current dimension

		dim_pointer dims_;		// original array dimension size
		dim_pointer sP_;		// range start point
		dim_pointer eP_;		// range end point

		bool end_;				// is iterator at the end
		bool front_;			// is iterator at the front

		// TODO:: replace sP_ to zero
		// static std::vector<dim_type> zeroP_;
	};

	template <typename Dty_>
	class itemIterator : public coordinateIterator<Dty_>
	{
	public:
		using self_type = itemIterator<Dty_>;
		using base_type = coordinateIterator<Dty_>;
		using coordinate_type = coordinate<Dty_>;
		using size_type = size_t;
		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_reference = const Dty_&;

	public:
		itemIterator(void* ptr, eleType eType, const size_type dSize, dim_const_pointer dims)
			: base_type(dSize, dims), curSeqPos_(0),
			eType_(eType), eSize_(getEleSize(eType))
		{
			this->basisDimOffset_ = this->eSize_;
			this->ptr_ = reinterpret_cast<char*>(ptr);
			this->initSeqLimit();
		}

		virtual void moveTo(const coordinate_type& coor)
		{
			coordinate_type coorOld = this->coor_;
			base_type::moveTo(coor);

			//this->adjustPtr(coorOld, coor);
			size_type offset = this->eSize_;
			for (dimensionId d = this->dSize() - 1; d != (dimensionId)-1; d--)
			{
				this->ptr_ += (coor[d] - coorOld[d]) * offset;
				offset *= this->dims_[d];
			}
		}

		virtual void setBasisDim(const unsigned int dim)
		{
			this->basisDimOffset_ = this->getDimOffset(dim);
			base_type::setBasisDim(dim);
		}

	public:
		coordinate_type coor() { return coordinate_type(this->coor_); }

		bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_; }
		bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_; }

		element operator*() { return element((void*)ptr_, this->eType_); }
		element operator->() { return element((void*)ptr_, this->eType_); }

	protected:
		virtual void next()
		{
			if(this->end_)		return;

			dimensionId curDim = this->basisDim_;
			do
			{
				dim_type offset = this->getDimOffset(curDim);
				if(this->coor_[curDim] + 1 < this->eP_[curDim])
				{
					// move next and return
					this->coor_[curDim]++;
					this->ptr_ += offset;
					return;
				}

				// move current dimension coordinate to sP_
				this->ptr_ -= ((dim_type)this->coor_[curDim] - (dim_type)this->sP_[curDim]) * offset;
				this->coor_[curDim] = this->sP_[curDim];

				if(curDim != 0)		--curDim;
				else				curDim = this->dSize() - 1;

			} while (curDim != this->basisDim_);

			this->end_ = true;
		}
		virtual void prev()
		{
			if (this->front_)		return;

			dimensionId curDim = this->basisDim_;
			do
			{
				dim_type offset = this->getDimOffset(curDim);
				if (this->coor_[curDim] > this->sP_[curDim])
				{
					// move next and return
					this->coor_[curDim]--;
					this->ptr_ += offset;
					return;
				}

				// move current dimension coordinate to sP_
				this->ptr_ += ((dim_type)this->eP_[curDim] - (dim_type)1 - (dim_type)this->coor_[curDim]) * offset;
				this->coor_[curDim] = this->eP_[curDim] - 1;

				curDim = (++curDim) % this->dSize();
			} while (curDim != this->basisDim_);

			this->front_ = true;
		}

		// call this function before you change a basisDim
		size_t getDimOffset(const unsigned int dim)
		{
			if (dim == this->basisDim_)
			{
				return this->basisDimOffset_;
			}

			size_t offset = this->eSize_;
			for (size_type i = this->dSize() - 1; i > dim; i--)
			{
				offset *= this->dims_[i];
			}

			//std::cout << "dim : " << dim << ", getOffset: " << offset << std::endl;
			return offset;
		}

		// basis dim pos
		virtual size_t posToSeq(const size_type pos)
		{
			size_type left = pos * this->basisDimOffset_;
			size_type seq = 0;
			size_type offset = this->eSize_;
			for (int d = this->basisDim_; d >= 0; d--)
			{
				seq += (left % this->dims_[d]) * offset;
				left = left / this->dims_[d];
				offset *= this->dims_[d];
			}

			for (int d = this->dSize() - 1; d > this->basisDim_; d--)
			{
				seq += (left % this->dims_[d]) * offset;
				left = left / this->dims_[d];
				offset *= this->dims_[d];
			}

			return seq;
		}

	private:
		size_type initSeqLimit()
		{
			this->seqLimit_ = this->dims_[0];
			for (int i = 1; i < this->dSize(); i++)
			{
				this->seqLimit_ *= this->dims_[i];
			}
			return this->seqLimit_;
		}

	protected:
		char* ptr_;					// pointer to element
		eleType eType_;				// element type
		size_type eSize_;			// element size in byte
		
		// sequencial pos
		size_type curSeqPos_;
		size_type seqLimit_;
		size_type basisDimOffset_;
	};

	template <typename Dty_>
	class itemRangeIterator : public itemIterator<Dty_>
	{
	public:
		using self_type = itemRangeIterator<Dty_>;
		using base_type = itemIterator<Dty_>;

		using coordinate_type = base_type::coordinate_type;
		using size_type = base_type::size_type;
		using dim_type = base_type::dim_type;
		using dim_pointer = base_type::dim_pointer;
		using dim_const_pointer = base_type::dim_const_pointer;
		using dim_reference = base_type::dim_reference;
		using dim_const_reference = base_type::dim_const_reference;

	public:
		itemRangeIterator(void* ptr, eleType eType, const size_type dSize, dim_const_pointer dim,
						  dim_const_pointer sP, dim_const_pointer eP)
			: base_type(ptr, eType, dSize, dim)
		{
			this->eP_ = new Dty_[dSize];
			memcpy(this->sP_, sP, dSize * sizeof(dim_type));
			memcpy(this->eP_, eP, dSize * sizeof(dim_type));

			assert(this->initCheckSpEp());

			this->moveToStart();
		}

		itemRangeIterator(const self_type& mit) : base_type(mit)
		{
			this->eP_ = new Dty_[mit.dSize()];
			memcpy(this->eP_, mit.eP_, mit.dSize() * sizeof(dim_type));
		}

		~itemRangeIterator()
		{
			delete[] this->eP_;
		}

	protected:
		// Check if sP_ and eP_ has proper coordinate.
		bool initCheckSpEp()
		{
			for (dimensionId d = 0; d < this->dSize(); d++)
			{
				if (this->sP_[d] > this->eP_[d] || this->dims_[d] < this->eP_[d])
				{
					return false;
				}
			}

			return true;
		}
	};

	// TODO:: Do not inherit caCoor
	template <typename Dty_, typename Ty_>
	class coorIterator : public coordinate<Dty_>
	{
	public:
		using self_type = coorIterator<Dty_, Ty_>;
		using coordinate_type = coordinate<Dty_>;
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

		virtual void moveTo(const coordinate_type& coor)
		{
			assert(this->dSize_ == coor.size());

			size_type offset = 1;
			for (Dty_ d = this->dSize_ - 1; d != (Dty_)-1; d--)
			{
				if (coor[d] >= this->coor_[d])
				{
					this->ptr_ += (coor[d] - this->coor_[d]) * offset;
				}
				else 
				{
					this->ptr_ -= (this->coor_[d] - coor[d]) * offset;
				}
				this->coor_[d] = coor[d];

				this->moveDimCoor(d, coor[d], offset);
				offset *= this->dims_[d];
			}
		}

		void moveToStart()
		{
			this->moveTo(coordinate<Dty_>(this->dSize_, this->sP_));
		}

		coordinate<Dty_> coor() { return static_cast<coordinate_type>(*this); }

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
		_NODISCARD const self_type& operator[](size_type pos) const
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

	using coor = coordinate<position_t>;
	using coorItr = coorIterator<position_t, value_t>;
	using coorRangeItr = coorRangeIterator<position_t, value_t>;
}

#endif