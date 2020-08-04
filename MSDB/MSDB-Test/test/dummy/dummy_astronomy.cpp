#include <pch.h>
#include <dummy/dummy_astronomy.h>
#include <fstream>
#include <string>

namespace msdb
{
namespace caDummy
{
namespace data2D_star1024x1024
{
std::vector<dim_type> dims = { dimY, dimX };
std::vector<dim_type> chunkNums = { 2, 2 };
std::vector<dim_type> chunkDims = { dims[0] / chunkNums[0], dims[1] / chunkNums[1] };
std::vector<dim_type> blockNums = {8, 8};
std::vector<dim_type> blockDims = { chunkDims[0] / blockNums[0], chunkDims[1] / blockNums[1] };


void getDummy(value_type* output, size_t length)
{
	assert(length >= dims[0] * dims[1]);
	std::ifstream in("STSCI-H-p1942a-f-1024x1024_Gray.txt");
	assert(in.is_open() == true);

	int c;
	int i = 0;
	while(in && i < dataLength)
	{
		in >> c;
		//switch(c)
		//{
		//case '{':
		//case '}':
		//case ',':
		//case ' ':
		//	continue;
		//}
		//std::cout << "[" << i << "]" << c << std::endl;
		output[i++] = c;
	}
	in.close();
}

void getSourceArrayIfEmpty(std::vector<pArray>& sourceArr)
{
	if (sourceArr.empty())
	{
		sourceArr = getSourceArray();
	}
}
}	// data2D_star1024x1024s
}	// atDummy
}	// msdb
