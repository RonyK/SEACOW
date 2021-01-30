#include <pch.h>

#include <compression/getOpCompression.h>

namespace msdb
{
namespace caDummy
{
void getOpZipSave(pArrayDesc sourceArrDesc, std::shared_ptr<zip_save_plan>& plan, std::shared_ptr<zip_save_action>& action, pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<zip_save_plan, zip_save_action, zip_save_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
void getOpZipLoad(pArrayDesc sourceArrDesc, std::shared_ptr<zip_load_plan>& plan, std::shared_ptr<zip_load_action>& action, pQuery& qry)
{
	auto planAction = getSingleArrayParamOperator<zip_load_plan, zip_load_action, zip_load_array_pset>(
		sourceArrDesc, qry);
	plan = std::get<0>(planAction);
	action = std::get<1>(planAction);
	qry = std::get<2>(planAction);
}
}	// caDummy
}	// msdb