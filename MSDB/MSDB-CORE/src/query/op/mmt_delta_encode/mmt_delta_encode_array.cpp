#include <op/mmt_delta_encode/mmt_delta_encode_array.h>

namespace msdb
{
mmt_delta_encode_array::mmt_delta_encode_array(pArrayDesc desc)
	: arrayBase(desc)
{
}
pMMT mmt_delta_encode_array::getMMT(attributeId attrId)
{
	return this->attrMMT[attrId];
}
void mmt_delta_encode_array::setMMT(attributeId attrId, pMMT myMMT)
{
	this->attrMMT[attrId] = myMMT;
}
}	// msdb
