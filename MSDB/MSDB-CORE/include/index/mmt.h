#pragma once
#ifndef _MSDB_MMT_H_
#define _MSDB_MMT_H_

#include <index/attributeIndex.h>
#include <io/bitstream.h>
#include <util/coordinate.h>
#include <util/math.h>
#include <memory>

namespace msdb
{
template<typename Dty_>
class mmt;
using pMMT = std::shared_ptr<mmt<position_t>>;

template <typename Ty_>
bit_cnt_type getPrefixPosForPrevLimit(Ty_, bit_cnt_type);

template <typename Dty_>
class mmt
{
public:
	using size_type = size_t;
	using size_const = const size_t;

	using dim_type = Dty_;
	using dim_pointer = Dty_*;
	using dim_const_pointer = const Dty_*;
	using dim_reference = Dty_&;
	using dim_const_reference = const Dty_;

	using dim_vector = std::vector<Dty_>;
	using dim_vector_pointer = std::vector<Dty_>*;
	using dim_vector_reference = std::vector<Dty_>&;
	using dim_vector_const_pointer = const std::vector<Dty_>*;
	using dim_vector_const_reference = std::vector<Dty_>&;

	//////////////////////////////
	// MMT NODE					//
	//////////////////////////////
	class mmtNode;
	using pNode = std::shared_ptr<mmtNode>;

	class mmtNode : public attributeIndex
	{
	public:
		sig_bit_type bMax_;			// significant bit of max value
		sig_bit_type bMin_;			// significant bit of min value
		bit_cnt_type bMaxDelta_;		// bMax delta from a parent node
		bit_cnt_type bMinDelta_;		// bMin delta from a parent node
		bit_cnt_type order_;			// n th significant bit
		bit_cnt_type bits_;			// required bits to represent min/max value

	public:
		mmtNode() : bMax_(0), bMin_(0), bits_(0x80), order_(1), bMaxDelta_(0), bMinDelta_(0) {}

		inline void copyMaxFrom(mmtNode* _right)
		{
			this->max = _right->max;
			this->bMax_ = _right->bMax_;
		}
		inline void copyMinFrom(mmtNode* _right)
		{
			this->min = _right->min;
			this->bMin_ = _right->bMin_;
		}

		void setMax(const void* max) = 0;
		void setMax(element& max) = 0;
		void setMin(const void* min) = 0;
		void setMin(element& min) = 0;
		void* getMax() = 0;
		void* getMin() = 0;

		void setMaxBoundary(sig_bit_type bMax) = 0;
		void setMinBoundary(sig_bit_type bMin) = 0;

		void initBits() = 0;
		void initBMax() = 0;
		void initBMin() = 0;

		void setMinMaxFromBitstream(bstream& bs) = 0;

		char compareMax(pNode right) = 0;
		char compareMin(pNode right) = 0;
	};

	template<typename Ty_>
	class mmtNodeImpl : public mmtNode
	{
	public:
		Ty_ max_;
		Ty_ min_;

	public:
		mmtNodeImpl() : mmtNode(), max_(0), min_(0) {}

	public:
		void* getMax()
		{
			return &this->max_;
		}

		void* getMin()
		{
			return &this->min_;
		}

		void setMax(const void* max)
		{
			this->max_ = *reinterpret_cast<Ty_*>(max);
			this->bMax = msb<Ty_>(std::abs(max)) * SIGN(max);
		}

		void setMax(element& max)
		{
			max.getData(&this->max_);
			this->bMax = msdb<Ty_>(std::abs(this->max_)) * SIGN(max_);
		}

		void setMin(const void* min)
		{
			this->min_ = *reinterpret_cast<Ty_*>(min);
			this->bMin = msb<Ty_>(std::abs(min)) * SIGN(min);
		}

		void setMin(element& min)
		{
			min.getData(&this->min_);
			this->bMin = msb<Ty_>(std::abs(this->min_)) * SIGN(min_);
		}

		void setMaxBoundary(sig_bit_type bMax)
		{
			this->max_ = getMaxBoundary<Ty_>(bMax);
		}
		void setMinBoundary(sig_bit_type bMin)
		{
			this->min_ = getMaxBoundary<Ty_>(bMin);
		}

