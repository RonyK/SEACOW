#include <op/mmt_delta_encode/mmt_delta_encode_action.h>
#include <op/mmt_delta_encode/mmt_delta_encode_array.h>

namespace msdb
{
mmt_delta_encode_action::mmt_delta_encode_action()
{
}

mmt_delta_encode_action::~mmt_delta_encode_action()
{
}

const char* mmt_delta_encode_action::name()
{
	return "msdb_delta_encode_action";
}

pArray mmt_delta_encode_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	assert(inputArrays.size() == 1);

	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getArrayId();

	auto deltaArr = std::make_shared<mmt_delta_encode_array>(this->getArrayDesc());

	for (auto attr : sourceArr->getDesc()->attrDescs_)
	{
		switch(attr->type_)
		{
		case eleType::CHAR:
			this->attributeEncode<char>(deltaArr, sourceArr, attr);
			break;
		case eleType::INT8:
			this->attributeEncode<int8_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::INT16:
			this->attributeEncode<int16_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::INT32:
			this->attributeEncode<int32_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::INT64:
			this->attributeEncode<int64_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::UINT8:
			this->attributeEncode<uint8_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::UINT16:
			this->attributeEncode<uint16_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::UINT32:
			this->attributeEncode<uint32_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::UINT64:
			this->attributeEncode<uint64_t>(deltaArr, sourceArr, attr);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	return deltaArr;
}


}	// msdb
