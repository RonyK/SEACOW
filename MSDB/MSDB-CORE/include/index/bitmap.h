#pragma once
#ifndef _MSDB_BITMAP_H_
#define _MSDB_BITMAP_H_

#include <util/coordinate.h>
#include <vector>

namespace msdb
{
class bitmap
{
public:
	//bitmap();
	bitmap(const size_t capacity);
	bitmap(const coor& space);

public:
	bool isExist(const size_t seqPos) const;
	bool isExist(const coor& pos) const;

	void setExist(const size_t seqPos);
	void setExist(const coor& pos);

	void setNull(const size_t seqPos);
	void setNull(const coor& pos);

	size_t getCapacity() const;

	//void setSpace(const coor& space);

	//itemIterator<position_t, bool> getItemIterator();

public:
	std::vector<bool>::reference operator[](size_t seqPos);
	const bool& operator[](size_t seqPos) const;

protected:
	inline size_t getSeqPos(const coor& pos) const;

protected:
	std::vector<bool> data_;
	coorItr it_;
};
}		// msdb
#endif	// _MSDB_BITMAP_H_