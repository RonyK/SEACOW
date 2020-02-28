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
			// chunk num은 level올라갈 때 마다 모든 차원에서 1/2씩 줄어듬

			// Build MMT from a 0 level.
			// level 0 일때는 data 이용해서 mmt 만들고
			this->buildLeafMMT(data, length, chunkDims, &chunkNum);
			for (size_type level = 1; level < maxLevel; level++)
			{
				// 이후부터는 이전 mmt level 이용해서 build 
				this->buildIntermediateMMT(level, chunkDims, &chunkNum);
			}
		}

		// For level 0
		void buildLeafMMT(value_pointer data, size_const length, dim_vector_pointer chunkDims,
			dim_vector_pointer chunkNum)
		{
			unsigned int tl = this->calcLength(chunkNum);
			this->nodes.push_back(new mmtNode[this->calcLength(chunkNum)]);

			caCoorIterator<Dty_, Ty_> it(data, this->dims_.size(), this->dims_.data());
			caCoorIterator<Dty_, mmtNode> cit(this->nodes[0], chunkNum->size(), chunkNum->data());

			size_type dimSize = this->dims_.size();

			for (size_type i = 0; i < length; i++)
			{
				// current iterator coordiate -> chunk coordinate
				caCoor<Dty_> cur = it;
				for (size_type d = 0; d < dimSize; d++)
				{
					cur[d] = cur[d] / chunkDims->at(d);
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
			size_type dimSize = chunkNum->size();

			// calc chunk num of prev and current level
			dim_vector curLevelChunkNum(chunkNum);
			dim_vector prevLevelChunkNum(dimSize);

			for (size_type d = 0; d < dimSize; d++)
			{
				prevLevelChunkNum[d] /= pow(2, prevLevel);
				curLevelChunkNum[d] = prevLevelChunkNum[d] / 2;
			}

			caCoorIterator<Dty_, mmtNode> pcit(this->nodes[prevLevel], dimSize,
				prevLevelChunkNum.data());
			caCoorIterator<Dty_, mmtNode> cit(this->nodes[prevLevel], dimSize,
				curLevelChunkNum.data());

			for (size_type i = 0; i < prevLevelChunkNum.size(); i++)
			{
				// current iterator coordiate -> chunk coordinate
				caCoor<Dty_> cur = pcit.coor;
				for (size_type d = 0; d < dimSize; d++)
				{
					cur[i] /= curLevelChunkNum[i];
				}

				// get target chunk
				cit.moveTo(cur);
				mmtNode node = *cit;

				// compare min max value
				if (*pcit > node.max)
				{
					node.max = *pcit;
				}
				if (*pcit < node.min)
				{
					node.min = *pcit;
				}

				// move to next value
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
				size_type chunks = 1 + (dims[d] + -1) / chunkDims[d];

				if (output->size() <= d)
				{
					output->push_back(chunks);
				}
				else
				{
					output[d] = chunks;
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