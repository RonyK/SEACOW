#pragma once
#ifndef _MSDB_OP_MMT_ENCODE_ARRAY_H_
#define	_MSDB_OP_MMT_ENCODE_ARRAY_H_

#include <array/array.h>
#include <io/bitstream.h>
#include <util/coordinate.h>
#include <compression/mmt.h>

namespace msdb
{
class mmt_encode_array : public arrayBase
{
public:
	using base_type = arrayBase;
	using serial_mmt_holder_type = std::shared_ptr<serializedMMTHolder>;

public:

protected:
	size_t maxLevel_;
	serial_mmt_holder_type serialMmtHolder_;
};
}	// msdb

#endif		// _MSDB_OP_MMT_ENCODE_ARRAY_H_