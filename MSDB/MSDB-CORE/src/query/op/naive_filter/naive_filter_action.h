#pragma once
#ifndef _MSDB_OP_NAIVE_FILTER_ACTION_H_
#define _MSDB_OP_NAIVE_FILTER_ACTION_H_

#include <query/opAction.h>

namespace msdb
{
class naive_filter_action : public opAction
{
public:
	naive_filter_action();
	virtual ~naive_filter_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);

private:
	template<class Ty_>
	void filter(pArray inputArray)
	{
		auto cItr = inputArray->getChunkIterator();

		while (!cItr->isEnd()) {
			auto bItr = (**cItr)->getBlockIterator();
			while (!bItr->isEnd()) {
				auto itemItr = (**bItr)->getItemIterator();
				while (!itemItr->isEnd())
				{
					auto data = (**itemItr).get<Ty_>();
					// expression.....
					++(*itemItr);
				}

				++(*bItr);
			}
			++(*cItr);
		}
	}
};
}

#endif // _MSDB_OP_NAIVE_FILTER_ACTION_H_
