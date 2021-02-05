#include <pch.h>

#include <op/between/between_plan.h>
#include <op/between/between_action.h>

#include <query/op/between/test_body_between.h>

namespace msdb
{
namespace caDummy
{
namespace data2D_star1024x1024
{
TEST(query_op_between, star1024x1024_ind_raw_between)
{
	bool printFlag = false;
	auto sourceArr = getArrayFromFunction<value_type>(getSourceArrayIfEmpty, printFlag);

	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	parameters params = {
		std::make_shared<opParamArray>(sourceArr[0]->getDesc()),
		std::make_shared<opParamCoor>(std::make_shared<coor>(sp)),
		std::make_shared<opParamCoor>(std::make_shared<coor>(ep))
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

TEST(query_op_between, start1024x1024_seq_se_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 sp, ep, printFlag);
}	// TEST()

TEST(query_op_between, start1024x1024_seq_spiht_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 sp, ep, printFlag);
}	// TEST()

TEST(query_op_between, start1024x1024_seq_load_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 sp, ep, printFlag);
}	// TEST()
}	// data2D_star1024x1024

namespace data2D_saturn1024x1024
{
TEST(query_op_between, saturn1024x1024_seq_se_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 sp, ep, printFlag);
}	// TEST()

TEST(query_op_between, saturn1024x1024_seq_spiht_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc, wtLevel, mmtLevel,
											sp, ep, printFlag);
}	// TEST()

TEST(query_op_between, saturn1024x1024_seq_load_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc, wtLevel, mmtLevel,
										   sp, ep, printFlag);
}	// TEST()
}	// data2D_saturn1024x1024

namespace data2D_solar1024x1024
{
TEST(query_op_between, solar1024x1024_seq_se_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 sp, ep, printFlag);
}	// TEST()

TEST(query_op_between, solar1024x1024_seq_spiht_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc, wtLevel, mmtLevel,
											sp, ep, printFlag);
}	// TEST()

TEST(query_op_between, solar1024x1024_seq_load_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc, wtLevel, mmtLevel,
										   sp, ep, printFlag);
}	// TEST()
}	// data2D_solar1024x1024

namespace data2D_mars4096x2048
{
TEST(query_op_between, mars4096x2048_seq_se_between)
{
	bool printFlag = false;

	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_se_between<value_type>(&getSourceArrayIfEmpty,
										 &getSourceArrayDesc, wtLevel, mmtLevel,
										 sp, ep, printFlag);
}	// TEST()

TEST(query_op_between, mars4096x2048_seq_spiht_between)
{
	bool printFlag = false;
	coor sp = { 0, 125 };
	coor ep = { 6, 129 };

	test_body_seq_spiht_between<value_type>(&getSourceArrayIfEmpty,
											&getSourceArrayDesc, wtLevel, mmtLevel,
											sp, ep, printFlag);
}	// TEST()

TEST(query_op_between, mars4096x2048_seq_load_between)
{
	bool printFlag = false;

	float selectivity = 0.1;
	int area = dimX * dimY;
	float selectedArea = area * selectivity;

	int spX;
	int spY;
	int width;
	int height;

	while (1) {
		spX = rand() % dimX;
		spY = rand() % dimY;

		for (int i = 0; i < 3; i++) {
			width = rand() % (dimX - spX);
			if (width > selectedArea)
				continue;

			height = selectedArea / width;

			if (spX + width < dimX && spY + height < dimY)
				break;
		}

		if (spX + width < dimX && spY + height < dimY)
			break;
	}

	int epX = spX + width;
	int epY = spY + height;
	int cal = (epX - spX) * (epY - spY);

	coor sp = { spX, spY };
	coor ep = { spX + width, spY + height };

	test_body_seq_load_between<value_type>(&getSourceArrayIfEmpty,
										   &getSourceArrayDesc, wtLevel, mmtLevel,
										   sp, ep, printFlag);
}	// TEST()
}	// data2D_mars4096x2048
}	// caDummy
}	// msdb