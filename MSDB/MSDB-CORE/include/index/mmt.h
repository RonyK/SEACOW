#pragma once
#ifndef _MSDB_MMT_H_
#define _MSDB_MMT_H_

#include <array/array.h>
#include <index/attributeIndex.h>
#include <io/bitstream.h>
#include <io/serializable.h>
#include <util/coordinate.h>
#include <util/math.h>
#include <memory>

namespace msdb
{
template<typename Dty_>
class MinMaxTree;
using mmt = MinMaxTree<position_t>;
using pMMT = std::shared_ptr<mmt>;

template <typename Ty_>
bit_cnt_type getPrefixPosForPrevLimit(Ty_, bit_cnt_type);

template <typename Dty_>
class MinMaxTree : public attributeIndex, public serializable
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
public:
	class mmtNode;
	using pNode = std::shared_ptr<mmtNode>;
	using nodeItr = coorIterator<Dty_, pNode>;

	class mmtNode : public attributeIndex
	{
	public:
		sig_bit_type bMax_;			// significant nit of max value
		sig_bit_type bMin_;			// significant nit of min value
		bit_cnt_type bMaxDelta_;		// bMax_ delta from a parent node
		bit_cnt_type bMinDelta_;		// bMin_ delta from a parent node
		bit_cnt_type order_;			// n th significant nit
		bit_cnt_type bits_;			// required bits to represent min/max value
		element max_;
		element min_;
		eleType eType_;

	public:
		mmtNode(eleType type) : bMax_(0), bMin_(0), bits_(0x80), order_(1), 
			bMaxDelta_(0), bMinDelta_(0), eType_(type)
		{}

	public:
		virtual void setMaxToLimit(sig_bit_type bMax_) = 0;
		virtual void setMinToLimit(sig_bit_type bMax_) = 0;
		virtual void setMaxToLimitFrom(pNode prevNode) = 0;
		virtual void setMinToLimitFrom(pNode prevNode) = 0;

		virtual void initBits() = 0;
		virtual void initBMax() = 0;
		virtual void initBMin() = 0;

		virtual void inMinMax(bstream& bs) = 0;
		virtual void outMinMax(bstream& bs) = 0;

		// use function cause of unknown compile error
		virtual void inDelta(bstream& bs)
		{
			bs >> setw(this->bits_) >> this->bMaxDelta_ >> this->bMinDelta_;
		}
	};

	template<typename Ty_>
	class mmtNodeImpl : public mmtNode
	{
	public:
		mmtNodeImpl() : mmtNode() {}

	public:
		virtual void setMaxToLimit(sig_bit_type bMax_)
		{
			this->max_ = element(getMaxBoundary<Ty_>(bMax_), this->eType_);
		}
		virtual void setMinToLimit(sig_bit_type bMin_)
		{
			this->min_ = element(getMinBoundary<Ty_>(bMin_), this->eType_);
		}
		virtual void setMaxToLimitFrom(pNode prevNode)
		{
			Ty_ prevMax = prevNode->max_.get<Ty_>();
			this->max_ = getMaxBoundary<Ty_>(prevMax, this->order_, this->bMax_);
		}
		virtual void setMinToLimitFrom(pNode prevNode)
		{
			Ty_ prevMin = prevNode->min_.get<Ty_>();
			this->min_ = getMinBoundary<Ty_>(prevMin, this->order_, this->bMin_);
		}

		virtual void inMinMax(bstream& bs)
		{
			Ty_ tMax, tMin;
			bs >> setw(sizeof(Ty_) * CHAR_BIT) >> tMax >> tMin;
			this->bits_ = msb<Ty_>(tMax - tMin);
			this->max_.set<Ty_>(tMax);
			this->min_.set<Ty_>(tMin);
		}

		virtual void outMinMax(bstream& bs)
		{
			bs << setw(sizeof(Ty_) * CHAR_BIT);
			//bs << (Ty_)this->max_ << this->min_;

			Ty_ max = this->max_.get<Ty_>();
			Ty_ min = this->min_.get<Ty_>();
			bs << max << min;
		}

		virtual void initBits()
		{
			Ty_ max = this->max_.get<Ty_>();
			Ty_ min = this->min_.get<Ty_>();
			this->bits_ = msb<Ty_>(max - min);
		}
		virtual void initBMax()
		{
			Ty_ max = this->max_.get<Ty_>();
			this->bMax_ = msb<Ty_>(abs_(max), this->order_) * SIGN(max);
		}
		virtual void initBMin()
		{
			Ty_ min = this->min_.get<Ty_>();
			this->bMin_ = msb<Ty_>(abs_(min), this->order_) * SIGN(min);
		}
	};

