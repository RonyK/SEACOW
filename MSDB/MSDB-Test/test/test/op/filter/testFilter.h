#pragma once
#ifndef _MSDB_TESTFILTER_H_
#define _MSDB_TESTFILTER_H_

#include <pch.h>
#include <array/attributeId.h>
#include <parse/predicate.h>
#include <index/testMMT.h>
#include <io/testIO.h>

#include <compression/testSeCompression.h>

#include <op/naive_filter/naive_filter_plan.h>
#include <op/naive_filter/naive_filter_action.h>

#include <op/index_filter/index_filter_plan.h>
#include <op/index_filter/index_filter_action.h>

namespace msdb
{
namespace caDummy
{
pArray naive_filter(_vectorSourceArray_, pPredicate myPredicate, pQuery qry = nullptr);

pArray index_filter(_vectorSourceArray_, pPredicate myPredicate, pQuery qry = nullptr);

pTerm getEqualTerm(int64_t value, attributeId attrId = 0);

template <typename value_type>
bool equalTest(pArray arr, int64_t value)
{
	size_t numValues = 0;

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
								EXPECT_EQ((**iit).get<value_type>(), static_cast<value_type>(value));
								++numValues;
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

	std::cout << "Filtered values: " << numValues << std::endl;
	return true;
}

template <typename value_type>
pArray action_execute_naive_filter(_pFuncGetSourceArray_,
								   pPredicate myPredicate, bool printFlag = false)
{
	//////////////////////////////
	// 01 Source Arr
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Source Arr" << std::endl;
		sourceArr[0]->print();
	}

	auto filterArray = naive_filter(sourceArr, myPredicate);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Filtered Arr" << std::endl;
		filterArray->print();
	}

	//EXPECT_TRUE(false);
	return filterArray;
}

template <typename value_type>
pArray action_execute_index_filter(_pFuncGetSourceArray_, 
								   pPredicate myPredicate,
								   eleDefault mmtLevel,
								   bool printFlag = false)
{
	//////////////////////////////
	// 01 Source Arr
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Source Arr" << std::endl;
		sourceArr[0]->print();
	}

	auto outArr = mmt_build(sourceArr, mmtLevel);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "MMT Build Arr" << std::endl;
		outArr->print();
	}

	auto filteredArr = index_filter(std::vector<pArray>({ outArr }), myPredicate);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Filtered Arr" << std::endl;
		filteredArr->print();
	}

	//EXPECT_TRUE(false);
	return filteredArr;
}

template <typename value_type>
pArray action_execute_load_index_filter(_pFuncGetSourceArray_,
										pPredicate inPredicate,
										eleDefault mmtLevel,
										bool printFlag = false)
{
	//////////////////////////////
	// 01 Source Arr
	std::vector<pArray> saveSourceArr;
	getSourceArrayIfEmpty(saveSourceArr);
	if (false)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Source Arr" << std::endl;
		saveSourceArr[0]->print();
	}

	auto outArr = mmt_build(saveSourceArr, mmtLevel);
	if (false)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "MMT Build Arr" << std::endl;
		outArr->print();
	}

	outArr = save(saveSourceArr);
	if (false)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Save Arr" << std::endl;
		outArr->print();
	}
	//saveSourceArr.clear();
	outArr = nullptr;

	//////////////////////////////
	// 02 Source Arr
	//std::vector<pArray> filterSourceArr;
	//getSourceArrayDesc(filterSourceArr);
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLoadPlan(saveSourceArr[0]->getDesc(), qry);
	auto filterPlan = getIndexFilterPlan(loadPlan, inPredicate, qry);

	auto loadAction = loadPlan->getAction();
	outArr = loadAction->execute(saveSourceArr, qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Load Arr" << std::endl;
		outArr->print();
	}

	auto filterAction = filterPlan->getAction();
	outArr = filterAction->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Filtered Arr" << std::endl;
		outArr->print();
	}

	dearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray action_execute_se_index_filter(_pFuncGetSourceArray_,
									  pPredicate inPredicate,
									  eleDefault wtLevel,
									  eleDefault mmtLevel,
									  bool printFlag = false)
{
	//////////////////////////////
	// 01 Save Source Arr
	std::vector<pArray> saveSourceArr;
	getSourceArrayIfEmpty(saveSourceArr);
	saveSourceArr[0]->setId(saveSourceArr[0]->getId() + 2);

	action_execute_mmt_build<value_type>(saveSourceArr, mmtLevel, false);
	action_execute_se_compression<value_type>(saveSourceArr, wtLevel, mmtLevel, false);

	//////////////////////////////
	// 02 Index Filter Test
	pQuery qry = std::make_shared<query>();

	auto seDecompPlan = getSeDecompressionPlan(saveSourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(seDecompPlan, wtLevel, qry);
	auto deltaDecodePlan = getMMTDeltaDecodePlan(wtDecodePlan, qry);
	auto filterPlan = getIndexFilterPlan(deltaDecodePlan, inPredicate, qry);

	auto outArr = seDecompPlan->getAction()->execute(saveSourceArr, qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Se Decomp Arr" << std::endl;
		outArr->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wt Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = deltaDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Delta Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Filtered Arr" << std::endl;
		outArr->print();
	}

	dearDownQuery(qry);

	return outArr;
}
}		// caDummy
}		// msdb
#endif	//_MSDB_TESTFILTER_H_
