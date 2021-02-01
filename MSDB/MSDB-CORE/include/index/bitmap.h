#pragma once
#ifndef _MSDB_BITMAP_H_
#define _MSDB_BITMAP_H_

#include <stdafx.h>
#include <util/coordinate.h>

namespace msdb
{
class bitmap;
using pBitmap = std::shared_ptr<bitmap>;
using cpBitmap = std::shared_ptr<const bitmap>;

class bitmapTree;
using pBitmapTree = std::shared_ptr<bitmapTree>;
using cpBitmapTree = std::shared_ptr<const bitmapTree>;

class bitmap
{
public:
	bitmap(const size_t capacity, const bool value = false);
	//bitmap(const coor& space, const bool value = false);
	bitmap(const bitmap& mit);

public:
	bool isExist(const size_t seqPos) const;
	void setExist(const size_t seqPos);
	void setNull(const size_t seqPos);

	size_t getCapacity() const;
	virtual bool isTree() const;

	void andMerge(bitmap& mit);
	void orMerge(bitmap& mit);

	void print() const;
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
	cpBitmap getChild(const size_t seqPos) const;
	bool hasChild(const size_t seqPos) const;

	virtual bool isTree();

protected:
	std::vector<pBitmap> childs_;
};
}		// msdb
#endif	// _MSDB_BITMAP_H_