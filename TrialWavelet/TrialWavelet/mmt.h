#pragma once
#ifndef _caMMT_
#define _caMMT_

#include <iostream>
#include <vector>
#include <type_traits>
#include "iterators.h"
#include "include/io/bitstringstream.h"
#include "array.h"

namespace msdb
{
#define SIGN(val) ((val >= 0) ? 1 : -1)
#ifdef GTEST_INCLUDE_GTEST_GTEST_H_
	namespace caDummy::data2D_sc8x8 { class caMMT_mmt_deserialize_sc8x8_Test; }
#endif
	using bit_cnt_type = unsigned char;
	using sig_bit_type = char;

	template<typename Ty_, typename size_type, size_t Bits_>
	size_type msb(Ty_, size_type);

	unsigned long long calcMaxLimit(bit_cnt_type bits);
	unsigned long long calcMinLimit(bit_cnt_type bits);

	template <typename Ty_>
	Ty_ getMaxBoundary(Ty_, bit_cnt_type, sig_bit_type);

	template <typename Ty_>
	Ty_ getMinBoundary(Ty_, bit_cnt_type, sig_bit_type);

	template <typename Ty_>
	bit_cnt_type getPrefixPosForPrevLimit(Ty_, bit_cnt_type);

	template <typename Dty_, typename Ty_>
	class caMMT
	{
	public:
		using size_type = size_t;
		using size_const = const size_t;

		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_reference = const Dty_;

		using value_type = Ty_;
		using value_pointer = Ty_*;
		using value_const_pointer = const Ty_*;
		using value_reference = Ty_&;
		using value_const_reference = const Ty_&;

		using dim_vector = std::vector<Dty_>;
		using dim_vector_pointer = std::vector<Dty_>*;
		using dim_vector_reference = std::vector<Dty_>&;
		using dim_vector_const_pointer = const std::vector<Dty_>*;
		using dim_vector_const_reference = std::vector<Dty_>&;

		const size_type TySize_ = sizeof(Ty_);
		const size_type TyBits_ = sizeof(Ty_) * CHAR_BIT;

		class mmtNode
		{
		public:
			Ty_ max;
			Ty_ min;
			sig_bit_type bMax;			// significant bit of max value
			sig_bit_type bMin;			// significant bit of min value
			bit_cnt_type bMaxDelta;		// bMax delta from a parent node
			bit_cnt_type bMinDelta;		// bMin delta from a parent node
			bit_cnt_type order;			// n th significant bit
			bit_cnt_type bits;			// required bits to represent min/max value

		public:
			mmtNode() : max(0), min(0), bMax(0), bMin(0), bits(0x80), order(1), bMaxDelta(0), bMinDelta(0) {}

			_NODISCARD const Ty_ delta() const noexcept
			{
				return this->max - this->min;
			}

			inline void copyMaxFrom(mmtNode* _right)
			{
				this->max = _right->max;
				this->bMax = _right->bMax;
			}

			inline void copyMinFrom(mmtNode* _right)
			{
				this->min = _right->min;
				this->bMin = _right->bMin;
			}

			inline void setMax(Ty_ max)
			{
				this->max = max;
				this->bMax = msb<Ty_>(std::abs(max)) * SIGN(max);
			}

			inline void setMin(Ty_ min)
			{
				this->min = min;
				this->bMin = msb<Ty_>(std::abs(min)) * SIGN(min);
			}
		};

	public:
		caMMT(dim_vector_reference dim, dim_vector_reference chunkDim, size_const maxLevel)
		{
			this->dim_ = dim;
			this->dSize_ = dim.size();
			this->leafChunkDim_ = chunkDim;
			this->maxLevel_ = maxLevel;
			this->initChunkInDim(dim, chunkDim, maxLevel);
		}

		~caMMT() {}

		void buildMMT(value_pointer data, size_const length)
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

		void serialize(value_const_pointer* output, size_const length)
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

		caMMT<Dty_, Ty_>::mmtNode* getNodes(size_type level)
		{
			assert(level < this->nodes_.size());
			return this->nodes_[level].data();
		}

