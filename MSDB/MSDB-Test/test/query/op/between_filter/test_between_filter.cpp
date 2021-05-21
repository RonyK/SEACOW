#include <pch.h>
#include "qry_between_filter.h"

namespace msdb
{
namespace caDummy
{
namespace between_filter
{
int64_t numTests = 3;
std::vector<float> selectivities = { 0.01, 0.1, 1};

bool saveArray = false;
bool printFlag = false;
bool validation = true;
}
// ##################################################
// # star1024x1024
namespace data2D_star1024x1024
{
TEST(experiment_query_op_between_filter, star1024x1024_seq_load_naive_filter)
{
	test_body_seq_load_random_between_filter<value_type>(&getSourceArrayIfEmpty,
														 &getSourceArrayDesc,
														 between_filter::numTests, 
														 between_filter::selectivities, dimX, dimY,
														 between_filter::saveArray, between_filter::validation, between_filter::printFlag);

}	// TEST()
}	// data2D_star1024x1024
// ##################################################
}	// caDummy
}	// msdb