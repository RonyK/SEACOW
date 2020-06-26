#pragma once
#ifndef _MSDB_COORDINATE_H_
#define _MSDB_COORDINATE_H_

#include <util/element.h>
#include <system/exceptions.h>
#include <array/dimensionId.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <initializer_list>

namespace msdb
{
typedef int64_t		position_t;

enum class iterateMode
{
	ALL,		// Iterate all possible coordinate
	EXIST		// Only exist items
};

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

	coordinate(const std::vector<dim_type>& coorVec)
	{
		this->coor_ = new dim_type[coorVec.size()];
		memcpy(this->coor_, coorVec.data(), coorVec.size() * sizeof(dim_type));
	}

	coordinate(const self_type& mit) : dSize_(mit.dSize_)
	{
		this->coor_ = new dim_type[this->dSize_];
		memcpy(this->coor_, mit.coor_, mit.dSize_ * sizeof(dim_type));
	}

	coordinate(std::initializer_list<dim_type> lst)
	{
		this->dSize_ = lst.size();
		this->coor_ = new dim_type[this->dSize_];

		auto it = lst.begin();
		for (size_type d = 0; d < this->dSize_; d++, it++)
		{
			this->coor_[d] = *it;
		}
	}

	~coordinate()
	{
		delete[] this->coor_;
	}

public:
	//////////////////////////////
	// Getter
	_NODISCARD inline constexpr size_type size() const noexcept
	{
		return this->dSize_;
	}
	dim_pointer data()
	{
		return this->coor_;
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	// Comparison
	bool operator==(const self_type& rhs) const
	{
		return this->dSize_ == rhs.dSize_ && (memcmp(this->coor_, rhs.coor_, this->dSize * sizeof(dim_type)) == 0);
	}
	bool operator==(const dim_type* rhs) const
	{
		for(dimensionId d = 0; d < this->dSize_; ++d)
		{
			if(this->coor_[d] != rhs[d])
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const self_type& rhs) const
	{
		return this->dSize_ != rhs.dSize_ || (memcmp(this->coor_, rhs.coor_, this->dSize * sizeof(dim_type)) != 0);
	}
	bool operator!=(const dim_type* rhs) const
	{
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			if (this->coor_[d] != rhs[d])
			{
				return true;
			}
		}
		return false;
	}

	// Assign
	self_type& operator=(const self_type& mit)
	{
		delete[] this->coor_;
		this->dSize_ = mit.dSize_;
		this->coor_ = new dim_type[mit.dSize_];
		this->memcpyCoor(this->coor_, mit.coor_);

		return *this;
	}
	self_type& operator=(const dim_type* mit)
	{
		this->memcpyCoor(this->coor_, mit);

		return *this;
	}

	// Move
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

	// Random access
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
	//////////////////////////////

protected:
	void memcpyCoor(dim_type* dest, const dim_type* src)
	{
		memcpy(dest, src, this->dSize_ * sizeof(dim_type));
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
		: coor_(dSize), dSize_(dSize), end_(false), basisDimOffset_(1), seqPos_(0)
	{
		this->dims_ = new dim_type[this->dSize()];
		this->memcpyDim(this->dims_, dims);

		this->basisDim_ = this->dSize() - 1;
		this->sP_ = new dim_type[this->dSize()]();
		this->eP_ = this->dims_;

		this->initSeqCapacity();
	}

	coordinateIterator(const self_type& mit)
		: coor_(mit.coor_), basisDim_(mit.basisDim_), dSize_(mit.dSize_),
		end_(mit.end_), basisDimOffset_(mit.basisDimOffset_),
		seqPos_(mit.seqPos_), seqCapacity_(mit.seqCapacity_)
	{
		this->dims_ = new dim_type[this->dSize()];
		this->memcpyDim(this->dims_, mit.dims_);

		this->sP_ = new dim_type[this->dSize()];
		this->memcpyDim(this->sP_, mit.sP_);

		this->eP_ = this->dims_;
	}

	~coordinateIterator()
	{
		delete[] this->dims_;
		delete[] this->sP_;
	}

public:
	virtual void setBasisDim(const unsigned int dim)
	{
		if (this->dSize() <= dim)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_OUT_OF_RANGE, "setBasisDim - too large dim"));
		}

