#pragma once
#ifndef _MSDB_MMT_H_
#define _MSDB_MMT_H_

#include <stdafx.h>
#include <array/array.h>
#include <index/mmtNode.h>
#include <io/serializable.h>
#include <index/attributeIndex.h>
#include <util/coordinate.h>
#include <util/logger.h>
#include <utility>

namespace msdb
{
class MinMaxTree;
using mmt = MinMaxTree;
using pMMT = std::shared_ptr<mmt>;

class MinMaxTree : public attributeIndex, public serializable
{
public:
	using size_type = size_t;
	using size_const = const size_t;

protected:
	//////////////////////////////
	// MMT Header				//
	//////////////////////////////
	class mmtHeader : public serialHeader
	{
	public:
		static const char mmt_header_version = 1;

	public:
		virtual void serialize(std::ostream& os) override
		{
			std::cout << "Header serialize" << std::endl;
			std::cout << this->version_ << ", " << this->bodySize_ << ", " << static_cast<int>(this->eType_) << ", " << this->lowerLevel_ << std::endl;

			int eTypeOut = static_cast<int>(this->eType_);
			os.write((char*)(&this->version_), sizeof(this->version_));
			os.write((char*)(&this->bodySize_), sizeof(this->bodySize_));
			os.write((char*)(&eTypeOut), sizeof(int));
			os.write((char*)(&this->lowerLevel_), sizeof(this->lowerLevel_));
			//os << this->version_ << this->size_ << static_cast<int>(this->eType_) << this->lowerLevel_;
			// operator<< outputs the values in ASCII CODE (TEXT).
		}
		virtual void deserialize(std::istream& is) override
		{
			std::cout << "Header deserialize" << std::endl;

			int eTypeIn;
			is.read((char*)(&this->version_), sizeof(this->version_));
			is.read((char*)(&this->bodySize_), sizeof(this->bodySize_));
			is.read((char*)(&eTypeIn), sizeof(int));
			is.read((char*)(&this->lowerLevel_), sizeof(this->lowerLevel_));
			this->eType_ = static_cast<eleType>(eTypeIn);

			//is >> this->version_ >> this->size_ >> eTypeIn >> lowerLevel_;
			std::cout << this->version_ << ", " << this->bodySize_ << ", " << static_cast<int>(this->eType_) << ", " << this->lowerLevel_ << std::endl;
		}

	public:
		//eleType dType_;	// dimension eType, not used, fixed now.
		eleType eType_;		// element eType
		size_type lowerLevel_;
	};

public:
	MinMaxTree(const eleType eType, size_const maxLevel = 0);

public:
	virtual void build(pChunkIterator& it) = 0;
	virtual void serialize(std::ostream& os) = 0;
	virtual void deserialize(std::istream& is) = 0;

public:
	eleType getEleType();
	size_type getMaxLevel();

public:
	static pMMT createMMT(const eleType eType,
						  const dimension& dim,
						  const dimension& chunkDim,
						  const dimension& blockDim,
						  size_const lowerLevel = 0);

protected:
	eleType eType_;
	size_type lowerLevel_;
	size_type maxLevel_;
};
template <typename Dty_, typename Ty_>
class MinMaxTreeImpl;

template <typename Dty_, typename Ty_>
class MinMaxTreeImpl : public MinMaxTree
{
public:
	using size_type = size_t;
	using size_const = const size_t;

	using dim_type = Dty_;
	using dim_pointer = Dty_*;
	using dim_const_pointer = const Dty_*;
	using dim_reference = Dty_&;
	using dim_const_reference = const Dty_;

	//////////////////////////////
	// MMT NODE					//
	//////////////////////////////
public:
	using nodeItr = itemIterator<Dty_, pMmtNode>;

protected:
	// inherit from serializable
	virtual void updateToHeader() override
	{
		auto curHeader = std::static_pointer_cast<mmtHeader>(this->getHeader());
		curHeader->version_ = MinMaxTree::mmtHeader::mmt_header_version;
		curHeader->bodySize_ = this->serializedSize_;
		curHeader->lowerLevel_ = this->lowerLevel_;
		curHeader->eType_ = this->eType_;
	}

	// inherit from serializable
	virtual void updateFromHeader() override
	{
		auto curHeader = std::static_pointer_cast<mmtHeader>(this->getHeader());
		this->lowerLevel_ = curHeader->lowerLevel_;
		this->serializedSize_ = curHeader->bodySize_;
		this->eType_ = curHeader->eType_;

		this->initNodeSpace();
		this->findChunkBlockLevel();
	}

