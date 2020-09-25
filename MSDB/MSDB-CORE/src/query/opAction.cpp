#include <stdafx.h>
#include <query/opAction.h>

namespace msdb
{
opAction::opAction()
{
}

opAction::~opAction()
{
}

const pArrayDesc msdb::opAction::getArrayDesc()
{
	return this->aDesc_;
}

void opAction::setParams(const parameters& params)
{
	this->params_ = params;
}

void opAction::setArrayDesc(pArrayDesc aDesc)
{
	this->aDesc_ = aDesc;
}
void opAction::setPlanBitmap(pBitmap planBitmap)
{
	this->planBitmap_ = planBitmap;
}
cpBitmap opAction::getPlanChunkBitmap() const
{
	return this->planBitmap_;
}
cpBitmap opAction::getPlanBlockBitmap(chunkId cid) const
{
	if(this->planBitmap_->isTree())
	{
		return std::static_pointer_cast<bitmapTree>(this->planBitmap_)->getChild(cid);
	}
	return nullptr;
}
}
