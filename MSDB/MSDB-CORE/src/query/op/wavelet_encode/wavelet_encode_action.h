#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_ACTION_H_
#define _MSDB_OP_WAVELET_ENCODE_ACTION_H_

#include <query/opAction.h>

namespace msdb
{
class wavelet_encode_action : public opAction
{
public:
	wavelet_encode_action();

public:
	virtual const char* name() override;
};
}

#endif		// _MSDB_OP_WAVELET_ENCODE_ACTION_H_