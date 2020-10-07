#pragma once
#ifndef _MSDB_DUMMY_H_
#define _MSDB_DUMMY_H_

#include <pch.h>
#include <array/memBlockArray.h>
#include <array/memChunk.h>
#include <array/blockChunk.h>
#include <vector>

namespace msdb
{
namespace caDummy
{
template <typename Aty_>
std::shared_ptr<Aty_> get2DCharArray(arrayId aid, std::string arrayName,
									 dimension dims, dimension chunkDims, dimension blockDims,
									 eleType eType)
{
	// Build Array
	pDimensionDescs dimDescs = std::make_shared<dimensionDescs>();
	dimensionId dimId = 0;
	dimDescs->push_back(std::make_shared<dimensionDesc>(dimId++, "Y", 0, dims[0], chunkDims[0], blockDims[0]));
	dimDescs->push_back(std::make_shared<dimensionDesc>(dimId++, "X", 0, dims[1], chunkDims[1], blockDims[1]));

	pAttributeDescs attrDescs = std::make_shared<attributeDescs>();
	attributeId attrId = 0;
	attrDescs->push_back(std::make_shared<attributeDesc>(attrId++, "ATTR_1", eType));		// SIGNED CHAR

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(aid, arrayName.c_str(), dimDescs, attrDescs);
	std::shared_ptr<Aty_> sourceArr = std::make_shared<Aty_>(arrDesc);

	return sourceArr;
}

inline char unsignedCharToSigned(char value);

template <typename Aty_, typename Ty_>
std::shared_ptr<Aty_> get2DCharArray(void* dummy, arrayId aid, std::string arrayName,
									 dimension dims, dimension chunkDims, dimension blockDims,
									 eleType eType)
{
	dimension chunkNums = dims / chunkDims;
	dimension blockNums = chunkDims / blockDims;

	std::shared_ptr<Aty_> sourceArr = get2DCharArray<Aty_>(aid, arrayName, dims, chunkDims, blockDims, eType);

	// Build Chunk
	Ty_* data = (Ty_*)dummy;
	for (int y = 0; y < chunkNums[0]; y++)
	{
		for (int x = 0; x < chunkNums[1]; x++)
		{
			// TODO::block iterate
			coor sP = { y * chunkDims[0], x * chunkDims[1] };
			coor eP = { sP[0] + chunkDims[0], sP[1] + chunkDims[1] };

			pChunkDesc cDesc = std::make_shared<chunkDesc>(
				sourceArr->getChunkIdFromItemCoor(sP),
				sourceArr->getDesc()->getAttrDescs()->at(0),
				chunkDims, blockDims,
				sP, eP);
			pChunk sourceChunk = std::make_shared<memBlockChunk>(cDesc);
			sourceChunk->bufferAlloc();
			//sourceChunk->replaceBlockBitmap(std::make_shared<bitmap>(blockDims.area()));
			sourceChunk->makeAllBlocks();

			// Insert data into chunk
			auto bItr = sourceChunk->getBlockIterator();
			while (!bItr->isEnd())
			{
				auto blockCoor = bItr->coor();
				auto it = (**bItr)->getItemIterator();
				for (int iy = 0; iy < blockDims[0]; ++iy)
				{
					for (int ix = 0; ix < blockDims[1]; ++ix)
					{
						size_t seqPos = (y * chunkDims[0] + blockCoor[0] * blockDims[0] + iy) * dims[1] + (x * chunkDims[1] + blockCoor[1] * blockDims[1] + ix);
						(**it).setChar(static_cast<Ty_>(data[seqPos]));
						++(*it);
					}
				}
				++(*bItr);
			}
			sourceArr->insertChunk(0, sourceChunk);
		}
	}

	// Build source array
	return sourceArr;
}

template <typename Ty_>
void compArrary(pArray lArr, pArray rArr)
{
	auto lAttrDesc = lArr->getDesc()->attrDescs_;
	auto rAttrDesc = rArr->getDesc()->attrDescs_;

	EXPECT_EQ(lAttrDesc->size(), rAttrDesc->size());

	for (int attrId = 0; attrId < lAttrDesc->size(); ++attrId)
	{
		auto lcItr = lArr->getChunkIterator();
		auto rcItr = rArr->getChunkIterator();

		EXPECT_EQ(lcItr->getCapacity(), rcItr->getCapacity());
		EXPECT_EQ(lcItr->isEnd(), rcItr->isEnd());

		while (!lcItr->isEnd() && !rcItr->isEnd())
		{
			EXPECT_EQ(lcItr->isExist(), rcItr->isExist());
			
			if(lcItr->isExist())
			{
				EXPECT_TRUE(lcItr->coor() == rcItr->coor());
#ifndef NDEBUG
				//if(!(*(**lcItr) == *(**rcItr)))
				//{
				//	BOOST_LOG_TRIVIAL(debug) << "NOT EQUAL";
				//	BOOST_LOG_TRIVIAL(debug) << "LEFT: ";
				//	(**lcItr)->print();
				//	BOOST_LOG_TRIVIAL(debug) << "RIGHT: ";
				//	(**lcItr)->print();
				//}
#endif
				//EXPECT_TRUE(*(**lcItr) == *(**rcItr));

				auto lbItr = (*lcItr)->getBlockIterator();
				auto rbItr = (*rcItr)->getBlockIterator();

				while (!lbItr->isEnd() && !rbItr->isEnd())
				{
					EXPECT_EQ(lbItr->isExist(), rbItr->isExist());
					if(lbItr->isExist())
					{
						auto liItr = (*lbItr)->getItemIterator();
						auto riItr = (*rbItr)->getItemIterator();

						while (!liItr->isEnd() && !riItr->isEnd())
						{
							EXPECT_EQ(liItr->isExist(), riItr->isExist());

							if(liItr->isExist())
							{
								Ty_ li = (**liItr).get<Ty_>();
								Ty_ ri = (**riItr).get<Ty_>();

								if (li != ri)
								{
									BOOST_LOG_TRIVIAL(debug) << "Diff : " << static_cast<int64_t>(li) << ", " << static_cast<int64_t>(ri);
									BOOST_LOG_TRIVIAL(debug) << "Chunk: " << lcItr->coor().toString() << " / Block: " << lbItr->coor().toString() << " / Item: " << liItr->coor().toString();
								}
								EXPECT_EQ(li, ri);
							}

							++(*liItr);
							++(*riItr);
						}

						EXPECT_EQ(liItr->isEnd(), riItr->isEnd());
					}

					++(*lbItr);
					++(*rbItr);
				}
			}

			++(*lcItr);
			++(*rcItr);
		}

		EXPECT_EQ(lcItr->isEnd(), rcItr->isEnd());
	}
}

template <typename Ty_>
void compChunkItems(pChunk lChunk, pChunk rChunk)
{
	std::cout << "==============================" << std::endl;
	lChunk->print();
	std::cout << "==============================" << std::endl;
	rChunk->print();
	std::cout << "==============================" << std::endl;
	EXPECT_TRUE(false);
	//auto lbItr = lChunk->getBlockIterator();
	//auto rbItr = rChunk->getBlockIterator();

	//while (!lbItr->isEnd() && !rbItr->isEnd())
	//{
	//	auto liItr = (*lbItr)->getItemIterator();
	//	auto riItr = (*rbItr)->getItemIterator();

	//	while (!liItr->isEnd() && !riItr->isEnd())
	//	{
	//		Ty_ li = (**liItr).get<Ty_>();
	//		Ty_ ri = (**riItr).get<Ty_>();

	//		EXPECT_EQ(li, ri);

	//		++(*liItr);
	//		++(*riItr);
	//	}

	//	EXPECT_EQ(liItr->isEnd(), riItr->isEnd());

	//	++(*lbItr);
	//	++(*rbItr);
	//}

	//EXPECT_EQ(lbItr->isEnd(), rbItr->isEnd());
}

using dim_type = position_t;
// signed char
namespace data2D_sc4x4
{
using value_type = char;

static const size_t dataLength = 16;
static const size_t dimX = 4;
static const size_t dimY = 4;
static const size_t wtLevel = 0;
static const size_t mmtLevel = 0;
static const arrayId aid = 441;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> blockNums;
extern std::vector<dim_type> blockDims;

void getDummy(value_type* output, size_t length);
void getChunkDummy(value_type* output, size_t length);
void getWTDummy(value_type* output, size_t length);
void getWTChunkDummy(value_type* output, size_t length);
void getExDummy(value_type* output, size_t length);
void getExMMTBuilded(value_type(*minOutput)[2][2], value_type(*maxOutput)[2][2], size_t length);
void getExDelta(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type data[dataLength];
	getDummy(data, dataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_sc4x4", dims, chunkDims, blockDims, eleType::CHAR)) });
	return arrs;
}

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArrayDesc()
{
	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_>(aid, "data2D_sc4x4", dims, chunkDims, blockDims, eleType::CHAR)) });
	return arrs;
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr);
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}