	//////////////////////////////
	// MMT Constructor			//
	//////////////////////////////
public:
	MinMaxTreeImpl(const eleType eType, 
				   const dimension& dim, const dimension& chunkDim, const dimension& blockDim,
				   size_const lowerLevel = 0)
		: MinMaxTree(eType, lowerLevel), dims_(dim), dSize_(dim.size()), chunkDims_(chunkDim), blockDims_(blockDim),
		leafDims_(blockDim / pow(2, lowerLevel)), blockSpace_(chunkDim / blockDim), TySize_(getEleSize(eType)),
		TyBits_(getEleSize(eType)* CHAR_BIT)
	{}

	~MinMaxTreeImpl() {}

public:
	// deprecated
	// forwardBuildLeaft -> insertLeaf
	void build(pChunkIterator& it)
	{
		assert(it->getIterateMode() == iterateMode::EXIST);

		//////////////////////////////
		// Init values
		this->initNodeSpace();
		this->findChunkBlockLevel();
		this->nodes_.clear();

		//////////////////////////////
		// Forward build
		// Set Min/Max values [0 (leaf) level -> refineLevel]
		this->forwardBuildLeaf(it);
		for (size_type level = 1; level <= this->maxLevel_; ++level)
		{
			this->forwardBuildNonLeaf(level);
			if(this->nodes_[level].size() == 1)
			{
				this->maxLevel_ = level;
			}
		}

		if(this->nodes_[this->maxLevel_].size() != 1)
		{
			++this->maxLevel_;
			this->forwardBuildRoot();
		}
		//////////////////////////////

		//////////////////////////////
		// Backward Build
		// Set bits [refineLevel -> 0 level]
		this->backwardBuildRoot();

		if(this->maxLevel_ >= 1)
		{
			this->backwardBuildFromRoot();

			for (size_type level = this->maxLevel_ - 2; level != (size_type)-1; --level)
			{
				this->backwardBuildNonRoot(level);
			}
		}
		//////////////////////////////
	}

	nodeItr getNodeIterator(size_type level)
	{
		return nodeItr(this->nodes_[level].data(), this->dSize_, this->nodeSpace_[level].data());
	}

	//////////////////////////////
	// MMT Serialize			//
	//////////////////////////////
	void serialize(bstream& bs)
	{
		size_type maxLevel = this->nodes_.size() - 1;

		if (maxLevel == (size_type)-1)
		{
			return;		// Empty
		}

		this->serializeRoot(bs);
		size_t nodeNums = 0;
		BOOST_LOG_TRIVIAL(debug) << "root: " << this->nodes_[maxLevel].size();
		nodeNums += this->nodes_[maxLevel].size();

		for (size_type l = maxLevel - 1; l != (size_type)-1; l--)
		{
			this->serializeNonRoot(bs, l);
			BOOST_LOG_TRIVIAL(debug) << "level" << l << ": " << this->nodes_[l].size();
			nodeNums += this->nodes_[l].size();
		}
		BOOST_LOG_TRIVIAL(debug) << "Total Nodes: " << nodeNums;

		this->serializedSize_ = bs.capacity();	// set serialized size
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
		////////////////////////////////////////
		// Re-generate nodes
		this->nodes_.clear();
		for (size_type l = 0; l <= this->maxLevel_; ++l)
		{
			// TODO:: Calc blockCnt on each level
			this->nodes_.push_back(std::vector<pMmtNode>());
		}

		this->deserializeRoot(bs);
		for (size_type l = this->maxLevel_ - 1; l != (size_type)-1; l--)
		{
			this->deserializeNonRoot(bs, l);
		}
	}

