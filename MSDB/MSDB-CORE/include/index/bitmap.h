#pragma once
#ifndef _MSDB_BITMAP_H_
#define _MSDB_BITMAP_H_

#include <vector>

namespace msdb
{
class bitmap
{
protected:
	std::vector<bool> data_;
};
}	// msdb
#endif	// _MSDB_BITMAP_H_