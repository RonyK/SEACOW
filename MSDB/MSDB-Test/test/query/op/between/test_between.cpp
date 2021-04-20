#include <pch.h>

#include <op/between/between_plan.h>
#include <op/between/between_action.h>

#include <query/op/between/test_body_between.h>

namespace msdb
{
namespace caDummy
{
namespace between
{
coor sp = { 0, 125 };
coor ep = { 6, 129 };

size_t numTests = 3;
std::vector<float> selectivities = { 0.01, 0.1, 1, 10, 20, 40 };

bool saveArray = false;
bool printFlag = false;
bool validation = false;

namespace expRandom
{
size_t expId = 100;
std::string expName = "ran_between";
}
}

// ##################################################
// # star1024x1024
namespace data2D_star1024x1024
{
TEST(query_op_between, star1024x1024_ind_raw_between)
{
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, between::printFlag);

	parameters params = {
		std::make_shared<opParamArray>(sourceArr[0]->getDesc()),
		std::make_shared<opParamCoor>(std::make_shared<coor>(between::sp)),
		std::make_shared<opParamCoor>(std::make_shared<coor>(between::ep))
	};

	auto betweenQuery = std::make_shared<query>();
	auto betweenPlan = std::make_shared<between_plan>();
	betweenPlan->setParamSet(std::make_shared<between_array_pset>(params));
	auto betweenAction = std::static_pointer_cast<between_action>(betweenPlan->getAction());

