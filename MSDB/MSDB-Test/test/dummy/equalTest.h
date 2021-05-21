#pragma once
#ifndef _MSDB_TEST_EQUALTEST_H_
#define _MSDB_TEST_EQUALTEST_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace caDummy
{
#define _MAX_EQUALTEST_ERROR_REPRESENT_		10

template <typename value_type>
bool equalTest(pArray arr, int64_t value)
{
	BOOST_LOG_TRIVIAL(debug) << "##############################";
	BOOST_LOG_TRIVIAL(debug) << "Value Equal Test";
	BOOST_LOG_TRIVIAL(debug) << "##############################";

	size_t numCells = 0;
	size_t numErrors = 0;

	for (auto attrDesc : *arr->getDesc()->attrDescs_)
	{
		auto cit = arr->getChunkIterator();
		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				auto bit = (**cit)->getBlockIterator();

				while (!bit->isEnd())
				{
					if (bit->isExist())
					{
						auto iit = (**bit)->getItemIterator();

						while (!iit->isEnd())
						{
							if (iit->isExist())
							{
								if ((**iit).get<value_type>() != value)
								{
									++numErrors;
								}

								if (numErrors > _MAX_EQUALTEST_ERROR_REPRESENT_)
								{
									return;
								}
								EXPECT_EQ((**iit).get<value_type>(), static_cast<value_type>(value));
								++numCells;
							}
							++(*iit);
						}
					}
					++(*bit);
				}
			}
			++(*cit);
		}
	}

	BOOST_LOG_TRIVIAL(info) << "[Validation] num values: " << numCells << std::endl;
	return true;
}
}		// caDummy
}		// msdb
#endif	// _MSDB_TEST_EQUALTEST_H_