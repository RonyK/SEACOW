#pragma once
#ifndef _MSDB_MMTNODE_H_
#define _MSDB_MMTNODE_H_

#include <stdafx.h>
#include <index/attributeIndex.h>
#include <util/math.h>
#include <util/logger.h>
#include <io/bitstream.h>

namespace msdb
{

class mmtNode;
using pMmtNode = std::shared_ptr<mmtNode>;

class mmtNode : public attributeIndex
{
public:
	//using TyType = Ty_;

	sig_bit_type bMax_;			// significant nit of max value
	sig_bit_type bMin_;			// significant nit of min value
	bit_cnt_type bMaxDelta_;	// bMax_ delta from a parent node
	bit_cnt_type bMinDelta_;	// bMin_ delta from a parent node
	bit_cnt_type order_;		// n th significant nit
	bit_cnt_type bits_;			// required bits to represent min/max value
	bit_cnt_type vBits_;		// required bits to represent value Ty_
	boost::any max_;
	boost::any min_;
	boost::any realMax_;
	boost::any realMin_;

public:
	mmtNode() : bMax_(0), bMin_(0), bits_(0x80), order_(1),
		bMaxDelta_(0), bMinDelta_(0), max_(0), min_(0), 
		vBits_(0), realMin_(0), realMax_(0)
	{
	}

public:
	// use function cause of unknown compile error
	inline void inDelta(bstream& bs)
	{
		bs >> setw(this->bits_) >> this->bMaxDelta_ >> this->bMinDelta_;
	}

	inline void outDelta(bstream& bs)
	{
		bs << setw(this->bits_) << this->bMaxDelta_ << this->bMinDelta_;
	}

	template <typename Ty_>
	inline void inMinMax(bstream& bs)
	{
		Ty_ min, max;
		bs >> setw(sizeof(Ty_) * CHAR_BIT) >> min >> max;
		this->min_ = min;
		this->max_ = max;
		this->realMin_ = min;
		this->realMax_ = max;
	}

	template <typename Ty_>
	inline void outMinMax(bstream& bs)
	{
		bs << setw(sizeof(Ty_) * CHAR_BIT);
		bs << this->getMax<Ty_>() << this->getMin<Ty_>();
	}

	template <typename Ty_>
	inline void initBits()
	{
		this->bits_ = sizeof(Ty_) * CHAR_BIT;
		this->order_ = 1;
		this->setMinMaxBits<Ty_>();
		this->vBits_ = msb<Ty_>(this->getMax<Ty_>() - this->getMin<Ty_>()) + 1;
	}

	template <typename Ty_>
	inline void setMinMaxBits()
	{
		this->bMax_ = msb<Ty_>(abs_(boost::any_cast<Ty_>(max_)), this->order_) * SIGN(boost::any_cast<Ty_>(max_));
		this->bMin_ = msb<Ty_>(abs_(boost::any_cast<Ty_>(min_)), this->order_) * SIGN(boost::any_cast<Ty_>(min_));
	}

	template <typename Ty_>
	inline Ty_ getMax()
	{
		return boost::any_cast<Ty_>(this->max_);
	}

	template <typename Ty_>
	inline Ty_ getMin()
	{
		return boost::any_cast<Ty_>(this->min_);
	}

	template <typename Ty_>
	inline Ty_ getRealMax()
	{
		return boost::any_cast<Ty_>(this->realMax_);
	}

	template <typename Ty_>
	inline Ty_ getRealMin()
	{
		return boost::any_cast<Ty_>(this->realMin_);
	}

	template <typename Ty_>
	inline void print()
	{
		BOOST_LOG_TRIVIAL(info) << "--------------------";
		BOOST_LOG_TRIVIAL(info) << static_cast<int64_t>(this->getMin<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMin_) << ", " << static_cast<int64_t>(this->bMinDelta_) << ")~"
			<< static_cast<int64_t>(this->getMax<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMax_) << ", " << static_cast<int64_t>(this->bMaxDelta_)
			<< ") / b: " << static_cast<int64_t>(this->bits_)
			<< " / v: " << static_cast<int64_t>(this->vBits_)
			<< " / or: " << static_cast<int64_t>(this->order_)
			<< "/ real: " << static_cast<int64_t>(this->getRealMin<Ty_>()) << "~" << static_cast<int64_t>(this->getRealMax<Ty_>());
	}
};

}		// msdb
#endif	// _MSDB_MMTNODE_H_
