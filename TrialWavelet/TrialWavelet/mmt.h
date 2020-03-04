#pragma once
#ifndef _caMMT_
#define _caMMT_

#include <iostream>
#include <vector>
#include <type_traits>
#include "iterators.h"
#include "bitstringstream.h"

namespace caWavelet
{
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
		using dim_vector_const_pointer = const std::vector<Dty_>*;

		const size_type TySize_ = sizeof(Ty_);
		const size_type TyBits_ = sizeof(Ty_) * CHAR_BIT;

		class mmtNode
		{
		public:
			Ty_ max;
			Ty_ min;
			char bMax;					// significant bit of max value
			char bMin;					// significant bit of min value
			unsigned char bMaxDelta;	// bMax delta from a parent node
			unsigned char bMinDelta;	// bMin delta from a parent node
			unsigned char order;		// n th significant bit
			unsigned char bits;			// required bits to represent min/max value

		public:
			mmtNode() : max(0), min(0), bMax(0), bMin(0), bits(0x80), order(0), bMaxDelta(0), bMinDelta(0) {}

			_NODISCARD const Ty_ delta() const noexcept
			{
				return this->max - this->min;
			}
		};

	public:
		caMMT()
		{

		}

		~caMMT()
		{
			auto it = this->nodes.begin();
			while (it != this->nodes.end())
			{
				if (!(*it))
				{
					delete[] (*it);
				}
				it++;
			}
		}

		void buildMMT(value_pointer data, size_const length,
			dim_vector_pointer dims, dim_vector_pointer chunkDims, size_const maxLevel)
		{
			this->length = length;
			this->dims_ = *dims;

			dim_vector chunkNum;
			this->calcChunkNum(&chunkNum, dims, chunkDims);

			// Build MMT from a 0 level.
			this->buildLeafMMT(data, length, chunkDims, &chunkNum);
			for (size_type level = 1; level <= maxLevel; level++)
			{
				this->buildIntermediateMMT(level, chunkDims, &chunkNum);
			}
		}

		void serialize(value_const_pointer* output, dim_vector_pointer chunkDims,
			dim_vector_pointer chunkNum)
		{
			bstream bs;

			auto it = this->nodes.begin();

			while (it != this->nodes.end())
			{
				auto curLevelNodes = (*it);
			}
		}

	protected:
		// For level 0
		void buildLeafMMT(value_pointer data, size_const length, dim_vector_pointer chunkDims,
			dim_vector_pointer chunkNum)
		{
			size_type dimSize = this->dims_.size();
			this->nodes.push_back(new mmtNode[this->calcLength(chunkNum)]);

			caCoorIterator<Dty_, Ty_> it(data, dimSize, this->dims_.data());
			caCoorIterator<Dty_, mmtNode> cit(this->nodes[0], dimSize, chunkNum->data());

			for (size_type i = 0; i < length; i++)
			{
				// current iterator coordiate -> chunk coordinate
				caCoor<Dty_> cur = it;
				for (size_type d = 0; d < dimSize; d++)
				{
					cur[d] /= chunkDims->at(d);
				}

				// get target chunk
				cit.moveTo(cur);
				mmtNode* node = &(*cit);

				// init min, max value
				if (node->bits == 0x80)
				{
					node->max = *it;
					node->min = *it;
					node->bMax = msb<Ty_>(node->max);
					node->bMin = msb<Ty_>(node->min);
					node->bits = TyBits_;
				}
				else
				{
					// compare min max value
					if (*it > node->max)
					{
						node->max = *it;
						node->bMax = msb<Ty_>(node->max);
					}
					if (*it < node->min)
					{
						node->min = *it;
						node->bMin = msb<Ty_>(node->min);
					}
				}

				// move to next value
				it++;
			}
		}

