#pragma once
#ifndef _MSDB_CHUNK_H_
#define _MSDB_CHUNK_H_

#include <array/chunkBuffer.h>
#include <array/chunkDesc.h>
#include <io/serializable.h>
#include <io/bitstream.h>
#include <memory>

namespace msdb
{
class chunk;
class chunkItemIterator;
using pChunk = std::shared_ptr<chunk>;

template <typename Dty_>
class chunkItemIteratorBase : virtual public coordinateIterator<Dty_>
{
public:
	using base_type = coordinateIterator<Dty_>;
	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	chunkItemIteratorBase(void* data, eleType eType, const size_type dSize,
						  position_t* dims, dim_pointer csP)
		: base_type(dSize, dims)
	{
		this->csP_ = new dim_type[dSize];
		this->memcpyDim(this->csP_, csP);
	}

public:
	coordinate_type coorOut2In(coordinate_type& out)
	{
		coordinate_type in(this->dSize());
		for (dimensionId d = 0; d < this->dSize(); d++)
		{
			in[d] = out[d] - this->csP_[d];
		}

		return in;
	}

	coordinate_type coorIn2Out(coordinate_type& in)
	{
		coordinate_type out(this->dSize());
		for (dimensionId d = 0; d < this->dSize(); d++)
		{
			out[d] = in[d] + this->csP_[d];
		}
		return out;
	}

	coordinate_type coorIn2Out()
	{
		return this->coorIn2Out(this->coor_);
	}

	coordinate_type ceP()
	{
		coordinate_type ceP(this->dSize());
		for (dimensionId d = 0; d < this->dSize(); d++)
		{
			ceP[d] = this->csP_[d] + this->dims_[d];
		}

		return ceP;
	}

	coordinate_type outCoor()
	{
		coordinate_type out(this->dSize());
		for (dimensionId d = 0; d < this->dSize(); d++)
		{
			out[d] = this->coor_[d] + this->csP_[d];
		}
		return out;
	}

	coordinate_type innerCoor()
	{
		return this->coor_;
	}

protected:
	dim_pointer csP_;			// Chunk start point
};

using chunkItemItrBase = chunkItemIteratorBase<position_t>;

class chunkItemIterator : public itemItr, public chunkItemItrBase
{
public:
	using self_type = chunkItemIterator;
	using base_type = itemItr;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	chunkItemIterator(void* data, eleType eType, const size_type dSize,
					  position_t* dims, dim_pointer csP);
};

class chunkItemRangeIterator : public itemRangeItr, public chunkItemItrBase
{
public:
	using base_type = itemRangeItr;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;


public:
	chunkItemRangeIterator(void* data, eleType eType, const size_type dSize,
						   position_t* dims, dim_const_pointer sP, dim_const_pointer eP,
						   dim_pointer csP);

	chunkItemRangeIterator(void* data, eleType eType, const size_type dSize,
						   position_t* dims, const coorRange& range,
						   dim_pointer csP);
};

class chunk : public std::enable_shared_from_this<chunk>, public serializable
{
public:
	using size_type = size_t;

public:
	chunk(pChunkDesc desc);
	virtual ~chunk();

public:
	virtual void alloc();
	virtual void alloc(bufferSize size);
	virtual void materializeCopy(void* data, bufferSize size);
	virtual void materializeAssign(void* data, bufferSize size);
	bool isMaterialized() const;

	chunkId getId() const;
	// chunk id can be chnaged in query processing
	void setId(chunkId id);
	const pChunkDesc getDesc() const;
	size_type numCells();

	void print();

	coor getChunkCoor();
	virtual chunkItemIterator getItemIterator() = 0;
	virtual chunkItemRangeIterator getItemRangeIterator(const coorRange& range) = 0;

	template <class Ty_>
	void printImp()
	{
		auto it = this->getItemIterator();
		std::cout << "==============================" << std::endl;
		for (size_t i = 0; i < it.getCapacity() && !it.isEnd(); ++i, ++it)
		{
			std::cout << (*it).get<Ty_>() << ", ";
		}
		std::cout << std::endl << "==============================" << std::endl;
	}

	template<>
	void printImp<char>()
	{
		auto it = this->getItemIterator();
		std::cout << "==============================" << std::endl;
		for (size_t i = 0; i < it.getCapacity() && !it.isEnd(); ++i, ++it)
		{
			std::cout << static_cast<int>((*it).get<char>()) << ", ";
		}
		std::cout << std::endl << "==============================" << std::endl;
	}

protected:
	void free();
	virtual void makeBuffer() = 0;

protected:
	pChunkBuffer cached_;	// hold materialized chunk
	pChunkDesc desc_;		// chunk desc

protected:
	//////////////////////////////
	// Chunk Header				//
	//////////////////////////////
	class chunkHeader : public serialHeader
	{
	public:
		static const char chunk_header_version = 1;

	public:
		virtual void serialize(std::ostream& os) override
		{
			std::cout << "Chunk Header serialize" << std::endl;
			std::cout << this->version_ << ", " << this->bodySize_ << std::endl;

			os.write((char*)(&this->version_), sizeof(this->version_));
			os.write((char*)(&this->bodySize_), sizeof(this->bodySize_));
		}
		virtual void deserialize(std::istream& is) override
		{
			std::cout << "Chunk Header deserialize" << std::endl;

			is.read((char*)(&this->version_), sizeof(this->version_));
			is.read((char*)(&this->bodySize_), sizeof(this->bodySize_));

			std::cout << this->version_ << ", " << this->bodySize_ << std::endl;
		}
	};

public:
	// inherit from serializable
	virtual void updateToHeader() override;
	virtual void updateFromHeader() override;
};
};
#endif	// _MSDB_CHUNK_H_