#pragma once
#ifndef _MSDB_OP_SPIHT_ENCODE_ACTION_H_
#define _MSDB_OP_SPIHT_ENCODE_ACTION_H_

#include <query/opAction.h>
#include <util/math.h>
#include <vector>
#include <list>

namespace msdb
{
class spiht_encode_action : public opAction
{
public:
	spiht_encode_action();
	virtual ~spiht_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);
	std::list<int> getCode();

private:
	void encode_init(size_t dSize, std::vector<size_t> bandSize);
	template<class Ty_>
	void encode_progress(size_t dSize, std::vector<position_t> cSize, std::vector<size_t> bandSize, chunkItemIterator itemItr);

	template<class Ty_>
	void encode_sigpass(size_t dSize, std::vector<position_t> cSize, std::vector<size_t> bandSize, 
		chunkItemIterator itemItr, Ty_ signBit, Ty_ stepBit);
	template<class Ty_>
	void encode_refinepass(chunkItemIterator itemItr, Ty_ stepBit, size_t LSP_size);

private:
	std::list<coor> LIP_;
	std::list<coor> LIS_;
	std::list<char> LIS_TYPE_;
	std::list<coor> LSP_;

	std::list<int> code;		// TODO
};
}

#endif // _MSDB_OP_SPIHT_ENCODE_ACTION_H_