		void setMinMaxFromBitstream(bstream& bs)
		{
			bs >> setw(TyBits_) >> this->max_ >> this->min_;
			this->bits = msb<Ty_>(max_ - min_);
		}

		void initBits()
		{
			this->bits = msb<Ty_>(max_ - min_);
		}
		void initBMax()
		{
			this->bMax = msb<Ty_>(std::abs(this->max_), this->order) * SIGN(this->max_);
		}
		void initBMin()
		{
			this->bMin = msb<Ty_>(std::abs(this->min_), this->order) * SIGN(this->min_);
		}

		char compareMax(pNode right)
		{
			Ty_ rightMax = *((Ty_*)right->getMax());
			if (this->max_ < rightMax)	return -1;
			if (this->max_ > rightMax) return 1;
			return 0;
		}
		char compareMin(pNode right)
		{
			Ty_ rightMax = *((Ty_*)right->getMax());
			if (this->min_ < rightMax)	return -1;
			if (this->min_ > rightMax) return 1;
			return 0;
		}
	};

public:
	pNode(mmt::* makeNodeFunc)();

	template<typename Ty_>
	pNode makeNodeImpl()
	{
		return std::make_shared<mmtNodeImpl<Ty_>>();
	}

	pNode makeNode()
	{
		return (this->*makeNodeFunc)();
	}

public:
	mmt(eleType eType, dim_vector_reference dim, dim_vector_reference chunkDim, size_const maxLevel)
		: dim_(dim), dSize_(dim.size()), leafChunkDim_(chunkDim), maxLevel_(maxLevel), eType_(eType_), TySize_(getEleSize(eType)), TyBits_(getEleSize(eType)* CHAR_BIT)
	{
		this->initChunkInDim(dim, chunkDim, maxLevel);
		this->setMakeNodeFunc(eType);
	}

	~mmt() {}

public:
	void build(const void* data, size_const length)
	{
		//////////////////////////////
		// Forward build
		// Set Min/Max values [0 (leaf) level -> maxLevel]
		this->forwardBuildLeaf(data, length);
		for (size_type level = 1; level <= this->maxLevel_; level++)
		{
			this->forwardBuildNonLeaf(level);
		}
		//////////////////////////////

		//////////////////////////////
		// Backward Build
		// Set bits [maxLevel -> 0 level]
		this->backwardBuildRoot();
		for (size_type level = this->maxLevel_ - 1; level != (size_type)-1; level--)
		{
			this->backwardBuildNonRoot(level);
		}
		//////////////////////////////
	}

	void serialize(void* output, size_const length)
	{
		bstream bs;
		this->serialize(bs);

		if (length < (bs.size() + CHAR_BIT - 1) / CHAR_BIT)
		{
			throw std::length_error();
		}

		memcpy(output, bs.data(), (bs.size() + CHAR_BIT - 1) / CHAR_BIT);
	}

	void serialize(bstream& bs)
	{
		size_type maxLevel = this->nodes_.size() - 1;

		if (maxLevel == -1)
		{
			return;		// Empty
		}

		this->serializeRoot(bs);

		for (size_type l = maxLevel - 1; l != (size_type)-1; l--)
		{
			this->serializeNonRoot(bs, l);
		}
	}

	// build MMT from bstream
	void deserialize(bstream& bs)
	{
		this->deserializeRoot(bs);

		for (size_type l = this->maxLevel_ - 1; l != (size_type)-1; l--)
		{
			this->deserializeNonRoot(bs, l);
		}
	}

	const pNode getNodes(size_type level)
	{
		assert(level < this->nodes_.size());
		return this->nodes_[level].data();
	}

