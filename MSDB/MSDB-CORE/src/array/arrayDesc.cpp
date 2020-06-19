#include <array/arrayDesc.h>

msdb::arrayDesc::arrayDesc()
{
	// TODO::
}

msdb::arrayDesc::arrayDesc(arrayId aid, std::string arrayName,
						   dimensionDescs& dimDescs, attributeDescs& attrDescs)
	: id_(aid), name_(arrayName), dimDescs_(dimDescs), attrDescs_(attrDescs)
{
}

msdb::arrayDesc::~arrayDesc()
{
	// TODO::
}