	virtual void deserialize(std::istream& is)
	{
		this->getHeader()->deserialize(is);
		this->updateFromHeader();
		bstream bs;
		bs.resize(this->serializedSize_);
		is.read(bs.data(), this->serializedSize_);
		this->deserialize(bs);
	}

public:
	coordinate<Dty_> chunkCoorToBlockCoor(const coordinate<Dty_>& chunkCoor, const dimension& chunkDimForBlock)
	{
		coordinate<Dty_> blockCoor(this->dSize_);
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			blockCoor[d] = chunkCoor[d] * chunkDimForBlock[d];
		}
		return blockCoor;
	}

protected:
	//////////////////////////////
	// MMT Build Functions
	// For level 0
	void forwardBuildLeaf(pChunkIterator& cItr)
	{
		////////////////////////////////////////
		// Create new mmtNodes
		this->nodes_.push_back(std::vector<pMmtNode>(this->nodeSpace_[0].area()));
		dimension leafSpace = this->blockDims_ / this->leafDims_;
		coorItr leafItr = coorItr(leafSpace);

		BOOST_LOG_TRIVIAL(debug) << "leaf Space: " << leafSpace.toString();

		while (!cItr->isEnd())
		{
			// Setup a start point of blockCoor for blocks in a chunk
			auto bItr = (**cItr)->getBlockIterator();
			while (!bItr->isEnd())
			{
				leafItr.moveToStart();
				while(!leafItr.isEnd())
				{
					auto bItemItr = (**bItr)->getItemRangeIterator(
						coorRange(leafItr.coor() * this->leafDims_, leafItr.coor() * this->leafDims_ + this->leafDims_));
					auto lNode = this->forwardInitLeafNode(bItemItr);
					this->setNode(lNode, cItr->coor(), bItr->coor(), leafItr.coor());

#ifndef NDEBUG
					//if (lNode->getMin<Ty_>() < 0)
					//{
					//	BOOST_LOG_TRIVIAL(warning) << "Wrong MMT value at " << cItr->coor().toString() << "|" << bItr->coor().toString() << ": " << lNode->getMin<Ty_>();
					//}
#endif
					++leafItr;

				}
				++(*bItr);	// Move on a next block in the chunk
			}

			++(*cItr);		// Move on a next chunk
		}
	}

	pMmtNode forwardInitLeafNode(pBlockItemRangeIterator iit)
	{
		pMmtNode node = std::make_shared<mmtNode>();

		Ty_ v = preprocessingForValue((**iit).get<Ty_>());
		Ty_ min_ = v;
		Ty_ max_ = v;

		node->bits_ = (bit_cnt_type)TyBits_;
		++(*iit);

		while (!iit->isEnd())
		{
			v = preprocessingForValue((**iit).get<Ty_>());
			//std::cout << static_cast<int>(v) << ", ";
			if (max_ < v)
			{
				max_ = v;
			}
			if (min_ > v)
			{
				min_ = v;
			}
			++(*iit);
		}

		node->min_ = min_;
		node->max_ = max_;
		node->realMin_ = min_;
		node->realMax_ = max_;

		return node;
	}

