#include <array/arrayDesc.h>

msdb::arrayDesc::arrayDesc()
{
	// TODO::
}

msdb::arrayDesc::arrayDesc(const arrayId aid, const std::string arrayName,
						   const dimensionDescs* dimDescs, const attributeDescs* attrDescs)
	: id_(aid), name_(arrayName), dimDescs_(*dimDescs), attrDescs_(*attrDescs)
{
}

msdb::arrayDesc::~arrayDesc()
{
	// TODO::
}
