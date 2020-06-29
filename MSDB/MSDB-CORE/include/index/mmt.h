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
class MinMaxTree;
using mmt = MinMaxTree;
using pMMT = std::shared_ptr<mmt>;

template <typename Ty_>
bit_cnt_type getPrefixPosForPrevLimit(Ty_, bit_cnt_type);

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
			std::cout << this->version_ << ", " << this->size_ << ", " << static_cast<int>(this->eType_) << ", " << this->maxLevel_ << std::endl;
			
			int eTypeOut = static_cast<int>(this->eType_);
			os.write((char*)(&this->version_), sizeof(this->version_));
			os.write((char*)(&this->size_), sizeof(this->size_));
			os.write((char*)(&eTypeOut), sizeof(int));
			os.write((char*)(&this->maxLevel_), sizeof(this->maxLevel_));
			//os << this->version_ << this->size_ << static_cast<int>(this->eType_) << this->maxLevel_;
			// operator<< outputs the values in ASCII CODE (TEXT).
		}
		virtual void deserialize(std::istream& is) override
		{
			std::cout << "Header deserialize" << std::endl;

			int eTypeIn;
			is.read((char*)(&this->version_), sizeof(this->version_));
			is.read((char*)(&this->size_), sizeof(this->size_));
			is.read((char*)(&eTypeIn), sizeof(int));			
			is.read((char*)(&this->maxLevel_), sizeof(this->maxLevel_));
			this->eType_ = static_cast<eleType>(eTypeIn);

			//is >> this->version_ >> this->size_ >> eTypeIn >> maxLevel_;
			std::cout << this->version_ << ", " << this->size_ << ", " << static_cast<int>(this->eType_) << ", " << this->maxLevel_ << std::endl;
		}

	public:
		//eleType dType_;	// dimension type, not used, fixed now.
		eleType eType_;		// element type
		size_type maxLevel_;
	};

public:
	MinMaxTree(eleType eType);

public:
	virtual void build(chunkIterator& it) = 0;
	virtual void serialize(std::ostream& os) = 0;
	virtual void deserialize(std::istream& is) = 0;

public:
	template <typename Dty_>
	static pMMT createMMT(eleType type, std::vector<Dty_>& dim, std::vector<Dty_>& blockDim,
						  size_const maxLevel = 0);
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
		bit_cnt_type bMaxDelta_;	// bMax_ delta from a parent node
		bit_cnt_type bMinDelta_;	// bMin_ delta from a parent node
		bit_cnt_type order_;		// n th significant nit
		bit_cnt_type bits_;			// required bits to represent min/max value
		Ty_ max_;
		Ty_ min_;

	public:
		mmtNode() : bMax_(0), bMin_(0), bits_(0x80), order_(1),
			bMaxDelta_(0), bMinDelta_(0), max_(0), min_(0)
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

		inline void inMinMax(bstream& bs)
		{
			bs >> setw(sizeof(Ty_) * CHAR_BIT) >> this->max_ >> this->min_;
		}

		inline void outMinMax(bstream& bs)
		{
			bs << setw(sizeof(Ty_) * CHAR_BIT);
			bs << max_ << min_;
		}

		inline void initBits()
		{
			this->bits_ = sizeof(Ty_) * CHAR_BIT;
			this->order_ = 1;
			this->setMinMaxBits();
		}

		inline void setMinMaxBits()
		{
			this->bMax_ = msb<Ty_>(abs_(max_), this->order_) * SIGN(max_);
			this->bMin_ = msb<Ty_>(abs_(min_), this->order_) * SIGN(min_);
		}
	};