		// Except level 0, (1 ~ maxLevel)
		void buildIntermediateMMT(const size_type level, dim_vector_pointer chunkDims, dim_vector_pointer chunkNum)
		{
			assert(level > 0);

			size_type prevLevel = level - 1;
			const size_type dimSize = this->dims_.size();

			////////////////////////////////////////
			// Calc chunk num of prev and current level
			//
			// Notes: In this method, prev is a lower level nodes (finer)
			dim_vector prevLevelChunkNum(*chunkNum);		// Copy chunkNum vector
			dim_vector curLevelChunkNum(dimSize);			// Init with 0s

			for (size_type d = 0; d < dimSize; d++)
			{
				prevLevelChunkNum[d] /= pow(2, prevLevel);
				curLevelChunkNum[d] = prevLevelChunkNum[d] / 2;
			}

			////////////////////////////////////////
			// Create new mmtNodes
			const size_type prevLength = this->calcLength(&prevLevelChunkNum);
			this->nodes.push_back(new mmtNode[prevLength / pow(2, dimSize)]);


			////////////////////////////////////////
			// Update min/max values
			caCoorIterator<Dty_, mmtNode> pcit(this->nodes[prevLevel], dimSize,
				prevLevelChunkNum.data());
			caCoorIterator<Dty_, mmtNode> cit(this->nodes[level], dimSize,
				curLevelChunkNum.data());

			for (size_type i = 0; i < prevLength; i++)
			{
				// current iterator coordiate -> parent coordinate
				caCoor<Dty_> cur = pcit;
				for (size_type d = 0; d < dimSize; d++)
				{
					cur[d] /= 2;
				}

				// get target chunk
				cit.moveTo(cur);
				mmtNode* node = &(*cit);

				// init min, max value
				if (node->bits == 0x80)
				{
					node->max = (*pcit).max;
					node->bMax = (*pcit).bMax;
					node->min = (*pcit).min;
					node->bMax = (*pcit).bMin;
					node->bits = TyBits_;
				}
				else
				{
					// compare min max value
					if ((*pcit).max > node->max)
					{
						node->max = (*pcit).max;
						node->bMax = (*pcit).bMax;
					}
					if ((*pcit).min < node->min)
					{
						node->min = (*pcit).min;
						node->bMax = (*pcit).bMin;
					}
				}

				// move to next chunk
				++pcit;
			}
		}

		// For max level nodes
		void serializeMaxLevel(bstream& bs, dim_vector_pointer chunkDims, dim_vector_pointer chunkNum)
		{
			const size_type level = this->nodes.size();
			dim_vector curLevelChunkNum(*chunkNum);		// Copy chunkNum vector

			for (size_type d = 0; d < dimSize; d++)
			{
				curLevelChunkNum[d] /= pow(2, level);
			}

			const size_type length = this->calcLength(&curLevelChunkNum);
			const size_type bitLength = sizeof(Ty_);
			mmtNode* curLevelNodes = nodes.back();

			bs.bits(sizeof(Ty_) * CHAR_BIT);
			for (size_type i = 0; i < length; i++)
			{
				bs << curLevelNodes[i].max << curLevelNodes[i].min;
			}
		}

