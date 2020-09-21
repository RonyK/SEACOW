#pragma once
#ifndef _MSDB_QUERY_H_
#define _MSDB_QUERY_H_

#include <util/timer.h>
#include <memory>

namespace msdb
{
class query;
using pQuery = std::shared_ptr<query>;

class query : std::enable_shared_from_this<query>
{
public:
	query();

public:
	pTimer getTimer();

protected:
	pTimer timer_;
};
}

#endif