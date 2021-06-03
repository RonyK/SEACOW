#pragma once
#ifndef _MSDB_DUMMY_H_
#define _MSDB_DUMMY_H_

#include <pch.h>
#include <array/memBlockArray.h>
#include <array/memChunk.h>
#include <array/blockChunk.h>
#include <util/threadUtil.h>
#include <vector>

namespace msdb
{
namespace caDummy
{
using dim_type = position_t;

template <typename Ty_>
void getChunk(Ty_* data, pChunk sourceChunk, const dimension originalDims, const dimension blockDims, const coor sP, const coor eP, const size_t offsetX, const size_t offsetY)
{
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
				int globalX = sP[1] + blockCoor[1] * blockDims[1] + ix + offsetX;
				int globalY = sP[0] + blockCoor[0] * blockDims[0] + iy + offsetY;

				size_t seqPos = globalY * originalDims[1] + globalX;
				(**it).set<Ty_>(static_cast<Ty_>(data[seqPos]));
				++(*it);
			}
		}
		++(*bItr);
	}
}

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
std::shared_ptr<Aty_> get2DCharArray(void* dummy, arrayId aid, std::string arrayName, dimension originalDims,
									 dimension dims, dimension chunkDims, dimension blockDims,
									 eleType eType)
{
	dimension chunkNums = dims / chunkDims;
	dimension blockNums = chunkDims / blockDims;
	size_t offsetX = 0;
	size_t offsetY = 0;

	std::shared_ptr<Aty_> sourceArr = get2DCharArray<Aty_>(aid, arrayName, dims, chunkDims, blockDims, eType);

	threadUtil myThread;
	myThread.createThread(_MSDB_ACTION_THREAD_NUM_);

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
			sourceArr->insertChunk(0, sourceChunk);

			myThread.get_io_service()->post(boost::bind(&getChunk<Ty_>, data, sourceChunk, originalDims, blockDims, sP, eP, offsetX, offsetY));
		}
	}

	myThread.threadStop();
	myThread.threadJoin();

	// Build source array
	return sourceArr;
}

template <typename Ty_>
void compArrary(pArray lArr, pArray rArr)
{
	BOOST_LOG_TRIVIAL(debug) << "##############################";
	BOOST_LOG_TRIVIAL(debug) << "Comp Array Test";
	BOOST_LOG_TRIVIAL(debug) << "##############################";

	bool isFirstWrongChunk = true;
	size_t wrongValue = 0;
	auto lAttrDesc = lArr->getDesc()->attrDescs_;
	auto rAttrDesc = rArr->getDesc()->attrDescs_;

	EXPECT_EQ(lAttrDesc->size(), rAttrDesc->size());

	size_t numChunks = 0, numBlocks = 0, numCells = 0;

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
				++numChunks;
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

				EXPECT_EQ(lbItr->isEnd(), rbItr->isEnd());
				while (!lbItr->isEnd() && !rbItr->isEnd())
				{
					EXPECT_EQ(lbItr->isExist(), rbItr->isExist());
					if(lbItr->isExist())
					{
						++numBlocks;
						auto liItr = (*lbItr)->getItemIterator();
						auto riItr = (*rbItr)->getItemIterator();

						EXPECT_EQ(liItr->isEnd(), riItr->isEnd());
						while (!liItr->isEnd() && !riItr->isEnd())
						{
							// Limit wrong value output
							if(wrongValue > 20)
							{
								return;
							}

							EXPECT_EQ(liItr->isExist(), riItr->isExist());
							if(liItr->isExist())
							{
								++numCells;
								Ty_ li = (**liItr).get<Ty_>();
								Ty_ ri = (**riItr).get<Ty_>();

								if (li != ri)
								{
									BOOST_LOG_TRIVIAL(debug) << "Diff : " << static_cast<int64_t>(li) << ", " << static_cast<int64_t>(ri);
									BOOST_LOG_TRIVIAL(debug) << "Chunk: " << lcItr->coor().toString() << "(" << lcItr->seqPos() << ") / Block: " << lbItr->coor().toString() << "(" << lbItr->seqPos() << ") / Item: " << liItr->coor().toString();
									++wrongValue;

									if(isFirstWrongChunk)
									{
										isFirstWrongChunk = false;

										BOOST_LOG_TRIVIAL(debug) << "Left Block" << std::endl;
										(*lbItr)->print();
										BOOST_LOG_TRIVIAL(debug) << "Right Block" << std::endl;
										(*rbItr)->print();

									}
								}
								EXPECT_EQ(li, ri);
							}

							++(*liItr);
							++(*riItr);

							EXPECT_EQ(liItr->isEnd(), riItr->isEnd());
						}
					}

					++(*lbItr);
					++(*rbItr);

					EXPECT_EQ(lbItr->isEnd(), rbItr->isEnd());
				}
			}

			++(*lcItr);
			++(*rcItr);

			EXPECT_EQ(lcItr->isEnd(), rcItr->isEnd());
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "Comp chunks: " << numChunks;
	BOOST_LOG_TRIVIAL(debug) << "Comp blocks: " << numBlocks;
	BOOST_LOG_TRIVIAL(debug) << "Comp cells: " << numCells;
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