		this->basisDimOffset_ = this->getDimOffset(dim);
		this->basisDim_ = dim;
	}

	//////////////////////////////
	// Getter
	//////////////////////////////
	_NODISCARD inline size_type seqPos()
	{
		return this->seqPos_;
	}
	_NODISCARD inline size_type dSize() const
	{
		// this->coor_.size();
		return this->dSize_;
	}
	_NODISCARD inline dim_pointer data() { return this->coor_; }
	_NODISCARD inline coordinate_type& coor() { return this->coor_; }
	_NODISCARD inline coordinate_type dims() const { return coordinate_type(dSize, this->dims_); }
	_NODISCARD inline bool isEnd() const { return this->end_; }
	_NODISCARD inline bool isFront() const { return this->front_; }
	size_type getCapacity()
	{
		return this->seqCapacity_;
	}
	//////////////////////////////

	//////////////////////////////
	// Converting
	//////////////////////////////
	// pos: basis dim pos
	virtual size_type posToSeq(const size_type pos)
	{
		//size_type absolutePos = this->coor_[this->basisDim_] + pos;
		//if(absolutePos < this->sP_[this->basisDim_] || this->eP_[this->basisDim_] <= absolutePos)
		//{
		//	_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_OUT_OF_RANGE, "itemIterator getAt out of range"));
		//}

		size_type left = pos * this->basisDimOffset_;
		size_type seq = 0;
		size_type offset = 1;
		// Do not change int -> dimensionId, 
		// Error happened when d == -1
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
	size_type coorToSeq(const coordinate_type coor)
	{
		size_type seq = 0, offset = 1;
		for (dimensionId d = this->dSize() - 1; d != (dimensionId)-1; d--)
		{
			seq += coor[d] * offset;
			offset *= this->dims_[d];
		}
		return seq;
	}
	//////////////////////////////

	//////////////////////////////
	// Iterating
	//////////////////////////////
	virtual void next()
	{
		if (this->end_)		return;
		this->front_ = false;

		dimensionId curDim = this->basisDim_;
		do
		{
			dim_type offset = this->getDimOffset(curDim);
			if (this->coor_[curDim] + 1 < this->eP_[curDim])
			{
				// move next and return
				this->coor_[curDim]++;
				this->seqPos_ += offset;
				return;
			}

			// move current dimension coordinate to sP_
			this->seqPos_ -= ((dim_type)this->coor_[curDim] - (dim_type)this->sP_[curDim]) * offset;
			this->coor_[curDim] = this->sP_[curDim];

			if (curDim != 0)		--curDim;
			else				curDim = this->dSize() - 1;

		} while (curDim != this->basisDim_);

		this->end_ = true;
	}
	virtual void prev()
	{
		if (this->front_)		return;
		this->end_ = false;

		dimensionId curDim = this->basisDim_;
		do
		{
			dim_type offset = this->getDimOffset(curDim);
			if (this->coor_[curDim] > this->sP_[curDim])
			{
				// move next and return
				this->coor_[curDim]--;
				this->seqPos_ += offset;
				return;
			}

			// move current dimension coordinate to sP_
			this->seqPos_ += ((dim_type)this->eP_[curDim] - (dim_type)1 - (dim_type)this->coor_[curDim]) * offset;
			this->coor_[curDim] = this->eP_[curDim] - 1;

			curDim = (++curDim) % this->dSize();
		} while (curDim != this->basisDim_);

		this->front_ = true;
	}

	virtual void moveTo(const coordinate_type& coor)
	{
		assert(this->dSize() == coor.size());

		size_type offset = 1;
		for (dimensionId d = this->dSize() - 1; d != (dimensionId)-1; d--)
		{
			this->seqPos_ += (coor[d] - this->coor_[d]) * offset;
			this->coor_[d] = coor[d];
			offset *= this->dims_[d];
		}

		this->setFrontEnd();
	}
	virtual void moveToStart()
	{
		this->moveTo(coordinate<Dty_>(this->dSize(), this->sP_));
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
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
	//////////////////////////////

protected:
	// copy dimension value using memcpy
	void memcpyDim(dim_type* dest, const dim_type* src)
	{
		memcpy(dest, src, this->dSize() * sizeof(dim_type));
	}

	size_type initSeqCapacity()
	{
		this->seqCapacity_ = this->dims_[0];
		for (int i = 1; i < this->dSize(); i++)
		{
			this->seqCapacity_ *= this->dims_[i];
		}
		return this->seqCapacity_;
	}

	void setFrontEnd()
	{
		if (this->coor_ == this->sP_)
		{
			this->front_ = true;
		} else
		{
			this->front_ = false;
		}
		if (this->coor_ == this->eP_)
		{
			this->end_ = true;
		} else
		{
			this->end_ = false;
		}
	}

	// call this function before you change a basisDim
	size_t getDimOffset(const unsigned int dim)
	{
		if (dim == this->basisDim_)
		{
			return this->basisDimOffset_;
		}

		size_t offset = 1;
		for (size_type i = this->dSize() - 1; i > dim; i--)
		{
			offset *= this->dims_[i];
		}

		//std::cout << "dim : " << dim << ", getOffset: " << offset << std::endl;
		return offset;
	}

protected:
	coordinate_type coor_;		// coordinate
	dimensionId basisDim_;		// current dimension

	dim_pointer dims_;			// original array dimension size
	dim_pointer sP_;			// range start point
	dim_pointer eP_;			// range end point
	size_type dSize_;			// number of dimensions

	bool end_;					// is iterator at the end
	bool front_;				// is iterator at the front

	size_type seqPos_;
	size_type seqCapacity_;
	size_type basisDimOffset_;

	// TODO:: replace sP_ to zero
	// static std::vector<dim_type> zeroP_;
};

template <typename Dty_>
class itemIterator : virtual public coordinateIterator<Dty_>
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
		: base_type(dSize, dims), eType_(eType), eSize_(getEleSize(eType))
	{
		this->ptr_ = reinterpret_cast<char*>(ptr);
	}

	itemIterator(const self_type& mit)
		: base_type(mit), ptr_(mit.ptr_), eType_(mit.eType_), eSize_(mit.eSize_)
	{

	}

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	element getAt(position_t pos)
	{
		return element((void*)(ptr_ + (this->seqPos_ + this->posToSeq(pos)) * this->eSize_), this->eType_);
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
	// Comparison
	bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_ && this->seqPos_ == rhs.seqPos_; }
	bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_ || this->seqPos_ != rhs.seqPos_; }

	// Pointer
	element operator*() { return element((void*)(ptr_ + this->seqPos_ * this->eSize_), this->eType_); }
	//element operator->() { return element((void*)(ptr_ + this->seqPos_ * this->eSize_), this->eType_); }
	//////////////////////////////

