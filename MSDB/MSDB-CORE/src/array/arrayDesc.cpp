#include <array/arrayDesc.h>

msdb::arrayDesc::arrayDesc()
{
	// TODO::
}

msdb::arrayDesc::arrayDesc(const arrayId aid, const std::string arrayName,
						   const dimensionDescs& dimDescs, 
						   const attributeDescs& attrDescs)
	: id_(aid), name_(arrayName), dimDescs_(dimDescs), attrDescs_(attrDescs)
{
}

msdb::arrayDesc::arrayDesc(const arrayDesc& mit)
	: id_(mit.id_), name_(mit.name_)
{
	for(size_t i = 0; i < mit.dimDescs_.size(); i++)
	{
		this->dimDescs_.push_back(std::make_shared<dimensionDesc>(*mit.dimDescs_[i]));
	}

	for (size_t i = 0; i < mit.attrDescs_.size(); i++)
	{
		this->attrDescs_.push_back(std::make_shared<attributeDesc>(*mit.attrDescs_[i]));
	}
}

msdb::arrayDesc::~arrayDesc()
{
	// TODO::
}
