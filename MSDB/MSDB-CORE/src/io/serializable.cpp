#include <io/serializable.h>

namespace msdb
{
serializable::serializable(pHeader myHeader)
	: myHeader_(myHeader)
{
	
}
pHeader serializable::getInHeader()
{
	this->updateFromHeader();
	return this->myHeader_;
}
pHeader serializable::getOutHeader()
{
	this->updateToHeader();
	return this->myHeader_;
}
pHeader serializable::getHeader()
{
	return this->myHeader_;
}
serialHeader::serialHeader()
	: size_(0), version_(0)
{
}
}	// msdb