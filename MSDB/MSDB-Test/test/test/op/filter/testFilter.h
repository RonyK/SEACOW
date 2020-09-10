#pragma once
#ifndef _MSDB_TESTFILTER_H_
#define _MSDB_TESTFILTER_H_

#include <pch.h>

#include <op/naive_filter/naive_filter_plan.h>
#include <op/naive_filter/naive_filter_action.h>

namespace msdb
{
namespace caDummy
{
pArray naive_filter(_vectorSourceArray_, pPredicate myPredicate);
}		// caDummy
}		// msdb
#endif	//_MSDB_TESTFILTER_H_