	class mmtHeader : public serialHeader
	{
	public:
		static const char mmt_header_version = 1;

	public:
		virtual void serialize(std::ostream& os) override
		{
			os << this->version_ << this->size_ << static_cast<int>(this->eType_) << maxLevel_;
		}
		virtual void deserialize(std::istream& is) override
		{
			int ieType;
			is >> this->version_ >> this->size_ >> ieType >> maxLevel_;

			this->eType_ = static_cast<eleType>(ieType);
		}

	public:
		//eleType dType_;	// dimension type, not used, fixed now.
		eleType eType_;		// element type
		size_type maxLevel_;
	};

protected:
	// inherit from serializable
	virtual void updateToHeader() override
	{
		auto curHeader = std::static_pointer_cast<mmtHeader>(this->getHeader());
		curHeader->version_ = MinMaxTree<Dty_>::mmtHeader::mmt_header_version;
		curHeader->size_ = this->serializedSize_;
		curHeader->maxLevel_ = this->maxLevel_;
		curHeader->eType_ = this->eType_;
	}

	// inherit from serializable
	virtual void updateFromHeader() override
	{
		auto curHeader = std::static_pointer_cast<mmtHeader>(this->getHeader());
		this->maxLevel_ = curHeader->maxLevel_;
		this->eType_ = curHeader->eType_;
	}

public:
	pNode(MinMaxTree::* makeNodeFunc)(eleType);

	template<typename Ty_>
	pNode makeNodeImpl(eleType type)
	{
		return std::make_shared<mmtNodeImpl<Ty_>>(type);
	}

	pNode makeNode()
	{
		return (this->*makeNodeFunc)(this->eType_);
	}

	//////////////////////////////
	// MMT Constructor			//
	//////////////////////////////
public:
	MinMaxTree(eleType eType, dim_vector_reference dim, dim_vector_reference blockDim, size_const maxLevel = 0)
		: dim_(dim), dSize_(dim.size()), leafBlockDim_(blockDim), maxLevel_(maxLevel), serializedSize_(0),
		eType_(eType_), TySize_(getEleSize(eType)), TyBits_(getEleSize(eType)* CHAR_BIT), 
		serializable(std::make_shared<mmtHeader>())
	{
		this->initChunkInDim(dim, blockDim, maxLevel);
		this->setMakeNodeFunc(eType);
	}

	~MinMaxTree() {}

public:
	// deprecated
	// forwardBuildLeaft -> insertLeaf
	void build(chunkIterator& it)
	{
		assert(it.getIterateMode() == iterateMode::EXIST);

		this->nodes_.clear();

		//////////////////////////////
		// Forward build
		// Set Min/Max values [0 (leaf) level -> maxLevel]
		this->forwardBuildLeaf(it);
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

	// ********************
	// * Not used
	//void serialize(void* output, size_const length)
	//{
	//	bstream bs;
	//	this->serialize(bs);

	//	if (length < (bs.size() + CHAR_BIT - 1) / CHAR_BIT)
	//	{
	//		throw std::length_error();
	//	}

	//	memcpy(output, bs.data(), (bs.size() + CHAR_BIT - 1) / CHAR_BIT);
	//}

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

		this->serializedSize_ = bs.capacity();
	}

	virtual void serialize(std::ostream& os)
	{
		bstream bs;
		this->serialize(bs);

		this->getOutHeader()->serialize(os);
		os.write(bs.data(), bs.capacity());
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

	virtual void deserialize(std::istream& is)
	{
		this->getInHeader()->deserialize(is);
		bstream bs;
		bs.resize(this->serializedSize_);
		is.read(bs.data(), this->serializedSize_);
		this->deserialize(bs);
	}

	const pNode getNodes(size_type level)
	{
		assert(level < this->nodes_.size());
		return this->nodes_[level].data();
	}

	const dim_vector& getChunkInDim(size_type level)
	{
		assert(level <= this->levelBlockDims_.size());
		return this->levelBlockDims_[level];
	}

protected:
	coor chunkCoorToBlockCoor(const coor& chunkCoor, const dimension& chunkDimForBlock)
	{
		coor blockCoor(this->dSize_);
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			blockCoor[d] = chunkCoor[d] * chunkDimForBlock[d];
		}
		return blockCoor;
	}