	// For level 1 ~ refineLevel
	// Notes: In this method, prev is a lower level nodes (finer)
	void forwardBuildNonLeaf(const size_type level)
	{
		assert(level > 0);

		////////////////////////////////////////
		// Create new mmtNodes
		dimension prevNodeSpace = this->nodeSpace_[level - 1];
		dimension nodeSpace = this->nodeSpace_[level];
		size_type blockCnt = nodeSpace.area();
		//const size_type blockCnt = calcNumItems(nodeSpace.data(), nodeSpace.size());
		this->nodes_.push_back(std::vector<pMmtNode>(this->nodeSpace_[level].area()));

		for (size_t i = 0; i < blockCnt; ++i)
		{
			this->nodes_[level][i] = std::make_shared<mmtNode>();
		}

		////////////////////////////////////////
		// Update min/max values
		itemIterator<Dty_, pMmtNode> pcit(this->nodes_[level - 1].data(), this->dSize_,
									   prevNodeSpace.data());
		itemIterator<Dty_, pMmtNode> cit(this->nodes_[level].data(), this->dSize_,
									  nodeSpace.data());

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
			auto node = (*cit);

			// init min, max value
			if (node->bits_ == 0x80)
			{
				node->realMax_ = node->max_ = (*pcit)->getMax<Ty_>();
				node->realMin_ = node->min_ = (*pcit)->getMin<Ty_>();
				node->bits_ = (bit_cnt_type)TyBits_;
			} else
			{
				// compare min max value
				if (node->getMax<Ty_>() < (*pcit)->getMax<Ty_>())
				{
					node->realMax_ = node->max_ = (*pcit)->getMax<Ty_>();
				}
				if (node->getMin<Ty_>() > (*pcit)->getMin<Ty_>())
				{
					node->realMin_ = node->min_ = (*pcit)->getMin<Ty_>();
				}
			}
			this->nodes_[level][cit.coorToSeq(cur)] = node;

			//std::cout << "forward-" << std::endl;
			//std::cout << "[" << cur[0] << ", " << cur[1] << "] : " << static_cast<int>(node->getMin<Ty_>()) << "~" << static_cast<int>(node->max_) << std::endl;

			// move to next chunk
			++pcit;
		}
	}

	void forwardBuildRoot()
	{
		assert(this->maxLevel_ > 0);

		////////////////////////////////////////
		// Create new mmtNodes
		this->nodeSpace_.push_back(dimension(this->dSize_, 1));

		dimension prevNodeSpace = this->nodeSpace_[this->maxLevel_ - 1];
		dimension nodeSpace(this->dSize_, 1);
		size_type blockCnt = nodeSpace.area();

		if(this->nodeSpace_.size() <= this->maxLevel_)
		{
			this->nodeSpace_.push_back(nodeSpace);
		}else
		{
			this->nodeSpace_[this->maxLevel_] = nodeSpace;
		}
		this->nodes_.push_back(std::vector<pMmtNode>(nodeSpace.area()));
		this->nodes_[this->maxLevel_][0] = std::make_shared<mmtNode>();	// Root has a single mmtNode

		////////////////////////////////////////
		// Update min/max values
		itemIterator<Dty_, pMmtNode> pcit(this->nodes_[this->maxLevel_ - 1].data(), this->dSize_,
									   prevNodeSpace.data());

		auto node = this->nodes_[this->maxLevel_][0];
		for (size_type i = 0; i < this->nodes_[this->maxLevel_ - 1].size(); ++i)
		{
			// init min, max value
			if (node->bits_ == 0x80)
			{
				node->realMax_ = node->max_ = (*pcit)->getMax<Ty_>();
				node->realMin_ = node->min_ = (*pcit)->getMin<Ty_>();
				node->bits_ = (bit_cnt_type)TyBits_;
			} else
			{
				// compare min max value
				if (node->getMax<Ty_>() < (*pcit)->getMax<Ty_>())
				{
					node->realMax_ = node->max_ = (*pcit)->getMax<Ty_>();
				}
				if (node->getMin<Ty_>() > (*pcit)->getMin<Ty_>())
				{
					node->realMin_ = node->min_ = (*pcit)->getMin<Ty_>();
				}
			}
			++pcit;
		}

		this->nodes_[this->maxLevel_][0] = node;
	}

	// For max level nodes
	void backwardBuildRoot()
	{
		assert(this->nodes_.back().size() == 1);

		this->nodes_.back()[0]->initBits<Ty_>();
		updateChildNodeOrder<Ty_>(this->nodes_.back()[0]);
	}

	// For max level nodes
	void backwardBuildFromRoot()
	{
		pMmtNode rootNode = this->nodes_[this->maxLevel_][0];
		pMmtNode* curLevel = this->nodes_[this->maxLevel_ - 1].data();

		for (size_type i = 0; i < this->nodes_[this->maxLevel_ - 1].size(); ++i)
		{
			backwardUpdateNode(rootNode, curLevel[i], isFinished(rootNode), isChildOrderChanged(rootNode));
		}
	}

	// Except max level nodes ((refineLevel - 1) -> level 0 )
	void backwardBuildNonRoot(const size_type level)
	{
		assert(level < this->nodes_.size() - 2);		// Not for max level nodes
		assert(level != (size_type)-1);

		size_type pLevel = level + 1;
		size_type childs = (size_type)pow(2, this->dims_.size());

		////////////////////////////////////////
		// Calc chunk num of prev and current level
		//
		// Notes: In this method, prev is a upper level nodes (coarse)
		dimension prevLevelDim = this->nodeSpace_[level + 1];
		dimension levelDim = this->nodeSpace_[level];

		////////////////////////////////////////
		// Update nit order for chunks in current level
		// Prev
		itemIterator<Dty_, pMmtNode> pcit(this->nodes_[level + 1].data(), this->dSize_,
									   prevLevelDim.data());
		// Current
		itemIterator<Dty_, pMmtNode> cit(this->nodes_[level].data(), this->dSize_,
									  levelDim.data());

		for (size_type i = 0; i < this->nodes_[level + 1].size(); ++i, ++pcit)
		{
			// Setup child Base
			coordinate<Dty_> childBase = pcit.coor();
			for (size_type d = 0; d < this->dSize_; d++)
			{
				childBase[d] *= 2;
			}
			pMmtNode prevNode = (*pcit);

			// Iterate childs
			for (size_type cID = 0; cID < childs; cID++)
			{
				// Set child coordinate and move to cit
				coordinate<Dty_> cur = childBase;
				for (size_type d = 0; d < this->dSize_; ++d)
				{
					if (cID & ((size_type)0x1 << d))
					{
						cur[d] += 1;
					}
				}
				cit.moveTo(cur);

				// Update
				pMmtNode curNode = (*cit);
				curNode->parent_ = prevNode;

				assert(pcit.coor() == this->getParentCoor(cit.coor()));
				backwardUpdateNode(prevNode, curNode, isFinished(prevNode), isChildOrderChanged(prevNode));
			}
		}
	}

	void backwardUpdateNode(pMmtNode prevNode, pMmtNode curNode, bool finished, bool orderChanged)
	{
		bit_cnt_type jumpedBits = 0;

		if (finished)
		{
			// The last bit. Has no more next significant bit
			curNode->bits_ = 0;
			curNode->finished_ = true;
			curNode->order_ = prevNode->childOrder_;
			curNode->bMin_ = prevNode->bMin_;
			curNode->bMax_ = prevNode->bMax_;
			curNode->bMinDelta_ = 0;
			curNode->bMaxDelta_ = 0;
			curNode->childOrder_ = curNode->order_;

			curNode->min_ = prevNode->min_;
			curNode->max_ = prevNode->max_;

			return;		// No fahter update for finished node
		}

		if (orderChanged)
		{
			// Order changed: move on to the next significant bit
			// If order changed, bMin_ == bMax_ in prevNode
			// any value is okay for next bits_
			curNode->bits_ = msb<sig_bit_type>(abs_(prevNode->bMax_) - 1);	// decrease required bits
			curNode->order_ = prevNode->childOrder_;
			curNode->setMinMaxBits<Ty_>();

			// (curNode->bMin_ - prevNode->bMin_) can be negative -> as order changed
			// 
			// If positive: prevbMin > curbMin
			//				prevbMax > curbMax
			//
			// If negative: prevbMin < curbMin
			//				prvbMax < curbMax
			//
			// as order changed, move on to the next significant bit and curNode->bMin_ should be smaller.
			curNode->bMinDelta_ = abs_(curNode->bMin_ - prevNode->bMin_) - 1;
			curNode->bMaxDelta_ = abs_(prevNode->bMax_ - curNode->bMax_) - 1;

			jumpedBits = updateChildNodeOrder<Ty_>(curNode);		// order changed -> child node order changed
		} else
		{
			// Order not changed
			curNode->bits_ = msb<sig_bit_type>(prevNode->bMax_ - prevNode->bMin_);
			curNode->order_ = prevNode->childOrder_;
			curNode->setMinMaxBits<Ty_>();

			curNode->bMinDelta_ = static_cast<bit_cnt_type>(curNode->bMin_ - prevNode->bMin_);	// min: prev <= cur
			curNode->bMaxDelta_ = static_cast<bit_cnt_type>(prevNode->bMax_ - curNode->bMax_);	// max: prev >= cur

			jumpedBits = updateChildNodeOrder<Ty_>(curNode);		// order changed -> child node order changed
			//curNode->childOrder_ = prevNode->childOrder_;	// order not changed -> child node order sustained
		}

//#ifndef NDEBUG
//		Ty_ tMax = curNode->getMax<Ty_>();
//		Ty_ tMin = curNode->getMin<Ty_>();
//#endif

		curNode->min_ = getMinBoundary<Ty_>(prevNode->getMin<Ty_>(), curNode->order_, curNode->bMin_);
		curNode->max_ = getMaxBoundary<Ty_>(prevNode->getMax<Ty_>(), curNode->order_, curNode->bMax_);

		if (isChildOrderChanged(curNode))
		{
			if (curNode->childOrder_)
			{
				backwardUpdateChildOrderChangedNode<Ty_>(curNode, jumpedBits);
			} else
			{
				// Childorder == 0, finished at child
				curNode->min_ = prevNode->getRealMin<Ty_>();
				curNode->max_ = prevNode->getRealMax<Ty_>();
			}
		}
		
		if (curNode->getMax<Ty_>() < curNode->getRealMax<Ty_>() || curNode->getMin<Ty_>() > curNode->getRealMin<Ty_>())
		{
			std::cout << "error";
		}
		assert(curNode->getMin<Ty_>() <= curNode->getRealMin<Ty_>());
		assert(curNode->getMax<Ty_>() >= curNode->getRealMax<Ty_>());

	}

	////////////////////////////////////////
	// Write bits in an outstream
	void serializeRoot(bstream& bs)
	{
		assert(this->nodes_.back().size() == 1);
		pMmtNode* rootNodes = this->nodes_.back().data();
		pMmtNode rootNode = rootNodes[0];
		
		rootNode->outMinMax<Ty_>(bs);		
		if(isChildOrderChanged(rootNode))
		{
			// root node only record 'order changed'
			rootNode->outChildOrderChanged(bs);
		}
	}

	void serializeNonRoot(bstream& bs, size_type level)
	{
		pMmtNode* curLevelNodes = this->nodes_[level].data();
		for (size_type i = 0; i < this->nodes_[level].size(); i++)
		{
			pMmtNode curNode = curLevelNodes[i];
			if (curNode->finished_)
			{
				continue;
			}

			//////////////////////////////
			// Works for not finished nodes
			curNode->outDelta(bs);

			if (isChildOrderChanged(curNode))
			{
				// non-root node record both 'order changed' and 'jumped bits'
				curNode->outChildOrderChanged(bs);
				if (curNode->childOrder_)
				{
					curNode->outJumpedBits<Ty_>(bs, (bit_cnt_type)msb<Ty_>(abs_(curNode->getRealMax<Ty_>()), curNode->childOrder_));
				} else
				{
					curNode->outJumpedBits<Ty_>(bs, 1);	// copy to end
				}
			}
			//////////////////////////////
		}
	}

	////////////////////////////////////////
	// Build MMT from an instream
	void deserializeRoot(bstream& bs)
	{
		////////////////////////////////////////
		// Create new mmtNodes
		//auto chunksInDim = this->nodeSpace_[this->lowerLevel_];
		//size_type chunkCnt = calcNumItems(chunksInDim.data(), chunksInDim.size());
		//this->nodes_[this->lowerLevel_].resize(chunkCnt);	// TODO::If generating Nodes are complete, remove this line.
		//pMmtNode* rootNodes = this->nodes_[this->lowerLevel_].data();

		this->nodes_[this->lowerLevel_].resize(1);
		pMmtNode* rootNodes = this->nodes_[this->lowerLevel_].data();
		pMmtNode rootNode = rootNodes[0];

		rootNode = std::make_shared<mmtNode>();
		rootNode->bits_ = sizeof(Ty_) * CHAR_BIT;
		rootNode->order_ = 1;
		rootNode->inMinMax<Ty_>(bs);
		
		if(isChildOrderChanged(rootNode))
		{
			rootNode->inChildOrderChanged(bs);
		}
	}

	void deserializeNonRoot(bstream& bs, size_type level)
	{
		// Calc chunk num of prev and current level
		dimension pChunksInDim = this->nodeSpace_[level + 1];
		dimension chunksInDim = this->nodeSpace_[level];
		size_type chunkCnt = calcNumItems(chunksInDim.data(), chunksInDim.size());
		this->nodes_[level].resize(chunkCnt);

		// Prev
		itemIterator<Dty_, pMmtNode> pcit(this->nodes_[level + 1].data(), this->dSize_,
									   pChunksInDim.data());

		// Current
		itemIterator<Dty_, pMmtNode> cit(this->nodes_[level].data(), this->dSize_,
									  chunksInDim.data());

		for (size_type i = 0; i < chunkCnt; i++)
		{
			pMmtNode curNode = std::make_shared<mmtNode>();
			(*cit) = curNode;
			pcit.moveTo(this->getParentCoor(cit.coor()));

			pMmtNode prevNode = (*pcit);
			curNode->parent_ = prevNode;

			deserializeUpdateNode(bs, prevNode, curNode, isFinished(prevNode));

			++cit;	// Move to next data
		}
	}

	void deserializeUpdateNode(bstream& bs, pMmtNode prevNode, pMmtNode curNode, bool finished)
	{
		if (finished)
		{
			curNode->bits_ = 0;
			curNode->finished_ = true;
			curNode->order_ = prevNode->childOrder_;
			curNode->bMin_ = prevNode->bMin_;
			curNode->bMax_ = prevNode->bMax_;
			curNode->min_ = prevNode->getMin<Ty_>();
			curNode->max_ = prevNode->getMax<Ty_>();
			curNode->bMinDelta_ = 0;
			curNode->bMaxDelta_ = 0;

			return;
		}

		//////////////////////////////
		// Works for not finished nodes
		curNode->inDelta(bs);

		curNode->order_ = prevNode->childOrder_;
		curNode->bMin_ = prevNode->bMin_ + curNode->bMinDelta_;
		curNode->bMax_ = prevNode->bMax_ - curNode->bMaxDelta_;

		if (isChildOrderChanged(curNode))
		{
			curNode->inChildOrderChanged(bs);

			// Get jump value and bits
			bit_cnt_type jumpedBits = 0;
			Ty_ jumpValue = 0;
			curNode->inJumpedBits<Ty_>(bs, jumpedBits, jumpValue);

			// bMax_ == bMin_ as child order changed -> just use bMax_
			bit_cnt_type tySize = sizeof(Ty_) * CHAR_BIT;
			bit_cnt_type prefixSize = sizeof(Ty_) * CHAR_BIT - curNode->bMax_ + 1;
			bit_cnt_type postfixSize = curNode->bMax_ - jumpedBits - 1;
			Ty_ jumpMask = (~((Ty_)-1 << jumpedBits)) << postfixSize;

			Ty_ prefixMask = ((~jumpMask) >> postfixSize) << postfixSize;
			Ty_ postfixMask = ~((Ty_)-1 << postfixSize);

			Ty_ min_ = curNode->getMin<Ty_>();
			Ty_ max_ = curNode->getMax<Ty_>();

			Ty_ realMin_ = curNode->getRealMin<Ty_>();
			Ty_ realMax_ = curNode->getRealMax<Ty_>();

			min_ = (Ty_)((prefixMask & min_) | (jumpMask & jumpValue));
			max_ = (Ty_)((prefixMask & max_) | (jumpMask & jumpValue));

			if(curNode->childOrder_)
			{
				if(max_ >= 0)
				{
					Ty_ childOrderMax_ = getMaxBoundary<Ty_>(max_, curNode->childOrder_, curNode->bMax_ - jumpedBits);
					curNode->min_ = min_;
					curNode->max_ = childOrderMax_;
				}else
				{
					Ty_ childOrderMin_ = getMinBoundary<Ty_>(min_, curNode->childOrder_, curNode->bMin_ + jumpedBits);
					curNode->min_ = childOrderMin_;
					curNode->max_ = max_;
				}
			}else
			{
				curNode->min_ = min_;
				curNode->max_ = max_;
			}
		} else
		{
			curNode->min_ = getMinBoundary<Ty_>(prevNode->getMin<Ty_>(), curNode->order_, curNode->bMin_);
			curNode->max_ = getMaxBoundary<Ty_>(prevNode->getMax<Ty_>(), curNode->order_, curNode->bMax_);
		}

		curNode->realMin_ = curNode->getMin<Ty_>();
		curNode->realMax_ = curNode->getMax<Ty_>();
		//////////////////////////////
	}

