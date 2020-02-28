#pragma once
#ifndef _caMMT_
#define _caMMT_

#include <iostream>
#include <vector>
#include "iterators.h"

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

		class mmtNode
		{
		public:
			Ty_ max;
			Ty_ min;
			unsigned char order;

		public:
			mmtNode() : order(0x80) {}
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
				if (node->order == 0x80)
				{
					node->max = *it;
					node->min = *it;
					node->order = 0;
				}
				else
				{
					// compare min max value
					if (*it > node->max)
					{
						node->max = *it;
					}
					if (*it < node->min)
					{
						node->min = *it;
					}
				}

				// move to next value
				it++;
			}
		}

		// Except level 0, (1~maxLevel)
		void buildIntermediateMMT(const size_type level, dim_vector_pointer chunkDims, dim_vector_pointer chunkNum)
		{
			assert(level > 0);

			size_type prevLevel = level - 1;
			size_type dimSize = this->dims_.size();

			// calc chunk num of prev and current level
			dim_vector prevLevelChunkNum(*chunkNum);		// Copy chunkNum vector
			dim_vector curLevelChunkNum(dimSize);			// Init with 0s

			for (size_type d = 0; d < dimSize; d++)
			{
				prevLevelChunkNum[d] /= pow(2, prevLevel);
				curLevelChunkNum[d] = prevLevelChunkNum[d] / 2;
			}

			size_type prevLength = this->calcLength(&prevLevelChunkNum);

			this->nodes.push_back(new mmtNode[prevLength / pow(2, dimSize)]);

			caCoorIterator<Dty_, mmtNode> pcit(this->nodes[prevLevel], dimSize,
				prevLevelChunkNum.data());
			caCoorIterator<Dty_, mmtNode> cit(this->nodes[level], dimSize,
				curLevelChunkNum.data());

			for (size_type i = 0; i < prevLength; i++)
			{
				// current iterator coordiate -> chunk coordinate
				caCoor<Dty_> cur = pcit;
				for (size_type d = 0; d < dimSize; d++)
				{
					cur[d] /= 2;
				}

				// get target chunk
				cit.moveTo(cur);
				mmtNode* node = &(*cit);

				// init min, max value
				if (node->order == 0x80)
				{
					node->max = (*pcit).max;
					node->min = (*pcit).min;
					node->order = 0;			// TODO::order
				}
				else
				{
					// compare min max value
					if ((*pcit).max > node->max)
					{
						node->max = (*pcit).max;
					}
					if ((*pcit).min < node->min)
					{
						node->min = (*pcit).min;
					}
				}

				// move to next chunk
				pcit++;
			}
		}

		void serialize(value_const_pointer output, const size_type length)
		{

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
}

#endif