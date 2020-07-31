#pragma once
#ifndef _MSDB_OP_SPIHT_DECODE_ACTION_H
#define _MSDB_OP_SPIHT_DECODE_ACTION_H

#include <query/opAction.h>
#include <util/math.h>
#include <vector>
#include <list>

namespace msdb
{
class spiht_decode_action : public opAction
{
public:
	spiht_decode_action();
	virtual ~spiht_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& outputArrays, pQuery q);

private:
	void decode_init(size_t dSize, std::vector<size_t> bandSize);
	template<class Ty_>
	void decode_progress(size_t dSize, std::vector<position_t> cSize, std::vector<size_t> bandSize, chunkItemIterator itemItr);

	template<class Ty_>
	void decode_sigpass(size_t dSize, std::vector<position_t> cSize, std::vector<size_t> bandSize,
		chunkItemIterator itemItr, Ty_ signBit, Ty_ stepBit);
	template<class Ty_>
	void decode_refinepass(chunkItemIterator itemItr, Ty_ stepBit, size_t LSP_size);

private:
	std::list<coor> LIP_;
	std::list<coor> LIS_;
	std::list<char> LIS_TYPE_;
	std::list<coor> LSP_;

	std::list<int> code;		// TODO
};
}


#endif // _MSDB_OP_SPIHT_DECODE_ACTION_H