protected:
	//////////////////////////////
	// UTILS					//
	//////////////////////////////
	// Calculate number of chunk on each dimension using the dimension sizes and chunk sizes
	// If any dimension cannot be merged, stops calculating node spaces.
	void initNodeSpace()
	{
		this->nodeSpace_.clear();
		dimension nodeSpace = (this->dims_ / this->blockDims_) * pow(2, this->lowerLevel_);
		size_type level = 0;

		while(true)
		{
			this->nodeSpace_.push_back(nodeSpace);
			for(dimensionId d = 0; d < this->dSize_; ++d)
			{
				if(nodeSpace[d] <= 1)
				{
					// nodes cannot be merged farther.
					this->maxLevel_ = level;
					return;
				}
			}
			nodeSpace /= 2;
			++level;
		}
	}

	void findChunkBlockLevel()
	{
		this->blockLevel_ = 0;
		this->chunkLevel_ = 0;
		dimension blockSpace = this->dims_ / this->blockDims_;
		dimension chunkSpace = this->dims_ / this->chunkDims_;

		for (size_type level = 0; level <= this->maxLevel_; ++level)
		{
			auto nSpace = this->getNodeSpace(level);
			if (blockSpace.area() == nSpace.area())
			{
				this->blockLevel_ = level;
			}
			if (chunkSpace.area() == nSpace.area())
			{
				this->chunkLevel_ = level;
				return;
			}
		}
	}

	_NODISCARD coordinate<Dty_> getParentCoor(coordinate<Dty_>& childCoor)
	{
		coordinate<Dty_> coorParent = childCoor;
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			coorParent[d] /= 2;
		}
		return coorParent;
	}

	inline bool isChildOrderChanged(pMmtNode curNode)
	{
		return curNode->bMax_ == curNode->bMin_;	// if bMax_ == bMin_, move on to the next most significant nit
	}

	inline bool isFinished(pMmtNode prevNode)
	{
		return (prevNode->childOrder_ == 0) || ((bit_cnt_type)prevNode->order_ + 1 > TyBits_);
	}

	inline Ty_ preprocessingForValue(Ty_ value)
	{
		// To solve 'minimum negative value problem'
		if((Ty_)abs_(value) < 0)
		{
			return value + 1;
		}

		return value;
	}

