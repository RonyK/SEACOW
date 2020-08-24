#pragma once
#ifndef _MSDB_BITMAP_H_
#define _MSDB_BITMAP_H_

#include <util/coordinate.h>
#include <vector>

namespace msdb
{
class bitmap;
using pBitmap = std::shared_ptr<bitmap>;

class bitmapTree;
using pBitmapTree = std::shared_ptr<bitmapTree>;

class bitmap
{
public:
	bitmap(const size_t capacity, const bool value = false);
	//bitmap(const coor& space, const bool value = false);
	bitmap(const bitmap& mit);

public:
	//void setSpace(const coor& space);

	bool isExist(const size_t seqPos) const;
	//bool isExist(const coor& pos) const;

	void setExist(const size_t seqPos);
	//void setExist(const coor& pos);

	void setNull(const size_t seqPos);
	//void setNull(const coor& pos);

	size_t getCapacity() const;

	virtual bool isTree();

public:
	std::vector<bool>::reference operator[](size_t seqPos);
	const bool& operator[](size_t seqPos) const;

protected:
	//inline size_t getSeqPos(const coor& pos) const;

protected:
	std::vector<bool> data_;
	//coorItr it_;
};

class bitmapTree : public bitmap
{
public:
	bitmapTree(const size_t capacity, const bool value = false);
	//bitmapTree(const coor& space, const bool value = false);
	bitmapTree(const bitmapTree& mit);

public:
	pBitmap makeChild(const size_t seqPos, const size_t capacity, const bool value = false);
	pBitmap getChild(const size_t seqPos);

	virtual bool isTree();

protected:
	std::vector<pBitmap> childs_;
};
}		// msdb
#endif	// _MSDB_BITMAP_H_