namespace data2D_tempTest
{
using value_type = char;

static const size_t dataLength = 16;
static const size_t dimX = 4;
static const size_t dimY = 4;
static const size_t wtLevel = 0;
static const size_t mmtLevel = 0;
static const arrayId aid = 9991;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> blockNums;
extern std::vector<dim_type> blockDims;

void getDummy(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type data[dataLength];
	getDummy(data, dataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_tempTest", dims, chunkDims, blockDims, eleType::CHAR)) });
	return arrs;
}

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArrayDesc()
{
	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_>(aid, "data2D_tempTest", dims, chunkDims, blockDims, eleType::CHAR)) });
	return arrs;
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr);
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}

// signed char
namespace data2D_sc8x8
{
using value_type = char;

static const size_t dataLength = 64;
static const size_t dimX = 8;
static const size_t dimY = 8;
static const size_t wtLevel = 2;
static const size_t mmtLevel = 2;
static const arrayId aid = 881;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> blockDims;

void getDummy(value_type* output, size_t length);
void getWTDummy(value_type* output, size_t length);
void getExDummy(value_type* output, size_t length);
void getExMMTBuilded(value_type(*minOutput)[4][4], value_type(*maxOutput)[4][4], size_t length);
void getExSerialMMT(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type data[dataLength];
	getDummy(data, dataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_sc8x8", dims, chunkDims, blockDims, eleType::CHAR)) });
	return arrs;
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_sc8x8
}	// caDummy
}	// msdb

#endif	//_MSDB_DUMMY_H_