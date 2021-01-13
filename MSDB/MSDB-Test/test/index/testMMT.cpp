#include <pch.h>
#include <index/mmtNode.h>

namespace msdb
{
template <typename Ty_>
pMmtNode getMmtNode(Ty_ realMin, Ty_ realMax, bit_cnt_type bMinDelta, bit_cnt_type bMaxDelta, bit_cnt_type order, bit_cnt_type childOrder, bool finished = false)
{
	pMmtNode curNode = std::make_shared<mmtNode>();

	curNode->realMin_ = realMin;
	curNode->realMax_ = realMax;
	curNode->bMinDelta_ = bMinDelta;
	curNode->bMaxDelta_ = bMaxDelta;
	curNode->order_ = order;
	curNode->childOrder_ = childOrder;
	curNode->finished_ = finished;

	curNode->bMin_ = msb<Ty_>(abs_(realMin), order) * SIGN(realMin);
	curNode->bMax_ = msb<Ty_>(abs_(realMax), order) * SIGN(realMax);

	curNode->min_ = getMinBoundary<Ty_>(realMin, curNode->order_, curNode->bMin_);
	curNode->max_ = getMaxBoundary<Ty_>(realMax, curNode->order_, curNode->bMax_);

	curNode->bits_ = std::max({abs_(msb<Ty_>(realMin, order)), abs_(msb<Ty_>(realMax, order))});;

	return curNode;
}

template <typename Ty_>
pMmtNode getMmtNode(Ty_ realMin, Ty_ realMax, Ty_ prevMin, Ty_ prevMax, bit_cnt_type bMinDelta, bit_cnt_type bMaxDelta, bit_cnt_type order, bit_cnt_type childOrder, bool finished = false)
{
	pMmtNode curNode = getMmtNode(realMin, realMax, bMinDelta, bMaxDelta, order, childOrder, finished);

	curNode->min_ = getMinBoundary<Ty_>(prevMin, curNode->order_, curNode->bMin_);
	curNode->max_ = getMaxBoundary<Ty_>(prevMax, curNode->order_, curNode->bMax_);

	return curNode;
}

TEST(mmt_update_childOrderChanged, nodeUpdateWhenChildOrderChanged)
{
	pMmtNode curNode = getMmtNode<char>(-124, -77, -127, 127, 0, 14, 1, 2);

	nodeUpdateWhenChildOrderChanged<char>(curNode, 0, -124);
	EXPECT_EQ(curNode->getMin<char>(), -127);
	EXPECT_EQ(curNode->getMax<char>(), -64);

	nodeUpdateWhenChildOrderChanged<char>(curNode, 0, -77);
	EXPECT_EQ(curNode->getMin<char>(), -127);
	EXPECT_EQ(curNode->getMax<char>(), -64);
}
}