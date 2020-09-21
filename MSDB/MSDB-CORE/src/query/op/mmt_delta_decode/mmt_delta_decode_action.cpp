#include <op/mmt_delta_decode/mmt_delta_decode_action.h>

namespace msdb
{
mmt_delta_decode_action::mmt_delta_decode_action()
{
}

mmt_delta_decode_action::~mmt_delta_decode_action()
{
}

const char* mmt_delta_decode_action::name()
{
	return "mmt_delta_decode_action";
}

pArray mmt_delta_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	std::shared_ptr<mmt_delta_encode_array> sourceArr = std::static_pointer_cast<mmt_delta_encode_array>(inputArrays[0]);
	arrayId arrId = sourceArr->getId();

	auto deltaArr = std::make_shared<mmt_delta_decode_array>(this->getArrayDesc());

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		switch (attr->type_)
		{
		case eleType::CHAR:
			this->attributeDecode<char>(deltaArr, sourceArr, attr);
			break;
		case eleType::INT8:
			this->attributeDecode<int8_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::INT16:
			this->attributeDecode<int16_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::INT32:
			this->attributeDecode<int32_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::INT64:
			this->attributeDecode<int64_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::UINT8:
			this->attributeDecode<uint8_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::UINT16:
			this->attributeDecode<uint16_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::UINT32:
			this->attributeDecode<uint32_t>(deltaArr, sourceArr, attr);
			break;
		case eleType::UINT64:
			this->attributeDecode<uint64_t>(deltaArr, sourceArr, attr);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	qry->getTimer()->pause(0);
	//========================================//

	return std::static_pointer_cast<arrayBase>(deltaArr);
}
}	// msdb