	//////////////////////////////
	// MMT Build Functions
	// For level 0
	void forwardBuildLeaf(chunkIterator& it)
	{
		////////////////////////////////////////
		// Create new mmtNodes
		auto blockDims = this->levelBlockDims_[0];					// get Level 0 block dims
		size_type blockCnt = calcNumItems(blockDims.data(), blockDims.size());	// block numbers
		this->nodes_.push_back(std::vector<pNode>(blockCnt));					// make new node

		auto chunkDim = (*it)->getDesc()->dims_;			// get chunk dimension
		dimension chunkDimForBlock(this->dSize_);			// block container dimension in chunk
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			chunkDimForBlock[d] = chunkDim[d] / blockDims[d];
		}

		// Node iterator for all level 0 nodes
		// Use to convert block coordinate to seqencial node index
		nodeItr nit(this->nodes_[0].data(), this->dSize_, blockDims.data());

		while(!it.isEnd())
		{
			// Setup a start point of blockCoor for blocks in a chunk
			coor blockCoorSp = this->chunkCoorToBlockCoor(it.coor(), chunkDimForBlock);
			coorItr bit(this->dSize_, chunkDimForBlock.data());
			while(!bit.isEnd())
			{
				// Set iterator range according the block size
				coor inBlockCoor = bit.coor(), inBlockDimSp(this->dSize_), inBlockDimEp(this->dSize_);
				for (dimensionId d = 0; d < this->dSize_; ++d)
				{
					inBlockDimSp[d] = blockDims[d] * inBlockCoor[d];
					inBlockDimEp[d] = blockDims[d] * (inBlockCoor[d] + 1);
				}

				// TODO :: Reduce in one line
				auto iit = (*it)->getItemRangeIterator(inBlockDimSp.data(), inBlockDimEp.data());
				auto lNode = this->forwardBuildLeafNode(iit);

				coor blockCoor(this->dSize_, blockCoorSp.data());
				for (dimensionId d = 0; d < this->dSize_; ++d)
				{
					blockCoor[d] += inBlockCoor[d];
				}

				this->nodes_[0][nit.coorToSeq(blockCoor)] = lNode;
				++bit;	// Move on a next block in the chunk
			}

			++it;	// Move on a next chunk
		}
	}

	pNode forwardBuildLeafNode(itemItr& iit)
	{
		pNode node = this->makeNode();

		auto value = *iit;
		node->setMax(value);
		node->setMin(value);
		node->bits_ = (bit_cnt_type)TyBits_;
		++iit;

		while(!iit.isEnd())
		{
			auto v = *iit;
			if(node->compareMax(v) < 0)
			{
				node->setMax(v);
			}
			if(node->compareMin(v) > 0)
			{
				node->setMin(v);
			}
			++iit;
		}

		return node;
	}

	// For level 1 ~ maxLevel
	// Notes: In this method, prev is a lower level nodes (finer)
	void forwardBuildNonLeaf(const size_type level)
	{
		assert(level > 0);

		////////////////////////////////////////
		// Create new mmtNodes
		dim_vector pChunksInDim = this->levelBlockDims_[level - 1];
		dim_vector chunksInDim = this->levelBlockDims_[level];
		const size_type chunkCnt = calcNumItems(chunksInDim.data(), chunksInDim.size());
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
			coordinate<Dty_> cur = pcit.coor();
			for (size_type d = 0; d < this->dSize_; ++d)
			{
				cur[d] /= 2;
			}

			// get target chunk
			cit.moveTo(cur);
			pNode node = makeNode();
			(*cit) = node;

			// init min, max value
			if (node->bits_ == 0x80)
			{
				node->max_ = (*pcit)->max_;
				node->min_ = (*pcit)->min_;
				node->bits_ = (bit_cnt_type)TyBits_;
			} else
			{
				// compare min max value
				if(node->max_ < (*pcit)->max_)
				{
					node->max_ = (*pcit)->max_;
				}
				if(node->min_ > (*pcit)->min_)
				{
					node->min_ = (*pcit)->min_;
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
		dim_vector pChunksInDim = this->levelBlockDims_[level + 1];
		dim_vector chunksInDim = this->levelBlockDims_[level];

		////////////////////////////////////////
		// Update nit order for chunks in current level
		// Prev
		coorIterator<Dty_, pNode> pcit(this->nodes_[level + 1].data(), this->dSize_,
									   pChunksInDim.data());

		// Current
		coorIterator<Dty_, pNode> cit(this->nodes_[level].data(), this->dSize_,
									  chunksInDim.data());

		for (size_type i = 0; i < this->nodes_[level + 1].size(); ++i, ++pcit)
		{
			coordinate<Dty_> childBase = pcit.coor();
			for (size_type d = 0; d < this->dSize_; d++)
			{
				childBase[d] *= 2;
			}

			pNode prevNode = (*pcit);
			bool sameOrder = (bool)(prevNode->bMax_ - prevNode->bMin_);	// if bMax_ == bMin_, move on to the next most significant nit
			bit_cnt_type bits = sameOrder ? msb<sig_bit_type>(prevNode->bMax_ - prevNode->bMin_) : msb<sig_bit_type>(abs_(prevNode->bMax_) - 1);

			for (size_type cID = 0; cID < siblings; cID++)
			{
				// Set child coordinate and move to iit
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
					cNode->bits_ = bits;
					cNode->order_ = prevNode->order_;

					cNode->bMaxDelta_ = static_cast<bit_cnt_type>(prevNode->bMax_ - cNode->bMax_);	// max: prev >= cur
					cNode->bMinDelta_ = static_cast<bit_cnt_type>(cNode->bMin_ - prevNode->bMin_);	// min: prev <= cur
				} else
				{
					if ((bit_cnt_type)prevNode->order_ + 1 < TyBits_)
					{
						// Move to next significant nit
						cNode->bits_ = bits;
						cNode->order_ = prevNode->order_ + 1;

						cNode->initBMax();
						cNode->initBMin();
						//cNode->bMax_ = msb<Ty_>(std::abs(cNode->max), cNode->order_) * SIGN(cNode->max);
						//cNode->bMin_ = msb<Ty_>(std::abs(cNode->min), cNode->order_) * SIGN(cNode->min);

						cNode->bMaxDelta_ = std::max(abs_(prevNode->bMax_ - cNode->bMax_) - 1, 0);
						cNode->bMinDelta_ = std::max(abs_(cNode->bMin_ - prevNode->bMin_) - 1, 0);
					} else
					{
						// The last nit. Has no more next significant nit
						cNode->bits_ = 0;
						cNode->order_ = prevNode->order_;
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
			curLevelNodes[i]->outMinMax(bs);
			//bs << setw(TyBits_) << curLevelNodes[i]->max << curLevelNodes[i]->min;
		}
	}

	void serializeNonRoot(bstream& bs, size_type level)
	{
		pNode* cNodes = this->nodes_[level].data();
		for (size_type i = 0; i < this->nodes_[level].size(); i++)
		{
			bs << setw(cNodes[i]->bits_) << cNodes[i]->bMaxDelta_ << cNodes[i]->bMinDelta_;
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
			// TODO:: Calc blockCnt on each level
			this->nodes_.push_back(std::vector<pNode>());
		}

		////////////////////////////////////////
		// Create new mmtNodes
		auto chunksInDim = this->levelBlockDims_[this->maxLevel_];
		size_type chunkCnt = calcNumItems(chunksInDim.data(), chunksInDim.size());
		this->nodes_[this->maxLevel_].resize(chunkCnt);	// TODO::If generating Nodes are complete, remove this line.
		pNode* rootNodes = this->nodes_[this->maxLevel_].data();

		for (size_type i = 0; i < chunkCnt; i++)
		{
			rootNodes[i] = makeNode();
			rootNodes[i]->inMinMax(bs);

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
		dim_vector pChunksInDim = this->levelBlockDims_[level + 1];
		dim_vector chunksInDim = this->levelBlockDims_[level];
		size_type chunkCnt = calcNumItems(chunksInDim.data(), chunksInDim.size());
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
			pcit.moveTo(this->getParentCoor(cit.coor()));
			pNode prevNode = (*pcit);

			bool sameOrder = prevNode->bMax_ != prevNode->bMin_;	// if bMax_ == bMin_, move on to the next most significant nit
			cNode->bits_ = sameOrder ? msb<sig_bit_type>(prevNode->bMax_ - prevNode->bMin_) : msb<sig_bit_type>(abs_(prevNode->bMax_) - 1);
			cNode->inDelta(bs);

			if (sameOrder)
			{
				cNode->order_ = prevNode->order_;
				cNode->bMax_ = prevNode->bMax_ - cNode->bMaxDelta_;
				cNode->bMin_ = prevNode->bMin_ + cNode->bMinDelta_;

				if (cNode->order_ == 1)
				{
					cNode->setMaxToLimit(cNode->bMax_);
					cNode->setMinToLimit(cNode->bMin_);
					//cNode->max = getMaxBoundary<Ty_>(cNode->bMax_);
					//cNode->min = getMinBoundary<Ty_>(cNode->bMin_);
				} else
				{
					cNode->setMaxToLimitFrom(prevNode);
					cNode->setMinToLimitFrom(prevNode);
					//cNode->max = getMaxBoundary(prevNode->max, cNode->order_, cNode->bMax_);
					//cNode->min = getMinBoundary(prevNode->min, cNode->order_, cNode->bMin_);
				}
			} else
			{
				if ((size_type)prevNode->order_ + 1 < TyBits_)
				{
					// Move to next significant nit
					cNode->order_ = prevNode->order_ + 1;
					cNode->bMax_ = prevNode->bMax_ - cNode->bMaxDelta_ - 1;
					cNode->bMin_ = prevNode->bMin_ - cNode->bMinDelta_ - 1;

					cNode->setMaxToLimitFrom(prevNode);
					cNode->setMinToLimitFrom(prevNode);
					//cNode->max = getMaxBoundary(prevNode->max, cNode->order_, cNode->bMax_);
					//cNode->min = getMinBoundary(prevNode->min, cNode->order_, cNode->bMin_);
				} else
				{
					cNode->bits_ = 0;
					cNode->order_ = prevNode->order_;
					cNode->max_ = prevNode->max_;
					cNode->min_ = prevNode->min_;
				}
			}

			// Fill out min, max values.
			if (cNode->order_ == 1)
			{
				cNode->setMaxToLimit(cNode->bMax_);
				cNode->setMinToLimit(cNode->bMin_);
				//cNode->max = getMaxBoundary<Ty_>(cNode->bMax_);
				//cNode->min = getMinBoundary<Ty_>(cNode->bMin_);
			} else
			{
				if (cNode->bits_)
				{
					cNode->setMaxToLimitFrom(prevNode);
					cNode->setMinToLimitFrom(prevNode);
					//cNode->max = getMaxBoundary(prevNode->max, cNode->order_, cNode->bMax_);
					//cNode->min = getMinBoundary(prevNode->min, cNode->order_, cNode->bMin_);
				}
			}

			++cit;	// Move to next data
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
			this->levelBlockDims_.push_back(calcChunkNums(dims.data(), chunkDim.data(), dims.size(), level));
		}
	}

	_NODISCARD coordinate<Dty_> getChildBaseCoor(coordinate<Dty_>& parentCoor)
	{
		coordinate<Dty_> childBase = parentCoor;
		for (size_type d = 0; d < this->dSize_; d++)
		{
			childBase[d] *= 2;
		}
		return childBase;
	}

	_NODISCARD coordinate<Dty_> getParentCoor(coordinate<Dty_>& childCoor)
	{
		coordinate<Dty_> coorParent = childCoor;
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

		case eleType::CHAR:
			this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<char>;
			break;
		case eleType::INT8:
			this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<int8_t>;
			break;
		case eleType::INT16:
			this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<int16_t>;
			break;
		case eleType::INT32:
			this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<int32_t>;
			break;
		case eleType::INT64:
			this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<int64_t>;
			break;
		case eleType::UINT8:
			this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<uint8_t>;
			break;
		case eleType::UINT16:
			this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<uint16_t>;
			break;
		case eleType::UINT32:
			this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<uint32_t>;
			break;
		case eleType::UINT64:
			this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<uint64_t>;
			break;

		// NOT SUPPORTS
		//case eleType::BOOL:
		//	this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<bool>;
		//	break;
		//case eleType::FLOAT:
		//	this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<float>;
		//	break;
		//case eleType::DOUBLE:
		//	this->makeNodeFunc = &MinMaxTree<Dty_>::makeNodeImpl<double>;
		//	break;
		}
	}

private:
	const size_type TySize_;
	const size_type TyBits_;
	eleType eType_;

private:
	size_type dSize_;
	size_type serializedSize_;
	size_type maxLevel_;
	dim_vector dim_;		// dimensions
	dim_vector leafBlockDim_;
	std::vector<dim_vector> levelBlockDims_;
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
}

#endif		// _MSDB_MMT_H_