// signed char
namespace data2D_sc4x4
{
using value_type = char;

static const size_t dataLength = 16;
static const size_t originalDataLength = 16;
static const size_t origianlDimX = 4;
static const size_t originalDimY = 4;
static const size_t dimX = 4;
static const size_t dimY = 4;
static const size_t wtLevel = 0;
static const size_t mmtLevel = 0;
static const arrayId aid = 441;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> originalDims;
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
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_sc4x4", originalDims, dims, chunkDims, blockDims, eleType::CHAR)) });
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
extern std::vector<dim_type> originalDims;
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
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_tempTest", originalDims, dims, chunkDims, blockDims, eleType::CHAR)) });
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

namespace data2D_test32x32
{
using dim_type = position_t;

using value_type = char;
static const eleType ele_type = eleType::CHAR;


static const size_t origianlDimX = 16;
static const size_t originalDimY = 16;
static const dim_type dimX = 16;
static const dim_type dimY = 16;
static const size_t dataLength = dimX * dimY;
static const size_t originalDataLength = origianlDimX * originalDimY;
static const size_t wtLevel = 2;
static const size_t mmtLevel = 2;
static const arrayId aid = 3232;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> originalDims;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> chunkNums;
extern std::vector<dim_type> blockDims;
extern std::vector<dim_type> blockNums;

void getDummy(value_type* output, size_t length);

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArray()
{
	// Get Dummy data
	value_type* data = new value_type[originalDataLength];
	getDummy(data, originalDataLength);

	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_test32x32", originalDims, dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

template<class Aty_ = memBlockArray>
std::vector<pArray> getSourceArrayDesc()
{
	std::vector<pArray> arrs(
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_>(aid, "data2D_test32x32", dims, chunkDims, blockDims, ele_type)) });
	return arrs;
}

void getSourceArrayDesc(std::vector<pArray>& sourceArr);
void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_test32x32

// signed char
namespace data2D_sc8x8
{
using value_type = char;

static const size_t dataLength = 64;
static const size_t origianlDimX = 8;
static const size_t originalDimY = 8;
static const size_t dimX = 8;
static const size_t dimY = 8;
static const size_t originalDataLength = origianlDimX * originalDimY;
static const size_t wtLevel = 2;
static const size_t mmtLevel = 2;
static const arrayId aid = 881;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> originalDims;
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
		{ std::static_pointer_cast<arrayBase>(get2DCharArray<Aty_, value_type>(data, aid, "data2D_sc8x8", originalDims, dims, chunkDims, blockDims, eleType::CHAR)) });
	return arrs;
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_sc8x8
}	// caDummy
}	// msdb

#endif	//_MSDB_DUMMY_H_