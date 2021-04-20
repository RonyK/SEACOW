#pragma once
#ifndef _MSDB_EXP_EXPERIMENTSINFO_H_
#define _MSDB_EXP_EXPERIMENTSINFO_H_

#include <pch.h>

namespace msdb
{
namespace caDummy
{
typedef struct expInfo
{
	size_t expId_;
	size_t trialId_;
	size_t dataId_;
};

namespace experiments
{
namespace load_save_load
{
static const size_t expId = 10;
}		// load_save_load

namespace se_save_load
{
static const size_t expId = 20;
}		// se_save_load

namespace spiht_save_load
{
static const size_t expId = 40;
}		// spiht_save_load

namespace compass_save_load
{
static const size_t expId = 50;
}		// compass_save_load

namespace zip_save_load
{
static const size_t expId = 60;
}		// zip_save_load

namespace mmt_save_load
{
static const size_t expId = 90;
}		// mmt_save_load

namespace between_random
{
static const size_t expId = 100;
}		// between_random

namespace filter_random
{
static const size_t expId = 200;
}		// filter_random
}		// experiments
}		// caDummy
}		// msdb
#endif	// _MSDB_EXP_EXPERIMENTSINFO_H_