	const dim_vector& getChunkInDim(size_type level)
	{
		assert(level <= this->chunksInDim_.size());
		return this->chunksInDim_[level];
	}

protected:
	//////////////////////////////
	// MMT Build Functions
	// For level 0
	void forwardBuildLeaf(itemItr& it, size_const length)
	{
		////////////////////////////////////////
		// Create new mmtNodes
		auto chunksInDim = this->chunksInDim_[0];
		size_type chunkCnt = calcArrayCellNums(chunksInDim.data(), chunksInDim.size());
		this->nodes_.push_back(std::vector<pNode>(chunkCnt));

		//coorIterator<Dty_, Ty_> it(data, this->dSize_, this->dim_.data());
		coorIterator<Dty_, pNode> cit(this->nodes_[0].data(), this->dSize_, chunksInDim.data());

		for (size_type i = 0; i < length; i++)
		{
			// current iterator coordiate -> chunk coordinate
			coordinate<Dty_> cur = it;
			for (size_type d = 0; d < this->dSize_; d++)
			{
				cur[d] /= this->leafChunkDim_[d];
			}

			// get target chunk
			cit.moveTo(cur);
			pNode node = (*cit);

			// init min, max value
			if (node->bits == 0x80)
			{
				node->setMax((*it));
				node->setMin((*it));
				//node->setMax(&(*it));
				//node->setMin(&(*it));
				node->bits = (bit_cnt_type)TyBits_;
			} else
			{
				// compare min max value
				if (*it > node->max)
				{
					node->setMax((*it));
					//node->setMax(&(*it));
				}
				if (*it < node->min)
				{
					node->setMin((*it));
					//node->setMin(&(*it));
				}
			}

			// move to next value
			++it;
		}
	}

	// For level 1 ~ maxLevel
	// Notes: In this method, prev is a lower level nodes (finer)
	void forwardBuildNonLeaf(const size_type level)
	{
		assert(level > 0);

		////////////////////////////////////////
		// Create new mmtNodes
		dim_vector pChunksInDim = this->chunksInDim_[level - 1];
		dim_vector chunksInDim = this->chunksInDim_[level];
		const size_type chunkCnt = calcArrayCellNums(chunksInDim.data(), chunksInDim.size());
		this->nodes_.push_back(std::vector<pNode>(chunkCnt));

		////////////////////////////////////////
		// Update min/max values
		coorIterator<Dty_, pNode> pcit(this->nodes_[level - 1].data(), this->dSize_,
									   pChunksInDim.data());
		coorIterator<Dty_, pNode> cit(this->nodes_[level].data(), this->dSize_,
									  chunksInDim.data());

		for (size_type i = 0; i < this->nodes_[level - 1].size(); i++)
		{
			// current iterator coordiate -> parent coordinate
			coordinate<Dty_> cur = pcit;
			for (size_type d = 0; d < this->dSize_; d++)
			{
				cur[d] /= 2;
			}

			// get target chunk
			cit.moveTo(cur);
			pNode node = makeNode();
			(*cit) = node;

			// init min, max value
			if (node->bits == 0x80)
			{
				node->copyMaxFrom((*pcit));
				node->copyMinFrom((*pcit));
				node->bits = (bit_cnt_type)TyBits_;
			} else
			{
				// compare min max value
				if ((*pcit).max > node->max)
				{
					node->copyMaxFrom((*pcit));
				}
				if ((*pcit).min < node->min)
				{
					node->copyMinFrom((*pcit));
				}
			}

			// move to next chunk
			++pcit;
		}
	}

	// For max level nodes
	void backwardBuildRoot()
	{
		pNode* curLevel = this->nodes_.back().data();

		for (size_type i = 0; i < this->nodes_.back().size(); i++)
		{
			curLevel[i]->initBits();
			//curLevel[i].bits = msb<Ty_>(curLevel[i].max - curLevel[i].min);
		}
	}

