#pragma once
#ifndef _MSDB_MMTNODE_H_
#define _MSDB_MMTNODE_H_

#include <stdafx.h>
#include <index/attributeIndex.h>
#include <util/math.h>
#include <util/logger.h>
#include <io/bitstream.h>
#include <array/dimension.h>

namespace msdb
{

class mmtNode;
using pMmtNode = std::shared_ptr<mmtNode>;

class mmtNode : public attributeIndex
{
public:
	//using TyType = Ty_;

	sig_bit_type bMax_;			// nth significant of max value
	sig_bit_type bMin_;			// nth significant of min value
	bit_cnt_type bMaxDelta_;	// bMax_ delta from a parent node
	bit_cnt_type bMinDelta_;	// bMin_ delta from a parent node
	bit_cnt_type order_;		// n
	bit_cnt_type childOrder_;	// order for child node
	bit_cnt_type bits_;			// required bits to represent min/max value
	boost::any max_;
	boost::any min_;
	boost::any realMax_;
	boost::any realMin_;
	bool finished_;

	// For Test
	dimension chunkCoor_;
	dimension blockCoor_;
	dimension nodeCoor_;
	size_t seqPos_;
	pMmtNode parent_;

public:
	mmtNode() : bMax_(0), bMin_(0), bits_(0x80), order_(1),
		bMaxDelta_(0), bMinDelta_(0), max_(0), min_(0),
		realMin_(0), realMax_(0), finished_(false),
		chunkCoor_(1), blockCoor_(1), nodeCoor_(1), seqPos_(0)
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

	inline void inChildOrderChanged(bstream& bs)
	{
		bit_cnt_type orderChangedDelta = 0;
		bs >> setw(this->bits_) >> orderChangedDelta;
		this->childOrder_ = this->order_ + orderChangedDelta;
	}

	inline void outChildOrderChanged(bstream& bs)
	{
		if(this->childOrder_ > this->order_)
		{
			// next order
			bs << setw(this->bits_) << this->childOrder_ - this->order_;
		}else
		{
			// cur node is last, end
			bs << setw(this->bits_) << 0x1;
		}
	}

	template <typename Ty_>
	inline void inJumpedBits(bstream& bs, bit_cnt_type jumpBits, Ty_ jumpValue)
	{
		jumpBits = 0;
		jumpValue = 0;
		bit_cnt_type orderDelta = this->childOrder_ - this->order_;
		char inBit = 0;

		// make jumpBits
		while(orderDelta)
		{
			++jumpBits;
			inBit = 0;
			bs >> setw(1) >> inBit;
			jumpValue << 1;
			jumpValue |= inBit;

			if(inBit)
			{
				--orderDelta;
			}
		}

		// erase last significant bit
		jumpValue >>= 1;
		--jumpBits;;

		// make to original value
		assert(this->bMax_ - jumpBits - 1 > 0);
		jumpValue <<= sizeof(Ty_) * CHAR_BIT - this->bMax_ - 1;
	}

	template <typename Ty_>
	inline void outJumpedBits(bstream& bs, bit_cnt_type posJumpEnd)
	{
		// make mask
		Ty_ mask = (Ty_)-1;
		mask >>= sizeof(Ty_) * CHAR_BIT - this->bMax_;

		if(posJumpEnd)
		{
			// next order
			Ty_ jumpValue = (this->getRealMax<Ty_>() >> (posJumpEnd - 1)) & mask;
			bs << setw(this->bMax_ - posJumpEnd + 1) << jumpValue;
		}else
		{
			// cur node is last, end
			Ty_ jumpValue = this->getRealMax<Ty_>() & mask;
			bs << setw(this->bMax_ - posJumpEnd + 1) << jumpValue;
			bs << setw(1) << 0x1;
		}
	}

	template <typename Ty_>
	inline void inMinMax(bstream& bs)
	{
		Ty_ min, max;
		bs >> setw(this->bits_) >> min >> max;
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
	}

	template <typename Ty_>
	inline void setMinMaxBits()
	{
		this->bMax_ = msb<Ty_>(abs_(boost::any_cast<Ty_>(max_)), this->order_) * SIGN(boost::any_cast<Ty_>(max_));
		this->bMin_ = msb<Ty_>(abs_(boost::any_cast<Ty_>(min_)), this->order_) * SIGN(boost::any_cast<Ty_>(min_));
		
#ifndef NDEBUG
		if(this->bMax_ < 0 || this->bMin_ < 0)
		{
			BOOST_LOG_TRIVIAL(warning) << "bMax: " << this->bMax_ << ", bMin: " << this->bMin_;
		}
#endif
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
		BOOST_LOG_TRIVIAL(debug) << static_cast<int64_t>(this->getMin<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMin_) << ", " << static_cast<int64_t>(this->bMinDelta_) << ")~"
			<< static_cast<int64_t>(this->getMax<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMax_) << ", " << static_cast<int64_t>(this->bMaxDelta_) << ")"
			<< " / b: " << static_cast<int64_t>(this->bits_)
			<< " / bTy: " << static_cast<int64_t>(sizeof(Ty_) * CHAR_BIT)
			<< " / or: " << static_cast<int64_t>(this->order_)
			<< " / real: " << static_cast<int64_t>(this->getRealMin<Ty_>()) << "~" << static_cast<int64_t>(this->getRealMax<Ty_>());
	}

	template <typename Ty_>
	inline std::string toString()
	{
		std::stringstream ss;

		ss << static_cast<int64_t>(this->getMin<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMin_) << ", " << static_cast<int64_t>(this->bMinDelta_) << ")~"
			<< static_cast<int64_t>(this->getMax<Ty_>())
			<< "(" << static_cast<int64_t>(this->bMax_) << ", " << static_cast<int64_t>(this->bMaxDelta_) << ")"
			<< " / b: " << static_cast<int64_t>(this->bits_)
			<< " / bTy: " << static_cast<int64_t>(sizeof(Ty_) * CHAR_BIT)
			<< " / or: " << static_cast<int64_t>(this->order_)
			<< " / real: " << static_cast<int64_t>(this->getRealMin<Ty_>()) << "~" << static_cast<int64_t>(this->getRealMax<Ty_>()) << "\n"
			<< this->chunkCoor_.toString() << " / " << this->blockCoor_.toString() << " / " << this->nodeCoor_.toString();

		return ss.str();
	}
};

}		// msdb
#endif	// _MSDB_MMTNODE_H_