protected:
	// inherit from serializable
	virtual void updateToHeader() override
	{
		auto curHeader = std::static_pointer_cast<mmtHeader>(this->getHeader());
		curHeader->version_ = MinMaxTree::mmtHeader::mmt_header_version;
		curHeader->size_ = this->serializedSize_;
		curHeader->maxLevel_ = this->maxLevel_;
		curHeader->eType_ = this->eType_;
	}

	// inherit from serializable
	virtual void updateFromHeader() override
	{
		auto curHeader = std::static_pointer_cast<mmtHeader>(this->getHeader());
		this->maxLevel_ = curHeader->maxLevel_;
		this->serializedSize_ = curHeader->size_;
		this->eType_ = curHeader->eType_;

		this->initLevelDims(this->dim_, this->leafBlockDim_, this->maxLevel_);
	}

	//////////////////////////////
	// MMT Constructor			//
	//////////////////////////////
public:
	MinMaxTreeImpl(eleType eType, dim_vector_reference dim, dim_vector_reference blockDim,
				   size_const maxLevel = 0)
		: MinMaxTree(eType), dim_(dim), dSize_(dim.size()), leafBlockDim_(blockDim),
		maxLevel_(maxLevel), serializedSize_(0), eType_(eType),
		TySize_(getEleSize(eType)), TyBits_(getEleSize(eType)* CHAR_BIT)
	{
		this->initLevelDims(dim, blockDim, maxLevel);
	}

	~MinMaxTreeImpl() {}

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
	//
	//	if (length < (bs.size() + CHAR_BIT - 1) / CHAR_BIT)
	//	{
	//		throw std::length_error();
	//	}
	//
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
			this->nodes_.push_back(std::vector<pNode>());
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

	//const pNode getNodes(size_type level)
	//{
	//	assert(level < this->nodes_.size());
	//	return this->nodes_[level].data();
	//}

	//const dim_vector& getChunkInDim(size_type level)
	//{
	//	assert(level <= this->levelBlockDims_.size());
	//	return this->levelBlockDims_[level];
	//}

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
		auto levelDims = this->levelDims_[0];				// get Level 0 block dims
		auto chunkDims = (*it)->getDesc()->dims_;				// get chunk dimension

		dimension blockDims(this->dSize_);
		dimension levelDimsInChunk(this->dSize_);				// block container in a chunk
		for (dimensionId d = 0; d < this->dSize_; ++d)
		{
			blockDims[d] = this->dim_[d] / levelDims[d];
			levelDimsInChunk[d] = chunkDims[d] / blockDims[d];
		}

		size_type blockCnt = calcNumItems(levelDims.data(), levelDims.size());	// block numbers
		this->nodes_.push_back(std::vector<pNode>(blockCnt));					// make new node

		// Node iterator for all level 0 nodes
		// Use to convert block coordinate to seqencial node index
		nodeItr nit(this->nodes_[0].data(), this->dSize_, levelDims.data());

		while (!it.isEnd())
		{
			// Setup a start point of blockCoor for blocks in a chunk
			coor blockCoorSp = this->chunkCoorToBlockCoor(it.coor(), levelDimsInChunk);
			coorItr bit(this->dSize_, levelDimsInChunk.data());
			while (!bit.isEnd())
			{
				// Set iterator range according the block size
				coor blockCoorInChunk = bit.coor(), blockSpInChunk(this->dSize_), blockEpInChunk(this->dSize_);
				for (dimensionId d = 0; d < this->dSize_; ++d)
				{
					blockSpInChunk[d] = blockDims[d] * blockCoorInChunk[d];
					blockEpInChunk[d] = blockDims[d] * (blockCoorInChunk[d] + 1);
				}

				// TODO :: Reduce in one line
				auto iit = (*it)->getItemRangeIterator(blockSpInChunk.data(), blockEpInChunk.data());
				auto lNode = this->forwardBuildLeafNode(iit);

				coor blockCoor(this->dSize_, blockCoorSp.data());
				for (dimensionId d = 0; d < this->dSize_; ++d)
				{
					blockCoor[d] += blockCoorInChunk[d];
				}

				//std::cout << "leaf forward-" << std::endl;
				//std::cout << "[" << blockCoor[0] << ", " << blockCoor[1] << "] : " << static_cast<int>(lNode->min_) << "~" << static_cast<int>(lNode->max_) << std::endl;

				this->nodes_[0][nit.coorToSeq(blockCoor)] = lNode;
				++bit;	// Move on a next block in the chunk
			}

			++it;	// Move on a next chunk
		}
	}

	pNode forwardBuildLeafNode(itemItr& iit)
	{
		pNode node = std::make_shared<mmtNode>();

		auto value = (*iit).get<Ty_>();
		//std::cout << "chunk: " << static_cast<int>(value) << ", ";

		node->max_ = value;
		node->min_ = value;
		node->bits_ = (bit_cnt_type)TyBits_;
		++iit;

		while (!iit.isEnd())
		{
			auto v = (*iit).get<Ty_>();
			//std::cout << static_cast<int>(v) << ", ";
			if (node->max_ < v)
			{
				node->max_ = v;
			}
			if (node->min_ > v)
			{
				node->min_ = v;
			}
			++iit;
		}

		//std::cout << std::endl << "----------" << std::endl;
		return node;
	}

	// For level 1 ~ maxLevel
	// Notes: In this method, prev is a lower level nodes (finer)
	void forwardBuildNonLeaf(const size_type level)
	{
		assert(level > 0);

		////////////////////////////////////////
		// Create new mmtNodes
		dim_vector prevLevelDim = this->levelDims_[level - 1];
		dim_vector levelDim = this->levelDims_[level];
		const size_type blockCnt = calcNumItems(levelDim.data(), levelDim.size());
		this->nodes_.push_back(std::vector<pNode>(blockCnt));

		for(size_t i = 0; i < blockCnt; ++i)
		{
			this->nodes_[level][i] = std::make_shared<mmtNode>();
		}

		////////////////////////////////////////
		// Update min/max values
		coorIterator<Dty_, pNode> pcit(this->nodes_[level - 1].data(), this->dSize_,
									   prevLevelDim.data());
		coorIterator<Dty_, pNode> cit(this->nodes_[level].data(), this->dSize_,
									  levelDim.data());

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
				node->max_ = (*pcit)->max_;
				node->min_ = (*pcit)->min_;
				node->bits_ = (bit_cnt_type)TyBits_;
			} else
			{
				// compare min max value
				if (node->max_ < (*pcit)->max_)
				{
					node->max_ = (*pcit)->max_;
				}
				if (node->min_ > (*pcit)->min_)
				{
					node->min_ = (*pcit)->min_;
				}
			}
			this->nodes_[level][cit.coorToSeq(cur)] = node;

			//std::cout << "forward-" << std::endl;
			//std::cout << "[" << cur[0] << ", " << cur[1] << "] : " << static_cast<int>(node->min_) << "~" << static_cast<int>(node->max_) << std::endl;

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
		}
	}

	// Except max level nodes ((maxLevel - 1) -> level 0 )
	void backwardBuildNonRoot(const size_type level)
	{
		assert(level < this->nodes_.size() - 1);		// Not for max level nodes
		assert(level != -1);

		size_type pLevel = level + 1;
		size_type childs = (size_type)pow(2, this->dim_.size());

		////////////////////////////////////////
		// Calc chunk num of prev and current level
		//
		// Notes: In this method, prev is a upper level nodes (coarse)
		dim_vector prevLevelDim = this->levelDims_[level + 1];
		dim_vector levelDim = this->levelDims_[level];

		////////////////////////////////////////
		// Update nit order for chunks in current level
		// Prev
		coorIterator<Dty_, pNode> pcit(this->nodes_[level + 1].data(), this->dSize_,
									   prevLevelDim.data());
		// Current
		coorIterator<Dty_, pNode> cit(this->nodes_[level].data(), this->dSize_,
									  levelDim.data());

		for (size_type i = 0; i < this->nodes_[level + 1].size(); ++i, ++pcit)
		{
			// Setup child Base
			coordinate<Dty_> childBase = pcit.coor();
			for (size_type d = 0; d < this->dSize_; d++)
			{
				childBase[d] *= 2;
			}

			pNode prevNode = (*pcit);
			bool orderChanged = (prevNode->bMax_ == prevNode->bMin_);	// if bMax_ == bMin_, move on to the next most significant nit
			bit_cnt_type bits = orderChanged ?
				msb<sig_bit_type>(abs_(prevNode->bMax_) - 1) :			// decreases a required bits
				msb<sig_bit_type>(prevNode->bMax_ - prevNode->bMin_);	// calculate a required bits from prev node
			bool isLastBit = (bit_cnt_type)prevNode->order_ + 1 >= TyBits_;

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
				pNode cNode = (*cit);
				if (orderChanged)
				{
					// Order changed
					if (isLastBit)
					{
						// The last bit. Has no more next significant bit
						cNode->bits_ = 0;
						cNode->order_ = prevNode->order_;
						cNode->bMax_ = 0;
						cNode->bMin_ = 0;
						cNode->bMaxDelta_ = 0;
						cNode->bMinDelta_ = 0;
					} else
					{
						// Move to next significant bit
						cNode->bits_ = bits;
						cNode->order_ = prevNode->order_ + 1;
						cNode->setMinMaxBits();
						cNode->bMaxDelta_ = std::max({ abs_(prevNode->bMax_ - cNode->bMax_) - 1, 0 });
						cNode->bMinDelta_ = std::max({ abs_(cNode->bMin_ - prevNode->bMin_) - 1, 0 });
						// TODO::Change min, max according order and deltaBits
					}
				} else
				{
					// Order not changed
					cNode->bits_ = bits;
					cNode->order_ = prevNode->order_;
					cNode->setMinMaxBits();
					cNode->bMaxDelta_ = static_cast<bit_cnt_type>(prevNode->bMax_ - cNode->bMax_);	// max: prev >= cur
					cNode->bMinDelta_ = static_cast<bit_cnt_type>(cNode->bMin_ - prevNode->bMin_);	// min: prev <= cur
					// TODO::Change min, max according order and deltaBits
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
		}
	}

	void serializeNonRoot(bstream& bs, size_type level)
	{
		pNode* curLevelNodes = this->nodes_[level].data();
		for (size_type i = 0; i < this->nodes_[level].size(); i++)
		{
			curLevelNodes[i]->outDelta(bs);
		}
	}

	////////////////////////////////////////
	// Build MMT from an instream
	void deserializeRoot(bstream& bs)
	{
		////////////////////////////////////////
		// Create new mmtNodes
		auto chunksInDim = this->levelDims_[this->maxLevel_];
		size_type chunkCnt = calcNumItems(chunksInDim.data(), chunksInDim.size());
		this->nodes_[this->maxLevel_].resize(chunkCnt);	// TODO::If generating Nodes are complete, remove this line.
		pNode* rootNodes = this->nodes_[this->maxLevel_].data();

		for (size_type i = 0; i < chunkCnt; i++)
		{
			rootNodes[i] = std::make_shared<mmtNode>();
			rootNodes[i]->inMinMax(bs);
			rootNodes[i]->initBits();
		}
	}

	void deserializeNonRoot(bstream& bs, size_type level)
	{
		// Calc chunk num of prev and current level
		dim_vector pChunksInDim = this->levelDims_[level + 1];
		dim_vector chunksInDim = this->levelDims_[level];
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
			pNode cNode = std::make_shared<mmtNode>();
			(*cit) = cNode;
			pcit.moveTo(this->getParentCoor(cit.coor()));
			pNode prevNode = (*pcit);

			// if bMax_ == bMin_, move on to the next most significant nit
			bool orderChanged = prevNode->bMax_ == prevNode->bMin_;	
			cNode->bits_ =
				orderChanged ?
				msb<sig_bit_type>(abs_(prevNode->bMax_) - 1) :
				msb<sig_bit_type>(prevNode->bMax_ - prevNode->bMin_);

			cNode->inDelta(bs);		// after set required bits, read delta data from bstream

			if (orderChanged)
			{
				if ((size_type)prevNode->order_ + 1 < TyBits_)
				{
					// Move to next significant bit
					cNode->order_ = prevNode->order_ + 1;
					cNode->bMax_ = prevNode->bMax_ - cNode->bMaxDelta_ - 1;
					cNode->bMin_ = prevNode->bMin_ - cNode->bMinDelta_ - 1;

					cNode->max_ = getMaxBoundary<Ty_>(prevNode->max_, cNode->order_, cNode->bMax_);
					cNode->min_ = getMinBoundary<Ty_>(prevNode->min_, cNode->order_, cNode->bMin_);
				} else
				{
					// order == TyBits_
					// No more detail
					cNode->bits_ = 0;
					cNode->order_ = prevNode->order_;
					cNode->max_ = prevNode->max_;
					cNode->min_ = prevNode->min_;
				}

			} else
			{
				cNode->order_ = prevNode->order_;
				cNode->bMax_ = prevNode->bMax_ - cNode->bMaxDelta_;
				cNode->bMin_ = prevNode->bMin_ + cNode->bMinDelta_;

				cNode->max_ = getMaxBoundary<Ty_>(prevNode->max_, cNode->order_, cNode->bMax_);
				cNode->min_ = getMinBoundary<Ty_>(prevNode->min_, cNode->order_, cNode->bMin_);
			}
			++cit;	// Move to next data
		}
	}