	// Except max level nodes ((maxLevel - 1) -> level 0 )
	void backwardBuildNonRoot(const size_type level)
	{
		assert(level < this->nodes_.size() - 1);		// Not for max level nodes
		assert(level != -1);

		size_type pLevel = level + 1;
		size_type siblings = (size_type)pow(2, this->dim_.size());

		////////////////////////////////////////
		// Calc chunk num of prev and current level
		//
		// Notes: In this method, prev is a upper level nodes (coarse)
		dim_vector pChunksInDim = this->chunksInDim_[level + 1];
		dim_vector chunksInDim = this->chunksInDim_[level];

		////////////////////////////////////////
		// Update bit order for chunks in current level
		// Prev
		coorIterator<Dty_, pNode> pcit(this->nodes_[level + 1].data(), this->dSize_,
									   pChunksInDim.data());

		// Current
		coorIterator<Dty_, pNode> cit(this->nodes_[level].data(), this->dSize_,
									  chunksInDim.data());

		for (size_type i = 0; i < this->nodes_[level + 1].size(); ++i, ++pcit)
		{
			coordinate<Dty_> childBase = pcit;
			for (size_type d = 0; d < this->dSize_; d++)
			{
				childBase[d] *= 2;
			}

			pNode prevNode = (*pcit);
			bool sameOrder = (bool)(prevNode->bMax - prevNode->bMin);	// if bMax == bMin, move on to the next most significant bit
			bit_cnt_type bits = sameOrder ? msb<sig_bit_type>(prevNode->bMax - prevNode->bMin) : msb<sig_bit_type>(std::abs(prevNode->bMax) - 1);

			for (size_type cID = 0; cID < siblings; cID++)
			{
				// Set child coordinate and move to it
				coordinate<Dty_> cur = childBase;
				for (size_type d = 0; d < this->dSize_; d++)
				{
					if (cID & ((size_type)0x1 << d))
					{
						cur[d] += 1;
					}
				}
				cit.moveTo(cur);

				// Update
				pNode cNode = (*cit);
				if (sameOrder)
				{
					cNode->bits = bits;
					cNode->order = prevNode->order;

					cNode->bMaxDelta = static_cast<bit_cnt_type>(prevNode->bMax - cNode->bMax);	// max: prev >= cur
					cNode->bMinDelta = static_cast<bit_cnt_type>(cNode->bMin - prevNode->bMin);	// min: prev <= cur
				} else
				{
					if ((bit_cnt_type)prevNode->order + 1 < TyBits_)
					{
						// Move to next significant bit
						cNode->bits = bits;
						cNode->order = prevNode->order + 1;

						cNode->initBMax();
						cNode->initBMin();
						//cNode->bMax = msb<Ty_>(std::abs(cNode->max), cNode->order) * SIGN(cNode->max);
						//cNode->bMin = msb<Ty_>(std::abs(cNode->min), cNode->order) * SIGN(cNode->min);

						cNode->bMaxDelta = std::max(std::abs(prevNode->bMax - cNode->bMax) - 1, 0);
						cNode->bMinDelta = std::max(std::abs(cNode->bMin - prevNode->bMin) - 1, 0);
					} else
					{
						// The last bit. Has no more next significant bit
						cNode->bits = 0;
						cNode->order = prevNode->order;
					}
				}
			}
		}
	}

	////////////////////////////////////////
	// Write bits in an outstream
	void serializeRoot(bstream& bs)
	{
		pNode* curLevelNodes = this->nodes_.back().data();

		for (size_type i = 0; i < this->nodes_.back().size(); i++)
		{
			bs << setw(TyBits_) << curLevelNodes[i].max << curLevelNodes[i].min;
		}
	}

	void serializeNonRoot(bstream& bs, size_type level)
	{
		pNode* cNodes = this->nodes_[level].data();
		for (size_type i = 0; i < this->nodes_[level].size(); i++)
		{
			bs << setw(cNodes[i].bits) << cNodes[i].bMaxDelta << cNodes[i].bMinDelta;
		}
	}

	////////////////////////////////////////
	// Build MMT from an instream
	void deserializeRoot(bstream& bs)
	{
		////////////////////////////////////////
		// Re-generate nodes
		// TODO:: Seperate this block to distict function.
		this->nodes_.clear();
		for (size_type l = 0; l < this->maxLevel_ + 1; l++)
		{
			// TODO:: Calc chunkCnt on each level
			this->nodes_.push_back(std::vector<pNode>());
		}

		////////////////////////////////////////
		// Create new mmtNodes
		auto chunksInDim = this->chunksInDim_[this->maxLevel_];
		size_type chunkCnt = calcArrayCellNums(chunksInDim.data(), chunksInDim.size());
		this->nodes_[this->maxLevel_].resize(chunkCnt);	// TODO::If generating Nodes are complete, remove this line.
		pNode* rootNodes = this->nodes_[this->maxLevel_].data();

		for (size_type i = 0; i < chunkCnt; i++)
		{
			rootNodes[i] = makeNode();
			rootNodes[i]->setMinMaxFromBitstream(bs);

			//Ty_ max, min;
			//bs >> setw(TyBits_) >> max >> min;
			//rootNodes[i].setMax(max);
			//rootNodes[i].setMin(min);
			//rootNodes[i].bits = msb<Ty_>(max - min);
		}
	}

