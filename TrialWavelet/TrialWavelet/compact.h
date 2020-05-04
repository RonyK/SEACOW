#pragma once
#ifndef _caCompact_
#define _caCompact_

#include <iostream>
#include <vector>
#include "bitstringstream.h"
#include "mmt.h"
#include "array.h"
#include "iterators.h"

namespace caWavelet
{
	template <typename Dty_, typename Ty_>
	class caCompact
	{
		using size_type = size_t;

		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_rfeference = const Dty_&;

		using dim_vector = std::vector<Dty_>;
		using dim_vector_const_reference = const std::vector<Dty_>&;

		using value_type = Ty_;
		using value_pointer = Ty_*;
		using value_const_pointer = const Ty_*;
		using value_reference = Ty_&;
		using value_const_reference = const Ty_&;

		using mmt_type = caMMT<Dty_, Ty_>;
		using mmt_reference = mmt_type&;


	public:
		caCompact(dim_vector_const_reference dims, dim_vector_const_reference leafChunkNums, size_type maxLevel) 
			: dims_(dims), dSize_(dims.size()), maxLevel_(maxLevel)
		{
			this->calcBandDims(dims, maxLevel);
			this->calcChunkNums(leafChunkNums, maxLevel);
			this->calcChunkDims(maxLevel);
		}

		void encode(bstream& output, value_pointer wtData, mmt_reference mmt)
		{
			this->enApprximate(output, wtData, mmt);
			this->enDetail(output, wtData, mmt);
		}

	protected:
		void enApprximate(bstream& output, value_pointer wtData, mmt_reference mmt)
		{
			dim_vector chunkNum = this->chunkNums_[this->maxLevel_];	// size of approximate chunks are different 
			dim_vector bandDims = this->bandDims_[this->maxLevel_];

			caCoorIterator<Dty_, size_type> chunkIt(NULL, this->dSize_, chunkNum.data());
			size_type chunkLength = calcLength(chunkNum.data(), chunkNum.size());
			size_type dataLength = calcLength(bandDims.data(), bandDims.size());

			dim_vector cSp(this->dSize_);	// start point of chunk
			dim_vector cEp(this->dSize_);	// end point of chunk

			auto nodes = mmt.getNodes(this->maxLevel_);

			size_type maxBits = 0;
			for (size_type c = 0; c < chunkLength; c++)
			{
				if (nodes[c].bits > maxBits)
				{
					maxBits = nodes[c].bits;
				}
			}

			for (size_type d = 0; d < this->dSize_; d++)
			{
				cSp[d] = 0;
				cEp[d] = bandDims[d];
			}

			// Approximate data consists of one chunk
			this->encodeChunk(output, wtData, cSp.data(), cEp.data(), dataLength, maxBits);
		}

		void enDetail(bstream& output, value_pointer wtData, mmt_reference mmt)
		{
			size_type bandLength = pow(2, this->dSize_);

			for (size_type level = this->maxLevel_; level != -1; --level)
			{
				dim_vector chunkNum = this->chunkNums_[level];
				dim_vector chunkDims = this->chunkDims_[level];
				dim_vector bandDims = this->bandDims_[level];

				// Get chunk Iterator
				caCoorIterator<Dty_, size_type> chunkIt(NULL, this->dSize_, chunkNum.data());
				size_type chunkLength = calcLength(chunkNum.data(), chunkNum.size());
				size_type dataLength = calcLength(chunkDims.data(), chunkDims.size());

				for (size_type chunk_id = 0; chunk_id < chunkLength; ++chunk_id, ++chunkIt)
				{
					this->enDetailChunk(output, wtData, mmt, level, chunk_id, chunkIt.coor(), dataLength);
				}
			}
		}

	private:
		void enDetailChunk(bstream& output, value_pointer wtData, mmt_reference mmt, size_type level,
			size_type chunk_id, caCoor<Dty_> chunkCoor, size_type length)
		{
			dim_vector cSp(this->dSize_);	// start point of chunk
			dim_vector cEp(this->dSize_);	// end point of chunk

			auto mmtChunk = mmt.getNodes(level)[chunk_id];

			for (size_type band_id = 1; band_id < pow(2, this->dSize_); band_id++)
			{
				// Set band start point (start, end)
				size_type mask = (size_type)0x1 << (this->dSize_ - 1);
				for (size_type d = 0; d < this->dSize_; d++)
				{
					cSp[d] = chunkCoor[d] * this->chunkDims_[level][d];
					cEp[d] = (chunkCoor[d] + 1) * this->chunkDims_[level][d];
					if (band_id & mask)
					{
						cSp[d] += this->bandDims_[level][d];
						cEp[d] += this->bandDims_[level][d];
					}
					mask >>= 1;
				}

				this->encodeChunk(output, wtData, cSp.data(), cEp.data(), length, mmtChunk.bits);
			}
		}

		void encodeChunk(bstream& output, value_pointer wtData, dim_const_pointer cSp, dim_const_pointer cEp,
			const size_type length, const size_type bits)
		{
			caWTRangeIterator<Dty_, Ty_> it(wtData, this->dSize_, this->dims_.data(), cSp, cEp, this->maxLevel_);

			output << setw(bits);
			for (size_type i = 0; i < length; i++)
			{
				output << (*it++);
			}
		}

		void calcBandDims(dim_vector_const_reference dims, size_type maxLevel)
		{
			for (size_type level = 0; level <= maxLevel; level++)
			{
				if (this->bandDims_.size() <= level)
				{
					this->bandDims_.push_back(dim_vector());
				}
				size_type factor = pow(2, level + 1);
				for (size_type d = 0; d < this->dSize_; d++)
				{
					this->bandDims_.back().push_back(this->dims_[d] / factor);
				}
			}
		}

		void calcChunkNums(dim_vector_const_reference leafChunkNums, size_type maxLevel)
		{
			for (size_type level = 0; level <= maxLevel; level++)
			{
				if (this->chunkNums_.size() <= level)
				{
					this->chunkNums_.push_back(dim_vector());
				}
				size_type factor = (size_type)pow(2, level);
				for (size_type d = 0; d < this->dSize_; d++)
				{
					this->chunkNums_.back().push_back((leafChunkNums[d] / factor));
				}
			}
		}

		void calcChunkDims(size_type maxLevel)
		{
			for (size_type level = 0; level <= maxLevel; level++)
			{
				if (this->chunkDims_.size() <= level)
				{
					this->chunkDims_.push_back(dim_vector());
				}
				for (size_type d = 0; d < this->dSize_; d++)
				{
					this->chunkDims_.back().push_back(this->bandDims_[level][d] / this->chunkNums_[level][d]);
				}
			}
		}

	protected:
		dim_vector dims_;
		std::vector<dim_vector> bandDims_;
		std::vector<dim_vector> chunkDims_;
		std::vector<dim_vector> chunkNums_;
		size_type maxLevel_;
		size_type dSize_;
	};
}

#endif	// _caCompact_