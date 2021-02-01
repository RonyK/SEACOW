#pragma once
#ifndef _MSDB_TESTFILTER_H_
#define _MSDB_TESTFILTER_H_

#include <pch.h>
#include <array/attributeId.h>
#include <parse/predicate.h>
#include <index/test_action_mmt.h>
#include <io/test_action_io.h>
#include <index/test_qry_mmt.h>

#include <compression/test_qry_secompression.h>

#include <op/naive_filter/naive_filter_plan.h>
#include <op/naive_filter/naive_filter_action.h>

#include <op/index_filter/index_filter_plan.h>
#include <op/index_filter/index_filter_action.h>

#include <compression/test_qry_spiht.h>

namespace msdb
{
namespace caDummy
{
pArray exe_act_ind_naive_filter(_vectorSourceArray_, pPredicate myPredicate, pQuery qry = nullptr);

pArray exe_act_ind_index_filter(_vectorSourceArray_, pPredicate myPredicate, pQuery qry = nullptr);

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

	BOOST_LOG_TRIVIAL(info) << "Filtered values: " << numValues << std::endl;
	return true;
}

template <typename value_type>
pArray exe_qbundle_seq_load_naive_filter(_vectorSourceArray_,
										 pPredicate myPredicate,
										 bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	exe_qry_ind_save<value_type>(sourceArr, false);

	//////////////////////////////
	// 02. Naive Filter Array
	return exe_qry_seq_load_naive_filter<value_type>(sourceArr,
													 myPredicate,
													 printFlag);
}

template <typename value_type>
pArray exe_qbundle_seq_load_index_filter(_vectorSourceArray_,
										 pPredicate inPredicate,
										 eleDefault mmtLevel,
										 bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	exe_qry_ind_save<value_type>(sourceArr, false);
	exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);

	//////////////////////////////
	// 02. Index Filter Array
	return exe_qry_seq_load_index_filter<value_type>(sourceArr,
													 inPredicate,
													 printFlag);
}

template <typename value_type>
pArray exe_qbundle_spiht_seq_index_filter(_vectorSourceArray_,
										  pPredicate inPredicate,
										  eleDefault wtLevel,
										  eleDefault mmtLevel,
										  bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	exe_qry_ind_spiht_encode<value_type>(sourceArr, wtLevel, false);

	//////////////////////////////
	// 02 Naive Filter Arrary
	return exe_qry_seq_spiht_naive_filter<value_type>(sourceArr,
													  inPredicate,
													  wtLevel, mmtLevel,
													  printFlag);
}

template <typename value_type>
pArray exe_qbundle_seq_se_index_filter(_vectorSourceArray_,
									   pPredicate inPredicate,
									   eleDefault wtLevel,
									   eleDefault mmtLevel,
									   bool printFlag = false)
{
	//////////////////////////////
	// 01. Save Source Array
	exe_qry_ind_mmt_build<value_type>(sourceArr, mmtLevel, false);
	exe_qry_ind_se_compression<value_type>(sourceArr, wtLevel, mmtLevel, false);

	//////////////////////////////
	// 02. Index Filter Array
	return exe_qry_seq_se_index_filter<value_type>(sourceArr,
												   inPredicate,
												   wtLevel, mmtLevel,
												   printFlag);
}

