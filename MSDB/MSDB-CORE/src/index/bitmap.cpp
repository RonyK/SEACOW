#include <index/bitmap.h>

namespace msdb
{
//bitmap::bitmap()
//	: it_(1), data_()
//{
//}
bitmap::bitmap(const size_t capacity)
	: it_(1), data_(capacity, false)
{
}
bitmap::bitmap(const coor& space)
	: it_(space), data_(space.area(), false)
{
}
bool bitmap::isExist(const size_t seqPos) const
{
	assert(seqPos < this->data_.size());
	return this->data_[seqPos];
}
bool bitmap::isExist(const coor& pos) const
{
	return this->isExist(this->getSeqPos(pos));
}

void bitmap::setExist(const size_t seqPos)
{
	assert(seqPos < this->data_.size());
	this->data_[seqPos] = true;
}
void bitmap::setExist(const coor& pos)
{
	this->setExist(this->getSeqPos(pos));
}

void bitmap::setNull(const size_t seqPos)
{
	assert(seqPos < this->data_.size());
	this->data_[seqPos] = false;
}
void bitmap::setNull(const coor& pos)
{
	this->setNull(this->getSeqPos(pos));
}

size_t bitmap::getCapacity() const
{
	return this->data_.size();
}

//void bitmap::setSpace(const coor& space)
//{
//	this->it_ = coorItr(space);
//}

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

size_t bitmap::getSeqPos(const coor& pos) const
{
	coorItr it = this->it_;
	it.moveTo(pos);
	return it.seqPos();
}
}	// msdb