	protected:
		//////////////////////////////
		// MMT Build Functions
		// For level 0
		void forwardBuildLeaf(value_pointer data, size_const length)
		{
			////////////////////////////////////////
			// Create new mmtNodes
			auto chunksInDim = this->chunksInDim_[0];
			size_type chunkCnt = calcArrayCellNums(chunksInDim.data(), chunksInDim.size());
			this->nodes_.push_back(std::vector<mmtNode>(chunkCnt));

			coorIterator<Dty_, Ty_> it(data, this->dSize_, this->dim_.data());
			coorIterator<Dty_, mmtNode> cit(this->nodes_[0].data(), this->dSize_, chunksInDim.data());

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
				mmtNode* node = &(*cit);

				// init min, max value
				if (node->bits == 0x80)
				{
					node->setMax(*it);
					node->setMin(*it);
					node->bits = (bit_cnt_type)TyBits_;
				} else
				{
					// compare min max value
					if (*it > node->max)
					{
						node->setMax(*it);
					}
					if (*it < node->min)
					{
						node->setMin(*it);
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
			this->nodes_.push_back(std::vector<mmtNode>(chunkCnt));

			////////////////////////////////////////
			// Update min/max values
			coorIterator<Dty_, mmtNode> pcit(this->nodes_[level - 1].data(), this->dSize_,
											   pChunksInDim.data());
			coorIterator<Dty_, mmtNode> cit(this->nodes_[level].data(), this->dSize_,
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
				mmtNode* node = &(*cit);

				// init min, max value
				if (node->bits == 0x80)
				{
					node->copyMaxFrom(&(*pcit));
					node->copyMinFrom(&(*pcit));
					node->bits = (bit_cnt_type)TyBits_;
				} else
				{
					// compare min max value
					if ((*pcit).max > node->max)
					{
						node->copyMaxFrom(&(*pcit));
					}
					if ((*pcit).min < node->min)
					{
						node->copyMinFrom(&(*pcit));
					}
				}

				// move to next chunk
				++pcit;
			}
		}

		// For max level nodes
		void backwardBuildRoot()
		{
			mmtNode* curLevel = this->nodes_.back().data();

			for (size_type i = 0; i < this->nodes_.back().size(); i++)
			{
				curLevel[i].bits = msb<Ty_>(curLevel[i].max - curLevel[i].min);
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
			coorIterator<Dty_, mmtNode> pcit(this->nodes_[level + 1].data(), this->dSize_,
											   pChunksInDim.data());

										   // Current
			coorIterator<Dty_, mmtNode> cit(this->nodes_[level].data(), this->dSize_,
											  chunksInDim.data());

			for (size_type i = 0; i < this->nodes_[level + 1].size(); ++i, ++pcit)
			{
				coordinate<Dty_> childBase = pcit;
				for (size_type d = 0; d < this->dSize_; d++)
				{
					childBase[d] *= 2;
				}

				mmtNode* pNode = &(*pcit);
				bool sameOrder = (bool)(pNode->bMax - pNode->bMin);	// if bMax == bMin, move on to the next most significant bit
				bit_cnt_type bits = sameOrder ? msb<Ty_>(pNode->bMax - pNode->bMin) : msb<Ty_>(std::abs(pNode->bMax) - 1);

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
					mmtNode* cNode = &(*cit);
					if (sameOrder)
					{
						cNode->bits = bits;
						cNode->order = pNode->order;

						cNode->bMaxDelta = static_cast<bit_cnt_type>(pNode->bMax - cNode->bMax);	// max: prev >= cur
						cNode->bMinDelta = static_cast<bit_cnt_type>(cNode->bMin - pNode->bMin);	// min: prev <= cur
					} else
					{
						if ((bit_cnt_type)pNode->order + 1 < TyBits_)
						{
							// Move to next significant bit
							cNode->bits = bits;
							cNode->order = pNode->order + 1;

							cNode->bMax = msb<Ty_>(std::abs(cNode->max), cNode->order) * SIGN(cNode->max);
							cNode->bMin = msb<Ty_>(std::abs(cNode->min), cNode->order) * SIGN(cNode->min);

							cNode->bMaxDelta = std::max(std::abs(pNode->bMax - cNode->bMax) - 1, 0);
							cNode->bMinDelta = std::max(std::abs(cNode->bMin - pNode->bMin) - 1, 0);
						} else
						{
							// The last bit. Has no more next significant bit
							cNode->bits = 0;
							cNode->order = pNode->order;
						}
					}
				}
			}
		}

		////////////////////////////////////////
		// Write bits in an outstream
		void serializeRoot(bstream& bs)
		{
			mmtNode* curLevelNodes = this->nodes_.back().data();

			for (size_type i = 0; i < this->nodes_.back().size(); i++)
			{
				bs << setw(TyBits_) << curLevelNodes[i].max << curLevelNodes[i].min;
			}
		}

		void serializeNonRoot(bstream& bs, size_type level)
		{
			mmtNode* cNodes = this->nodes_[level].data();
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
				this->nodes_.push_back(std::vector<mmtNode>());
			}

			////////////////////////////////////////
			// Create new mmtNodes
			auto chunksInDim = this->chunksInDim_[this->maxLevel_];
			size_type chunkCnt = calcArrayCellNums(chunksInDim.data(), chunksInDim.size());
			this->nodes_[this->maxLevel_].resize(chunkCnt);	// TODO::If generating Nodes are complete, remove this line.
			mmtNode* rootNodes = this->nodes_[this->maxLevel_].data();

			for (size_type i = 0; i < chunkCnt; i++)
			{
				Ty_ max, min;
				bs >> setw(TyBits_) >> max >> min;
				rootNodes[i].setMax(max);
				rootNodes[i].setMin(min);
				rootNodes[i].bits = msb<Ty_>(max - min);
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
			coorIterator<Dty_, mmtNode> pcit(this->nodes_[level + 1].data(), this->dSize_,
											   pChunksInDim.data());
			// Current
			coorIterator<Dty_, mmtNode> cit(this->nodes_[level].data(), this->dSize_,
											  chunksInDim.data());

			for (size_type i = 0; i < chunkCnt; i++)
			{
				mmtNode* cNode = &(*cit);
				pcit.moveTo(this->getParentCoor(cit));
				mmtNode* pNode = &(*pcit);

				bool sameOrder = pNode->bMax != pNode->bMin;	// if bMax == bMin, move on to the next most significant bit
				cNode->bits = sameOrder ? msb<Ty_>(pNode->bMax - pNode->bMin) : msb<Ty_>(std::abs(pNode->bMax) - 1);

				bs >> setw(cNode->bits) >> cNode->bMaxDelta >> cNode->bMinDelta;

				if (sameOrder)
				{
					cNode->order = pNode->order;
					cNode->bMax = pNode->bMax - cNode->bMaxDelta;
					cNode->bMin = pNode->bMin + cNode->bMinDelta;

					if (cNode->order == 1)
					{
						cNode->max = getMaxBoundary<Ty_>(cNode->bMax);
						cNode->min = getMinBoundary<Ty_>(cNode->bMin);
					} else
					{
						cNode->max = getMaxBoundary(pNode->max, cNode->order, cNode->bMax);
						cNode->min = getMinBoundary(pNode->min, cNode->order, cNode->bMin);
					}
				} else
				{
					if ((size_type)pNode->order + 1 < TyBits_)
					{
						// Move to next significant bit
						cNode->order = pNode->order + 1;
						cNode->bMax = pNode->bMax - cNode->bMaxDelta - 1;
						cNode->bMin = pNode->bMin - cNode->bMinDelta - 1;

						cNode->max = getMaxBoundary(pNode->max, cNode->order, cNode->bMax);
						cNode->min = getMinBoundary(pNode->min, cNode->order, cNode->bMin);
					} else
					{
						cNode->bits = 0;
						cNode->order = pNode->order;
						cNode->copyMaxFrom(pNode);
						cNode->copyMinFrom(pNode);
					}
				}

				// Fill out min, max values.
				if (cNode->order == 1)
				{
					cNode->max = getMaxBoundary<Ty_>(cNode->bMax);
					cNode->min = getMinBoundary<Ty_>(cNode->bMin);
				} else
				{
					if (cNode->bits)
					{
						cNode->max = getMaxBoundary(pNode->max, cNode->order, cNode->bMax);
						cNode->min = getMinBoundary(pNode->min, cNode->order, cNode->bMin);
					}
				}

				cit++;	// Move to next data
			}

			auto a = 0;
		}

		//////////////////////////////
		// UTILS					//
		//////////////////////////////
	protected:
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
		size_type dSize_;
		size_type maxLevel_;
		dim_vector dim_;		// dimensions
		dim_vector leafChunkDim_;
		std::vector<dim_vector> chunksInDim_;
		std::vector<std::vector<mmtNode>> nodes_;	// mmt
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

	private:
#ifdef GTEST_INCLUDE_GTEST_GTEST_H_
		FRIEND_TEST(caMMT, buildLeafMMT);
		FRIEND_TEST(caMMT, buildIntermediateMMT);
		FRIEND_TEST(caMMT, buildMMT);
		FRIEND_TEST(caMMT, mmt_deserialize_sc8x8);
		FRIEND_TEST(caDummy::data2D_sc8x8::caMMT, mmt_deserialize_sc8x8);
#endif
	};

	// Return Max Limit value where num of bits is provided.
	template <typename Ty_>
	Ty_ getMaxBoundary(sig_bit_type bits)
	{
		if (bits == 0)
		{
			return 0;
		} else if (bits > 0)
		{
			return static_cast<Ty_>(calcMaxLimit(bits));
		} else
		{
			return static_cast<Ty_>(calcMinLimit(-bits) * -1);
		}
	}

	template <typename Ty_>
	Ty_ getMinBoundary(sig_bit_type bits)
	{
		if (bits == 0)
		{
			return 0;
		} else if (bits > 0)
		{
			return static_cast<Ty_>(calcMinLimit(bits));
		} else
		{
			return static_cast<Ty_>(calcMaxLimit(-bits) * -1);
		}
	}

	// Return Max Limit value where num of bits is provided.
	template <typename Ty_>
	Ty_ getMaxBoundary(Ty_ prevLimit, bit_cnt_type order, sig_bit_type sigBitPos)
	{
		assert(order > 1 && sigBitPos > 0);

		if (prevLimit == 0)
		{
			return 0;
		} else if (prevLimit > 0)
		{
			bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
			assert(prefixPos >= sigBitPos);

			return (prevLimit | ~static_cast<Ty_>(calcMaxLimit(prefixPos)))& calcMaxLimit(sigBitPos);
		} else
		{
			bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
			assert(prefixPos >= sigBitPos);

			return ((std::abs(prevLimit) | ~static_cast<Ty_>(calcMaxLimit(prefixPos))) & ((Ty_)1 << (sigBitPos - 1))) * -1;
		}
	}

	template <typename Ty_>
	Ty_ getMinBoundary(Ty_ prevLimit, bit_cnt_type order, sig_bit_type sigBitPos)
	{
		assert(order > 1 && sigBitPos > 0);

		if (prevLimit == 0)
		{
			return 0;
		} else if (prevLimit > 0)
		{
			bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
			assert(prefixPos >= sigBitPos);

			return (prevLimit | ~static_cast<Ty_>(calcMaxLimit(prefixPos)))& ((Ty_)1 << (sigBitPos - 1));
		} else
		{
			bit_cnt_type prefixPos = getPrefixPosForPrevLimit(prevLimit, order);
			assert(prefixPos >= sigBitPos);

			return ((std::abs(prevLimit) | ~static_cast<Ty_>(calcMaxLimit(prefixPos)))& calcMaxLimit(sigBitPos)) * -1;
		}
	}

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

	template<typename Ty_, typename size_type = std::conditional_t<sizeof(Ty_) < 32, unsigned char, unsigned int > ,
		size_t Bits_ = sizeof(Ty_)* CHAR_BIT>
		size_type msb(Ty_ value, size_type order = 1)
	{
		assert(order > 0);

		if (value == 0)
			return 0;

		Ty_ mask = 1 << (Bits_ - 1);
		size_type i = Bits_ + 1;
		while (order != 0 && i-- != 0)
		{
			(mask & value) && --order;
			value <<= 1;
		}

		return i;
	}
}

#endif