public:
	// chunkCoor: coordinate of a chunk that contains the block
	// blockCoor: blockCoor coordiante of blocks in a chunk
	_NODISCARD coordinate<Dty_> getBlockCoor(coordinate<Dty_>& chunkCoor, coordinate<Dty_>& inner)
	{
		coordinate<Dty_> blockCoor(inner);
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			blockCoor[d] += chunkCoor[d] * this->blockSpace_[d];
		}
		return blockCoor;
	}

	coorRange getBlockItemBoundary(coordinate<Dty_>& inner)
	{
		coordinate<Dty_> spOut(this->dSize_), epOut(this->dSize_);
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			spOut[d] = this->blockDims_[d] * inner[d];
			epOut[d] = this->blockDims_[d] * (inner[d] + 1);
		}
		return coorRange(spOut, epOut);
	}

	_NODISCARD dimension getLeafDim() const
	{
		return this->leafDims_;
	}
	_NODISCARD size_type getBlockLevel() const
	{
		return this->blockLevel_;
	}
	_NODISCARD size_type getChunkLevel() const
	{
		return this->chunkLevel_;
	}
	
public:
	// For test
	std::vector<std::vector<pMmtNode>> getNodes()
	{
		return this->nodes_;
	}

	pMmtNode getNode(blockId nodeId, size_type level = 0)
	{
		return this->nodes_[level][nodeId];
	}

	pMmtNode getNode(coor& nodeCoor, size_type level = 0)
	{
		auto nit = this->getNodeIterator(level);
		return this->nodes_[level][nit.coorToSeq(nodeCoor)];
	};

	pMmtNode getNode(coor& chunkCoor, coor& inner, size_type level = 0)
	{
		auto nit = this->getNodeIterator(level);
		coor nodeCoor(inner);
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			nodeCoor[d] += chunkCoor[d] * this->blockSpace_[d];
		}
		return this->nodes_[level][nit.coorToSeq(nodeCoor)];
	}

	// qRange: query range
	pMmtNode getNode(const coorRange qRange)
	{
		auto rangeDim = qRange.width();
		coor blockDim(this->blockDims_);

		for (size_type level = 0; level <= this->maxLevel_; ++level)
		{
			if (rangeDim > blockDim)
			{
				blockDim /= 2;
				continue;
			}

			auto nodeCoor = qRange.getSp();
			nodeCoor /= blockDim;

			auto nit = this->getNodeIterator(level);
			return this->nodes_[level][nit.coorToSeq(nodeCoor)];
		}

		return nullptr;
	};

	void setNode(pMmtNode node, coor& nodeCoor, size_type level = 0)
	{
		auto nit = this->getNodeIterator(level);
		this->nodes_[level][nit.coorToSeq(nodeCoor)] = node;
	}

	void setNode(pMmtNode node, coor& chunkCoor, coor& blockCoor, coor& inner, size_t level = 0)
	{
		auto nit = this->getNodeIterator(level);
		dimension leafSpace = this->blockDims_ / this->leafDims_;
		coor nodeCoor(inner);
		nodeCoor += (blockCoor * leafSpace) + (chunkCoor * this->blockSpace_ * leafSpace);

		//BOOST_LOG_TRIVIAL(trace) << "setNode at level: " << static_cast<int64_t>(level) << ", chunkCoor: " << chunkCoor.toString() << ", blockCoor: " << blockCoor.toString()
		//	<< ", inner: " << inner.toString() << ", nodeCoor: " << nodeCoor.toString() << ", seq: " << nit.coorToSeq(nodeCoor);

		this->nodes_[level][nit.coorToSeq(nodeCoor)] = node;
		node->chunkCoor_ = chunkCoor;
		node->blockCoor_ = blockCoor;
		node->nodeCoor_ = nodeCoor;
		node->seqPos_ = nit.coorToSeq(nodeCoor);
	}

	dimension getNodeSpace(size_type level)
	{
		return this->nodeSpace_[level];
	}

	dimension getBlockDims()
	{
		return this->blockDims_;
	}

	void print()
	{
		for(size_type level = 0; level < this->getMaxLevel(); ++level)
		{
			BOOST_LOG_TRIVIAL(debug) << "==============================\n" << "Level: " << level;

			auto levelNodes = this->nodes_[level];
			int nodeSize = nodeSpace_[level].area();

			for(int i = 0; i < nodeSize; ++i)
			{
				auto node = levelNodes[i];
				node->print<Ty_>();
			}
		}
	}
private:
	const size_type TySize_;
	const size_type TyBits_;

private:
	size_type dSize_;
	dimension dims_;					// dimensions
	dimension chunkDims_;				
	dimension blockDims_;				// leaf block dimension (num of items)
	dimension leafDims_;				// leaf dim
	dimension blockSpace_;
	std::vector<dimension> nodeSpace_;	// level dim (num of blocks)
	std::vector<std::vector<pMmtNode>> nodes_;	// mmt
	size_type blockLevel_;
	size_type chunkLevel_;
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
}

#endif		// _MSDB_MMT_H_