protected:
	char* ptr_;					// pointer to element
								// set pointer type as char, to move one byte for each seqPos_
	eleType eType_;				// element type
	size_type eSize_;			// element size in byte
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
		: base_type(ptr, eType, dSize, dim), coordinateIterator<Dty_>(dSize, dim)
	{
		this->eP_ = new dim_type[dSize];
		this->memcpyDim(this->sP_, sP);
		this->memcpyDim(this->eP_, eP);

		assert(this->initCheckSpEp());

		this->moveToStart();
	}

	itemRangeIterator(const self_type& mit) : base_type(mit), coordinateIterator<Dty_>(mit)
	{
		this->eP_ = new dim_type[mit.dSize()];
		this->memcpyDim(this->eP_, mit.eP_);
		//memcpy(this->eP_, mit.eP_, mit.dSize() * sizeof(dim_type));
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
class coorIterator : public coordinateIterator<Dty_>
{
public:
	using self_type = coorIterator<Dty_, Ty_>;
	using base_type = coordinateIterator<Dty_>;

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
	coorIterator(value_pointer ptr, const size_type dSize, dim_const_pointer dims)
		: base_type(dSize, dims), ptr_(ptr)
	{

	}

	coorIterator(const self_type& mit) : base_type(mit), ptr_(mit.ptr_)
	{

	}

	~coorIterator()
	{

	}

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	Ty_ getAt(position_t pos)
	{
		return ptr_[this->seqPos_ + this->posToSeq(pos)];
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
	// Comparison
	bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_ && this->seqPos_ == rhs.seqPos_; }
	bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_ || this->seqPos_ != rhs.seqPos_; }

	// Pointer
	Ty_ operator*() { return ptr_[this->seqPos_]; }
	Ty_ operator->() { return ptr_[this->seqPos_]; }

	// Move
	self_type& operator++()
	{
		base_type::operator++();
		return *this;
	}
	self_type operator++(int)
	{
		self_type tmp(*this);
		operator++();
		return tmp;
	}
	self_type& operator--()
	{
		base_type::operator--();
		return *this;
	}
	self_type operator--(int)
	{
		self_type tmp(*this);
		operator--();
		return tmp;
	}

	// random access
	_NODISCARD value_reference operator[](size_type pos)
	{
		// TODO:: out of range check
		return ptr_[this->seqPos_ + this->posToSeq(pos)];
	}
	_NODISCARD const value_reference operator[](size_type pos) const
	{
		// TODO:: out of range check
		return ptr_[this->seqPos_ + this->posToSeq(pos)];
	}
	//////////////////////////////

protected:
	Ty_* ptr_;				// pointer to element
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
		this->memcpyDim(this->sP_, sP);
		this->memcpyDim(this->eP_, eP);

		this->moveToStart();
	}

	coorRangeIterator(const coorRangeIterator& mit) : coorIterator<Dty_, Ty_>(mit)
	{
		this->eP_ = new Dty_[mit.dSize_];
		this->memcpyDim(this->eP_, mit.eP_);
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
using coorItr = coordinateIterator<position_t>;
using itemItr = itemIterator<position_t>;
using itemRangeItr = itemRangeIterator<position_t>;
}

#endif