	auto outArr = betweenAction->execute(sourceArr, betweenQuery);
	auto it = sourceArr[0]->getChunkIterator();
	++(*it);
	(**it)->print();
	outArr->print();
}	// TEST()

TEST(query_op_between, star1024x1024_seq_se_between)
{
	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 between::sp, between::ep,
										 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, star1024x1024_seq_spiht_between)
{
	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc, wtLevel,
											between::sp, between::ep,
											between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, star1024x1024_seq_load_between)
{
	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc,
										   between::sp, between::ep,
										   between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, star1024x1024_seq_random_load_between)
{
	test_body_seq_random_load_between<value_type>(&getSourceArrayIfEmpty,
												  &getSourceArrayDesc,
												  between::numTests, between::selectivities,
												  dimX, dimY,
												  between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, star1024x1024_seq_random_spiht_between)
{
	test_body_seq_random_spiht_between<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc, wtLevel,
												   between::numTests, between::selectivities,
												   dimX, dimY,
												   between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, star1024x1024_seq_random_se_between)
{
	test_body_seq_random_se_between<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, wtLevel, mmtLevel,
												between::numTests, between::selectivities,
												dimX, dimY,
												between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, star1024x1024_seq_random_compass_between)
{
	test_body_seq_random_compass_between<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, numBins,
													 between::numTests, between::selectivities,
													 dimX, dimY,
													 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, star1024x1024_seq_random_zip_between)
{
	test_body_seq_random_zip_between<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 between::numTests, between::selectivities,
												 dimX, dimY,
												 between::saveArray, between::validation, between::printFlag);
}	// TEST()
}	// data2D_star1024x1024
// ##################################################

// ##################################################
// # saturn1024x1024
namespace data2D_saturn1024x1024
{
TEST(query_op_between, saturn1024x1024_seq_se_between)
{
	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 between::sp, between::ep,
										 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, saturn1024x1024_seq_spiht_between)
{
	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc, wtLevel,
											between::sp, between::ep,
											between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, saturn1024x1024_seq_load_between)
{
	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc,
										   between::sp, between::ep,
										   between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, saturn1024x1024_random_load_between)
{
	test_body_seq_random_load_between<value_type>(&getSourceArrayIfEmpty,
												  &getSourceArrayDesc,
												  between::numTests, between::selectivities,
												  dimX, dimY,
												  between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, saturn1024x1024_seq_random_spiht_between)
{
	test_body_seq_random_spiht_between<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc, wtLevel,
												   between::numTests, between::selectivities,
												   dimX, dimY,
												   between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, saturn1024x1024_seq_random_se_between)
{
	test_body_seq_random_se_between<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, wtLevel, mmtLevel,
												between::numTests, between::selectivities,
												dimX, dimY,
												between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, saturn1024x1024_seq_random_compass_between)
{
	test_body_seq_random_compass_between<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, numBins,
													 between::numTests, between::selectivities,
													 dimX, dimY,
													 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, saturn1024x1024_seq_random_zip_between)
{
	test_body_seq_random_zip_between<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 between::numTests, between::selectivities,
												 dimX, dimY,
												 between::saveArray, between::validation, between::printFlag);
}	// TEST()
}	// data2D_saturn1024x1024
// ##################################################

// ##################################################
// # solar1024x1024
namespace data2D_solar1024x1024
{
TEST(query_op_between, solar1024x1024_seq_se_between)
{
	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 between::sp, between::ep,
										 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, solar1024x1024_seq_spiht_between)
{
	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc, wtLevel,
											between::sp, between::ep,
											between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, solar1024x1024_seq_load_between)
{
	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc,
										   between::sp, between::ep,
										   between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, solar1024x1024_seq_random_load_between)
{
	test_body_seq_random_load_between<value_type>(&getSourceArrayIfEmpty,
												  &getSourceArrayDesc,
												  between::numTests, between::selectivities,
												  dimX, dimY,
												  between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, solar1024x1024_seq_random_spiht_between)
{
	test_body_seq_random_spiht_between<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc, wtLevel,
												   between::numTests, between::selectivities,
												   dimX, dimY,
												   between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, solar1024x1024_seq_random_se_between)
{
	test_body_seq_random_se_between<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, wtLevel, mmtLevel,
												between::numTests, between::selectivities,
												dimX, dimY,
												between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, solar1024x1024_seq_random_compass_between)
{
	test_body_seq_random_compass_between<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, numBins,
													 between::numTests, between::selectivities,
													 dimX, dimY,
													 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, solar1024x1024_seq_random_zip_between)
{
	test_body_seq_random_zip_between<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 between::numTests, between::selectivities,
												 dimX, dimY,
												 between::saveArray, between::validation, between::printFlag);
}	// TEST()
}	// data2D_solar1024x1024
// ##################################################

// ##################################################
// # mars4096x2048
namespace data2D_mars4096x2048
{
TEST(query_op_between, mars4096x2048_seq_se_between)
{
	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 between::sp, between::ep,
										 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, mars4096x2048_seq_spiht_between)
{
	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc, wtLevel,
											between::sp, between::ep,
											between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, mars4096x2048_seq_load_between)
{
	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc,
										   between::sp, between::ep, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mars4096x2048_seq_random_load_between)
{
	test_body_seq_random_load_between<value_type>(&getSourceArrayIfEmpty,
												  &getSourceArrayDesc,
												  between::numTests, between::selectivities,
												  dimX, dimY,
												  between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mars4096x2048_seq_random_spiht_between)
{
	test_body_seq_random_spiht_between<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc, wtLevel,
												   between::numTests, between::selectivities,
												   dimX, dimY,
												   between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mars4096x2048_seq_random_se_between)
{
	test_body_seq_random_se_between<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, wtLevel, mmtLevel,
												between::numTests, between::selectivities,
												dimX, dimY,
												between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mars4096x2048_seq_random_compass_between)
{
	test_body_seq_random_compass_between<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, numBins,
													 between::numTests, between::selectivities,
													 dimX, dimY,
													 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mars4096x2048_seq_random_zip_between)
{
	test_body_seq_random_zip_between<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 between::numTests, between::selectivities,
												 dimX, dimY,
												 between::saveArray, between::validation, between::printFlag);
}	// TEST()
}	// data2D_mars4096x2048
// ##################################################

// ##################################################
// # data2D_mercurydem20480x10240
namespace data2D_mercurydem20480x10240
{
TEST(query_op_between, mercurydem20480x10240_seq_se_between)
{
	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 between::sp, between::ep,
										 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, mercurydem20480x10240_seq_spiht_between)
{
	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc, wtLevel,
											between::sp, between::ep,
											between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, mercurydem20480x10240_seq_load_between)
{
	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc,
										   between::sp, between::ep, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mercurydem20480x10240_seq_random_load_between)
{
	test_body_seq_random_load_between<value_type>(&getSourceArrayIfEmpty,
												  &getSourceArrayDesc,
												  between::numTests, between::selectivities,
												  dimX, dimY,
												  between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mercurydem20480x10240_seq_random_spiht_between)
{
	test_body_seq_random_spiht_between<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc, wtLevel,
												   between::numTests, between::selectivities,
												   dimX, dimY,
												   between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mercurydem20480x10240_seq_random_se_between)
{
	test_body_seq_random_se_between<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, wtLevel, mmtLevel,
												between::numTests, between::selectivities,
												dimX, dimY,
												between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mercurydem20480x10240_seq_random_compass_between)
{
	test_body_seq_random_compass_between<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, numBins,
													 between::numTests, between::selectivities,
													 dimX, dimY,
													 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, mercurydem20480x10240_seq_random_zip_between)
{
	test_body_seq_random_zip_between<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 between::numTests, between::selectivities,
												 dimX, dimY,
												 between::saveArray, between::validation, between::printFlag);
}	// TEST()
}	// data2D_mercurydem20480x10240
// ##################################################

// ##################################################
// # data2D_lunar102400x40960
namespace data2D_lunar102400x40960
{
TEST(query_op_between, lunar102400x40960_seq_se_between)
{
	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 between::sp, between::ep,
										 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, lunar102400x40960_seq_spiht_between)
{
	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc, wtLevel,
											between::sp, between::ep,
											between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(query_op_between, lunar102400x40960_seq_load_between)
{
	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc,
										   between::sp, between::ep, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, lunar102400x40960_seq_random_load_between)
{
	test_body_seq_random_load_between<value_type>(&getSourceArrayIfEmpty,
												  &getSourceArrayDesc,
												  between::numTests, between::selectivities,
												  dimX, dimY,
												  between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, lunar102400x40960_seq_random_spiht_between)
{
	test_body_seq_random_spiht_between<value_type>(&getSourceArrayIfEmpty,
												   &getSourceArrayDesc, wtLevel,
												   between::numTests, between::selectivities,
												   dimX, dimY,
												   between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, lunar102400x40960_seq_random_se_between)
{
	test_body_seq_random_se_between<value_type>(&getSourceArrayIfEmpty,
												&getSourceArrayDesc, wtLevel, mmtLevel,
												between::numTests, between::selectivities,
												dimX, dimY,
												between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, lunar102400x40960_seq_random_compass_between)
{
	test_body_seq_random_compass_between<value_type>(&getSourceArrayIfEmpty,
													 &getSourceArrayDesc, numBins,
													 between::numTests, between::selectivities,
													 dimX, dimY,
													 between::saveArray, between::validation, between::printFlag);
}	// TEST()

TEST(experiment_query_op_between, lunar102400x40960_seq_random_zip_between)
{
	test_body_seq_random_zip_between<value_type>(&getSourceArrayIfEmpty,
												 &getSourceArrayDesc,
												 between::numTests, between::selectivities,
												 dimX, dimY,
												 between::saveArray, between::validation, between::printFlag);
}	// TEST()
}	// data2D_lunar102400x40960
// ##################################################
}	// caDummy
}	// msdb