protected:
	//////////////////////////////
	// UTILS					//
	//////////////////////////////
	// Calculate number of chunk on each dimension using the dimension sizes and chunk sizes
	void initLevelDims(dim_vector_const_reference dims,
					   dim_vector_const_reference leafBlockDim,
					   size_type maxLevel)
	{
		this->levelDims_.clear();
		for (size_type level = 0; level <= maxLevel; level++)
		{
			this->levelDims_.push_back(calcChunkNums(dims.data(), leafBlockDim.data(), dims.size(), level));
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

	// For test
public:
	std::vector<std::vector<pNode>> getNodes()
	{
		return this->nodes_;
	}

	dim_vector getLevelDim(size_type level)
	{
		return this->levelDims_[level];
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
	std::vector<dim_vector> levelDims_;
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

template<typename Dty_>
inline pMMT MinMaxTree::createMMT(eleType type,
								  std::vector<Dty_>& dim, std::vector<Dty_>& blockDim,
								  size_const maxLevel)
{
	switch (type)
	{
	case eleType::CHAR:
		return std::make_shared<MinMaxTreeImpl<Dty_, char>>(type, dim, blockDim, maxLevel);
	case eleType::INT8:
		return std::make_shared<MinMaxTreeImpl<Dty_, int8_t>>(type, dim, blockDim, maxLevel);
	case eleType::INT16:
		return std::make_shared<MinMaxTreeImpl<Dty_, int16_t>>(type, dim, blockDim, maxLevel);
	case eleType::INT32:
		return std::make_shared<MinMaxTreeImpl<Dty_, int32_t>>(type, dim, blockDim, maxLevel);
	case eleType::INT64:
		return std::make_shared<MinMaxTreeImpl<Dty_, int64_t>>(type, dim, blockDim, maxLevel);
	case eleType::UINT8:
		return std::make_shared<MinMaxTreeImpl<Dty_, uint8_t>>(type, dim, blockDim, maxLevel);
	case eleType::UINT16:
		return std::make_shared<MinMaxTreeImpl<Dty_, uint16_t>>(type, dim, blockDim, maxLevel);
	case eleType::UINT32:
		return std::make_shared<MinMaxTreeImpl<Dty_, uint32_t>>(type, dim, blockDim, maxLevel);
	case eleType::UINT64:
		return std::make_shared<MinMaxTreeImpl<Dty_, uint64_t>>(type, dim, blockDim, maxLevel);
	}

	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_UNKNOWN_ERROR, MSDB_ER_NOT_IMPLEMENTED));
}
}

#endif		// _MSDB_MMT_H_