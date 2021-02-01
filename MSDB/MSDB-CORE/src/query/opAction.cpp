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
void opAction::threadCreate(size_t threadNums)
{
	this->io_service_ = std::make_shared<boost::asio::io_service>();
	this->work_ = std::make_shared<boost::asio::io_service::work>(*(this->io_service_));

	for(size_t i = 0; i < threadNums; ++i)
	{
		this->threadpool_.create_thread(boost::bind(&boost::asio::io_service::run, this->io_service_));
	}
}
void opAction::threadStop()
{
	this->io_service_->stop();
}
void opAction::threadJoin()
{
	this->threadpool_.join_all();
}
}
