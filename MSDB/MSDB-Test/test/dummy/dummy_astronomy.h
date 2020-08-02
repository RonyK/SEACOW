#pragma once
#ifndef _MSDB_DUMMY_ASTRONOMY_H_
#define _MSDB_DUMMY_ASTRONOMY_H_

#include <pch.h>

namespace msdb
{
namespace caDummy
{
using dim_type = position_t;

namespace data2D_star1024x1024
{
using value_type = char;

//static const size_t dataLength = 1024 * 1024;
//static const size_t dimX = 1024;
//static const size_t dimY = 1024;
static const size_t dataLength = 512 * 512;
static const size_t dimX = 512;
static const size_t dimY = 512;
static const size_t maxLevel = 5;
static const arrayId aid = 24241;

extern std::vector<dim_type> dims;
extern std::vector<dim_type> chunkDims;
extern std::vector<dim_type> chunkNums;

void getDummy(value_type* output, size_t length);

template<class Aty_ = arrayBase>
std::vector<std::shared_ptr<Aty_>> getSourceArray()
{
	// Get Dummy data
	value_type* data = new value_type[dataLength];
	getDummy(data, dataLength);

	// Build Array
	dimensionDescs arrDimDescs;
	dimensionId dimId = 0;
	arrDimDescs.push_back(std::make_shared<dimensionDesc>(dimId++, "X", 0, dims[1], chunkDims[1]));
	arrDimDescs.push_back(std::make_shared<dimensionDesc>(dimId++, "Y", 0, dims[0], chunkDims[0]));

	attributeDescs attrDescs;
	attributeId attrId = 0;
	attrDescs.push_back(std::make_shared<attributeDesc>(attrId++, "ATTR_1", eleType::CHAR));		// SIGNED CHAR

	pArrayDesc arrDesc = std::make_shared<arrayDesc>(aid, "data2D_star1024x1024", arrDimDescs, attrDescs);
	std::shared_ptr<Aty_> sourceArr = std::make_shared<Aty_>(arrDesc);

	// Build Chunk
	auto a = chunkDims.data();
	auto s = chunkDims.size();
	dimension dimChunk(s, a);
	for (int y = 0; y < chunkNums[0]; y++)
	{
		for (int x = 0; x < chunkNums[1]; x++)
		{
			coor sP = { y * chunkDims[0], x * chunkDims[1] };
			coor eP = { sP[0] + chunkDims[0], sP[1] + chunkDims[1] };

			pChunkDesc cDesc = std::make_shared<chunkDesc>(
				sourceArr->getChunkIdFromItemCoor(sP), attrDescs[0], dimChunk, sP, eP);
			pChunk sourceChunk = std::make_shared<memChunk>(cDesc);
			sourceChunk->alloc();

			// Insert data into chunk
			auto it = sourceChunk->getItemIterator();
			//std::cout << "-----" << std::endl;
			for (int iy = 0; iy < chunkDims[0]; iy++)
			{
				for (int ix = 0; ix < chunkDims[1]; ix++)
				{
					(**it).setChar(data[(y * chunkDims[0] + iy) * dimX + (x * chunkDims[1] + ix)]);
					//char c = (*it).getChar();
					//std::cout << static_cast<int>(c) << ", ";
					++(*it);
				}
				std::cout << std::endl;
			}
			//std::cout << std::endl << "-----" << std::endl;
			sourceArr->insertChunk(sourceChunk);
		}
	}

	delete[] data;
	// Build source array
	std::vector<std::shared_ptr<Aty_>> arrs({ sourceArr });
	return arrs;
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr);
}	// data2D_star1024x1024
}	// caDummy
}	// msdb
#endif	// _MSDB_DUMMY_ASTRONOMY_H_