template <typename value_type>
pArray exe_qry_ind_naive_filter(_vectorSourceArray_,
								pPredicate inPredicate,
								bool printFlag = false)
{
	auto outArr = exe_act_ind_naive_filter(sourceArr, inPredicate);
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Filtered Arr" << std::endl;
		outArr->print();
	}

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_load_naive_filter(_vectorSourceArray_,
									 pPredicate inPredicate,
									 bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLoadPlan(sourceArr[0]->getDesc(), qry);
	auto filterPlan = getNaiveFilterPlan(loadPlan, inPredicate, qry);

	auto loadAction = loadPlan->getAction();
	auto outArr = loadAction->execute(sourceArr, qry);
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

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_load_index_filter(_vectorSourceArray_,
									 pPredicate inPredicate,
									 bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto loadPlan = getLoadPlan(sourceArr[0]->getDesc(), qry);
	auto filterPlan = getIndexFilterPlan(loadPlan, inPredicate, qry);

	auto loadAction = loadPlan->getAction();
	auto outArr = loadAction->execute(sourceArr, qry);
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

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_spiht_naive_filter(_vectorSourceArray_,
									  pPredicate inPredicate,
									  eleDefault wtLevel,
									  eleDefault mmtLevel,
									  bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto spihtPlan = getSPIHTDecodePlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(spihtPlan, wtLevel, qry);
	auto filterPlan = getNaiveFilterPlan(wtDecodePlan, inPredicate, qry);

	auto outArr = spihtPlan->getAction()->execute(sourceArr, qry);
	outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Se Decomp Arr" << std::endl;
		outArr->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wt Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}

template <typename value_type>
pArray exe_qry_seq_se_index_filter(_vectorSourceArray_,
								   pPredicate inPredicate,
								   eleDefault wtLevel,
								   eleDefault mmtLevel,
								   bool printFlag = false)
{
	pQuery qry = std::make_shared<query>();

	auto seDecompPlan = getSeDecompressionPlan(sourceArr[0]->getDesc(), wtLevel, qry);
	auto wtDecodePlan = getWaveletDecodePlan(seDecompPlan, wtLevel, qry);
	auto deltaDecodePlan = getMMTDeltaDecodePlan(wtDecodePlan, qry);
	auto filterPlan = getIndexFilterPlan(deltaDecodePlan, inPredicate, qry);

	auto outArr = seDecompPlan->getAction()->execute(sourceArr, qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Se Decomp Arr" << std::endl;
		outArr->print();
	}

	outArr = wtDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Wt Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = deltaDecodePlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Delta Decode Arr" << std::endl;
		outArr->print();
	}

	outArr = filterPlan->getAction()->execute(std::vector<pArray>({ outArr }), qry);
	//outArr->getChunkBitmap()->print();
	if (printFlag)
	{
		std::cout << "##############################" << std::endl;
		std::cout << "Filtered Arr" << std::endl;
		outArr->print();
	}

	tearDownQuery(qry);

	return outArr;
}



template <typename value_type>
pArray test_body_seq_load_naive_filter(_pFuncGetSourceArray_, int64_t value,
									   bool printFlag)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	//////////////////////////////

	//////////////////////////////
	// 02-1. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_load_naive_filter<value_type>(sourceArr,
																myNaivePredicate, false);
	// 02-2. Evaluate Result
	equalTest<value_type>(outArr, value);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_load_index_filter(_pFuncGetSourceArray_,
									   _pFuncGetSourceArrayDesc_,
									   eleDefault mmtLevel, int64_t value,
									   bool printFlag)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	//////////////////////////////

	//////////////////////////////
	// 02-1. Index Filter Array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_load_index_filter<value_type>(sourceArr,
																myPredicate, mmtLevel, printFlag);
	// 02-2. Evaluate Result
	equalTest<value_type>(outArr, value);
	//////////////////////////////

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
															 myNaivePredicate, false);
	//////////////////////////////

	//////////////////////////////
	// 04. Evaluation
	compArrary<value_type>(filterOutArr, outArr);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_spiht_index_filter(_pFuncGetSourceArray_,
										_pFuncGetSourceArrayDesc_,
										eleDefault wtLevel, eleDefault mmtLevel,
										int64_t value, bool printFlag)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 3);
	//////////////////////////////

	//////////////////////////////
	// 02-1. Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_spiht_seq_index_filter<value_type>(sourceArr,
																 myPredicate, wtLevel, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
															 myNaivePredicate, false);
	//////////////////////////////

	//////////////////////////////
	// 04. Evaluation
	compArrary<value_type>(filterOutArr, outArr);
	//////////////////////////////

	return outArr;
}

template <typename value_type>
pArray test_body_seq_se_index_filter(_pFuncGetSourceArray_,
									 _pFuncGetSourceArrayDesc_,
									 eleDefault wtLevel, eleDefault mmtLevel,
									 int64_t value, bool printFlag)
{
	//////////////////////////////
	// 01. Get Source Array
	std::vector<pArray> sourceArr;
	getSourceArrayIfEmpty(sourceArr);
	sourceArr[0]->setId(sourceArr[0]->getId() + 2);
	//////////////////////////////

	//////////////////////////////
	// 02-1. Se Index filter array
	pPredicate myPredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto outArr = exe_qbundle_seq_se_index_filter<value_type>(sourceArr,
															  myPredicate, wtLevel, mmtLevel, printFlag);
	equalTest<value_type>(outArr, value);
	//////////////////////////////

	//////////////////////////////
	// 03. Naive Filter Array
	pPredicate myNaivePredicate = std::make_shared<singlePredicate>(getEqualTerm(value));
	auto filterOutArr = exe_qry_ind_naive_filter<value_type>(sourceArr,
															 myNaivePredicate, false);
	//////////////////////////////

	//////////////////////////////
	// 04. Evaluation
	//BOOST_LOG_TRIVIAL(debug) << "Array: " << outArr->getDesc()->name_;
	//auto naiveBitmap = filterOutArr->getChunkBitmap();
	//BOOST_LOG_TRIVIAL(debug) << "naiveBitmap";
	//naiveBitmap->print();

	//auto seBitmap = outArr->getChunkBitmap();
	//BOOST_LOG_TRIVIAL(debug) << "seBitmap";
	//seBitmap->print();

	compArrary<value_type>(filterOutArr, outArr);
	//////////////////////////////

	return outArr;
}
}		// caDummy
}		// msdb
#endif	//_MSDB_TESTFILTER_H_
