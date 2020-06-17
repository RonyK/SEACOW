#include <array/arrayDesc.h>

msdb::arrayDesc::arrayDesc()
{
	// TODO::
}

msdb::arrayDesc::arrayDesc(arrayId aid, std::string arrayName,
						   attributeDescs& attrDescs, dimensionDescs& dimDescs)
	: id_(aid), name_(arrayName), attrDescs_(attrDescs), dimDescs_(dimDescs)
{
}

msdb::arrayDesc::~arrayDesc()
{
	// TODO::
}