	void deserializeNonRoot(bstream& bs, size_type level)
	{
		// Calc chunk num of prev and current level
		dim_vector pChunksInDim = this->chunksInDim_[level + 1];
		dim_vector chunksInDim = this->chunksInDim_[level];
		size_type chunkCnt = calcArrayCellNums(chunksInDim.data(), chunksInDim.size());
		this->nodes_[level].resize(chunkCnt);

		// Prev
		coorIterator<Dty_, pNode> pcit(this->nodes_[level + 1].data(), this->dSize_,
									   pChunksInDim.data());

		// Current
		coorIterator<Dty_, pNode> cit(this->nodes_[level].data(), this->dSize_,
									  chunksInDim.data());

		for (size_type i = 0; i < chunkCnt; i++)
		{
			pNode cNode = (*cit);
			pcit.moveTo(this->getParentCoor(cit));
			pNode prevNode = (*pcit);

			bool sameOrder = prevNode->bMax != prevNode->bMin;	// if bMax == bMin, move on to the next most significant bit
			cNode->bits = sameOrder ? msb<sig_bit_type>(prevNode->bMax - prevNode->bMin) : msb<sig_bit_type>(std::abs(prevNode->bMax) - 1);

			bs >> setw(cNode->bits) >> cNode->bMaxDelta >> cNode->bMinDelta;

			if (sameOrder)
			{
				cNode->order = prevNode->order;
				cNode->bMax = prevNode->bMax - cNode->bMaxDelta;
				cNode->bMin = prevNode->bMin + cNode->bMinDelta;

				if (cNode->order == 1)
				{
					cNode->setMaxBoundary(cNode->bMax);
					cNode->setMinBoundary(cNode->bMin);
					//cNode->max = getMaxBoundary<Ty_>(cNode->bMax);
					//cNode->min = getMinBoundary<Ty_>(cNode->bMin);
				} else
				{
					cNode->max = getMaxBoundary(prevNode->max, cNode->order, cNode->bMax);
					cNode->min = getMinBoundary(prevNode->min, cNode->order, cNode->bMin);
				}
			} else
			{
				if ((size_type)prevNode->order + 1 < TyBits_)
				{
					// Move to next significant bit
					cNode->order = prevNode->order + 1;
					cNode->bMax = prevNode->bMax - cNode->bMaxDelta - 1;
					cNode->bMin = prevNode->bMin - cNode->bMinDelta - 1;

					cNode->max = getMaxBoundary(prevNode->max, cNode->order, cNode->bMax);
					cNode->min = getMinBoundary(prevNode->min, cNode->order, cNode->bMin);
				} else
				{
					cNode->bits = 0;
					cNode->order = prevNode->order;
					cNode->copyMaxFrom(prevNode);
					cNode->copyMinFrom(prevNode);
				}
			}

			// Fill out min, max values.
			if (cNode->order == 1)
			{
				cNode->setMaxBoundary(cNode->bMax);
				cNode->setMinBoundary(cNode->bMin);
				//cNode->max = getMaxBoundary<Ty_>(cNode->bMax);
				//cNode->min = getMinBoundary<Ty_>(cNode->bMin);
			} else
			{
				if (cNode->bits)
				{
					cNode->max = getMaxBoundary(prevNode->max, cNode->order, cNode->bMax);
					cNode->min = getMinBoundary(prevNode->min, cNode->order, cNode->bMin);
				}
			}

			cit++;	// Move to next data
		}
	}

protected:
	//////////////////////////////
	// UTILS					//
	//////////////////////////////
	// Calculate number of chunk on each dimension using the dimension sizes and chunk sizes
	void initChunkInDim(dim_vector_const_reference dims, dim_vector_const_reference chunkDim, size_type maxLevel)
	{
		for (size_type level = 0; level <= maxLevel; level++)
		{
			this->chunksInDim_.push_back(calcChunkNums(dims.data(), chunkDim.data(), dims.size(), level));
		}
	}

