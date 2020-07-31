#pragma once
#include <pch.h>
#include <compression/testSPIHT.h>

/*
namespace msdb
{
	namespace caDummy
	{
		namespace data2D_spiht_sc4x4
		{
			pArray spiht_encode(std::vector<pArray> sourceArr)
			{
				getSourceArrayIfEmpty(sourceArr);

				eleDefault level = 1;
				std::shared_ptr<spiht_encode_plan> spihtPlan;
				std::shared_ptr<spiht_encode_action> spihtAction;
				pQuery mmtQuery;
				getSPIHTEncode(sourceArr[0]->getDesc(), spihtPlan, spihtAction, mmtQuery);

				// Execute mmt build action
				auto afterArray = spihtAction->execute(sourceArr, mmtQuery);

				return afterArray;
			}

			void getSPIHTEncode(pArrayDesc sourceArrDesc,
				std::shared_ptr<spiht_encode_plan>& plan,
				std::shared_ptr<spiht_encode_action>& action,
				pQuery& qry)
			{
				pQuery mmtQuery_ = std::make_shared<query>();
				auto mmtPlan_ = std::make_shared<spiht_encode_plan>();
				auto mmtAction_ = std::make_shared<spiht_encode_action>();
				parameters params = {
					std::make_shared<opParamArray>(sourceArrDesc),
				};
				auto pSet = std::make_shared<spiht_encode_pset>(params);

				mmtPlan_->setParamSet(pSet);
				mmtAction_->setArrayDesc(mmtPlan_->inferSchema());
				mmtAction_->setParams(params);

				plan = mmtPlan_;
				action = mmtAction_;
				qry = mmtQuery_;
			}
		}
	}
}
*/