		// Except max level nodes (0 ~ (maxLevel - 1))
		void serializeLevelNodes(bstream& bs, const size_type level, dim_vector_pointer chunkDims, dim_vector_pointer chunkNum)
		{
			assert(level + 1 < this->nodes.size());		// Not for max level nodes

			size_type pLevel = level - 1;
			size_type siblings = pow(2, this->dims_.size());
			const size_type dimSize = this->dims_.size();

			////////////////////////////////////////
			// Calc chunk num of prev and current level
			//
			// Notes: In this method, prev is a upper level nodes (coarse)
			dim_vector pChunkNum(dimSize);			// Init with 0s
			dim_vector cChunkNum(*chunkNum);			// Copy chunkNum vector

			for (size_type d = 0; d < dimSize; d++)
			{
				cChunkNum[d] /= pow(2, level);
				pChunkNum[d] = cChunkNum[d] / 2;
			}

			////////////////////////////////////////
			// Update bit order for chunks in current level
			caCoorIterator<Dty_, mmtNode> cit(this->nodes[level], dimSize,
				cChunkNum.data());
			caCoorIterator<Dty_, mmtNode> pcit(this->nodes[level + 1], dimSize,
				pChunkNum.data());

			const size_type prevLength = this->calcLength(&prevLevelChunkNum);

			for (size_type i = 0; i < prevLength; i++)
			{
				caCoor<Dty_> childBase = pcit;
				for (size_type d = 0; d < dimSize; d++)
				{
					childBase[d] *= 2;
				}

				mmtNode* pNode = &(*pcit);
				size_type bits = pNode->bMax - pNode->bMin;
				size_type order = pNode->order + 1;

				for (size_type cID = 0; cID < siblings; cID++)
				{
					// Set child coordinate and move to it
					caCoor<Dty_> cur = childBase;
					for (size_type d = 0; d < dimSize; d++)
					{
						if (cID & (0x1 >> d))
						{
							cur[d] += 1;
						}
					}
					cit.moveTo(cur);

					// Update
					mmtNode* cNode = &(*cit);
					if (bits)
					{
						cNode->bits = pNode->bits;
						cNode->order = pNode->order;

						cNode->bMaxDelta = pNode->bMax - cNode->bMax;
						cNode->bMinDelta = cNode->bMin - pNode->bMin;
					}
					else
					{
						// Move to next significant bit
						if (pNode->order + 1 < TyBits_)
						{
							cNode->bits = msb<Ty_>(pNode->bMax);
							cNode->order = pNode->order + 1;

							cNode->bMaxDelta = cNode->bMax = msb<Ty_>(pNode->max, cNode->order);
							cNode->bMinDelta = cNode->bMin = msb<Ty_>(pNode->min, cNode->order);
						}
						else
						{
							cNode->bits = 0;
							cNode->order = pNode->order;
						}
					}
				}
			}
			
			////////////////////////////////////////
			// Write bits in an outstream
			mmtNode* cNodes = nodes[level];
			const size_type length = this->calcLength(&cChunkNum);
			for (size_type i = 0; i < length; i++)
			{
				bs << bits(cNodes[i].bits) << cNodes[i].max << cNodes[i].min;
			}
		}

		//////////////////////////////
		// UTILS					//
		//////////////////////////////
	protected:
		size_type calcLength(dim_vector_const_pointer dims)
		{
			size_type length = 1;
			for (size_type i = 0; i < dims->size(); i++)
			{
				length *= dims->at(i);
			}
			return length;
		}

		// Calculate number of chunk on each dimension using the dimension sizes and chunk sizes
		void calcChunkNum(dim_vector_pointer output,
			dim_vector_pointer dims, dim_vector_pointer chunkDims)
		{
			for (size_type d = 0; d < dims->size(); d++)
			{
				// ceiling
				size_type chunks = 1 + (dims->at(d) - 1) / chunkDims->at(d);

				if (output->size() <= d)
				{
					output->push_back(chunks);
				}
				else
				{
					output->at(d) = chunks;
				}
			}
		}

	private:
		size_type length;				// length of data
		std::vector<Dty_> dims_;		// dimensions
		std::vector<mmtNode*> nodes;	// mmt

	private:
		FRIEND_TEST(caMMT, buildLeafMMT);
		FRIEND_TEST(caMMT, buildIntermediateMMT);
		FRIEND_TEST(caMMT, buildMMT);
	};

	template<typename Ty_, typename size_type = std::conditional_t<sizeof(Ty_) < 32, unsigned char, unsigned int>, 
		size_t Bits_ = sizeof(Ty_) * CHAR_BIT>
	size_type msb(Ty_ value, size_type order = 1)
	{
		if (value == 0)
			return 0;

		Ty_ mask = 1 << (Bits_ - 1);
		size_type i = 0;
		while (!((mask & value) && --order == 0) && i++ < Bits_)
		{
			value <<= 1;
		}

		return Bits_ - i;
	}
}

#endif