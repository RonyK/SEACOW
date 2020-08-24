#include <index/bitmap.h>

namespace msdb
{
//bitmap::bitmap()
//	: it_(1), data_()
//{
//}
//bitmap::bitmap(const size_t capacity, const bool value)
//	: it_(1), data_(capacity, value)
//{
//}
//bitmap::bitmap(const coor& space, const bool value)
//	: it_(space), data_(space.area(), value)
//{
//}
//bitmap::bitmap(const bitmap& mit)
//	: it_(mit.it_), data_(mit.data_)
//{
//}
bitmap::bitmap(const size_t capacity, const bool value)
	: data_(capacity, value)
{
}
bitmap::bitmap(const bitmap& mit)
	: data_(mit.data_)
{
}
//void bitmap::setSpace(const coor& space)
//{
//	this->it_ = coorItr(space);
//	this->data_.resize(space.area(), false);
//}
bool bitmap::isExist(const size_t seqPos) const
{
	assert(seqPos < this->data_.size());
	return this->data_[seqPos];
}
//bool bitmap::isExist(const coor& pos) const
//{
//	return this->isExist(this->getSeqPos(pos));
//}

void bitmap::setExist(const size_t seqPos)
{
	assert(seqPos < this->data_.size());
	this->data_[seqPos] = true;
}
//void bitmap::setExist(const coor& pos)
//{
//	this->setExist(this->getSeqPos(pos));
//}

void bitmap::setNull(const size_t seqPos)
{
	assert(seqPos < this->data_.size());
	this->data_[seqPos] = false;
}
//void bitmap::setNull(const coor& pos)
//{
//	this->setNull(this->getSeqPos(pos));
//}

size_t bitmap::getCapacity() const
{
	return this->data_.size();
}
bool bitmap::isTree()
{
	return false;
}
std::vector<bool>::reference bitmap::operator[](size_t seqPos)
{
	assert(seqPos < this->data_.size());
	return this->data_[seqPos];
}

const bool& bitmap::operator[](size_t seqPos) const
{
	assert(seqPos < this->data_.size());
	return this->data_[seqPos];
}

//size_t bitmap::getSeqPos(const coor& pos) const
//{
//	coorItr it = this->it_;
//	it.moveTo(pos);
//	return it.seqPos();
//}

bitmapTree::bitmapTree(const size_t capacity, const bool value)
	: bitmap(capacity, value), childs_(capacity, nullptr)
{

}
//bitmapTree::bitmapTree(const coor& space, const bool value)
//	: bitmap(space, value), childs_(space.area(), nullptr)
//{
//}
bitmapTree::bitmapTree(const bitmapTree& mit)
	: bitmap(mit), childs_(mit.childs_)
{
	for(auto node : this->childs_)
	{
		if(node)
		{
			node = std::make_shared<bitmap>(*node);
		}
	}
}
pBitmap bitmapTree::makeChild(const size_t seqPos, const size_t capacity, const bool value)
{
	pBitmap child = std::make_shared<bitmap>(capacity, value);
	this->childs_[seqPos] = child;
	return child;
}
pBitmap bitmapTree::getChild(const size_t seqPos)
{
	assert(seqPos < this->childs_.size());
	return this->childs_[seqPos];
}
bool bitmapTree::isTree()
{
	return true;
}
}	// msdb