	_NODISCARD coordinate<Dty_> getChildBaseCoor(coordinate<Dty_>& it)
	{
		coordinate<Dty_> childBase = it;
		for (size_type d = 0; d < this->dSize_; d++)
		{
			childBase[d] *= 2;
		}
		return childBase;
	}

	_NODISCARD coordinate<Dty_> getParentCoor(coordinate<Dty_>& it)
	{
		coordinate<Dty_> coorParent = it;
		for (size_type d = 0; d < this->dSize_; d++)
		{
			coorParent[d] /= 2;
		}
		return coorParent;
	}

private:
	void setMakeNodeFunc(eleType eType)
	{
		switch (eType)
		{
		case eleType::BOOL:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<bool>;
			break;
		case eleType::CHAR:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<char>;
			break;
		case eleType::INT8:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<int8_t>;
			break;
		case eleType::INT16:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<int16_t>;
			break;
		case eleType::INT32:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<int32_t>;
			break;
		case eleType::INT64:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<int64_t>;
			break;
		case eleType::UINT8:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<uint8_t>;
			break;
		case eleType::UINT16:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<uint16_t>;
			break;
		case eleType::UINT32:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<uint32_t>;
			break;
		case eleType::UINT64:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<uint64_t>;
			break;
		case eleType::FLOAT:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<float>;
			break;
		case eleType::DOUBLE:
			this->makeNodeFunc = mmt<Dty_>::makeNodeImpl<double>;
			break;
		}
	}

private:
	const size_type TySize_;
	const size_type TyBits_;
	eleType eType_;

private:
	size_type dSize_;
	size_type maxLevel_;
	dim_vector dim_;		// dimensions
	dim_vector leafChunkDim_;
	std::vector<dim_vector> chunksInDim_;
	std::vector<std::vector<pNode>> nodes_;	// mmt
	/*
	* Here is an example of a 'nodes_' with size 4 (has 0~3 levels).
	*
	*  Leaf     Root
	*   ▼        ▼
	* ┌──┬──┬──┬──┐
	* │ 0│ 1│ 2│ 3│ Level
	* └──┴──┴──┴──┘
	*
	*  NOTE::Root nodes are in 'MaxLevel'.
	*  MMT is serialized from 'MaxLevel' -> '0 Level'.
	*
	*/
};

class mmtHolder
{
public:

	mmtHolder(pMMT inMMT);

protected:
	pMMT mmt_;
};

class inMmtHolder : public mmtHolder
{

};

class outMmtHolder : public mmtHolder
{

};

class serializedMMTHolder
{
public:
	serializedMMTHolder(pBstream bs);
	serializedMMTHolder(void* data, size_t length);

protected:
	pBstream bs_;
};

template <typename Ty_>
bit_cnt_type getPrefixPosForPrevLimit(Ty_ prevLimit, bit_cnt_type order)
{
	/*
	*  Here is an example:
	*
	*  Suppose prevLimit (1001 1111), order (3), significantBit (3)
	*  ┌──┬──┬──┬──┬──┬──┬──┬──┐
	*  │ 1│ 0│ 0│ 1│ 1│ 1│ 1│ 1│
	*  └──┴──┴──┴──┴──┴──┴──┴──┘
	*               ▲      ▲
	*               pL     sB
	*  ==================================
	*  Result:
	*  ┌──┬──┬──┬──┬──┬──┬──┬──┐
	*  │ 1│ 0│ 0│ 1│ 0│ 0│ 1│ 1│
	*  ├──┴──┴──┴──┼──┴──┼──┴──┤
	*  │prefix_copy│ 0's │ 1's │
	*  └───────────┴─────┴─────┘
	*               ▲      ▲
	*               pL     sB
	*/
	assert(order > 0);

	Ty_ absPrevLimit = std::abs(prevLimit);
	sig_bit_type bits = msb<Ty_>(absPrevLimit);
	Ty_ mask = (unsigned long long)1 << (bits - 1);
	size_t prefixPos = bits;

	while (prefixPos > 0 && order > 1)
	{
		(absPrevLimit & mask) && --order;
		--prefixPos;
		mask >>= 1;
	}
	return prefixPos;
}
}

#endif		// _MSDB_MMT_H_