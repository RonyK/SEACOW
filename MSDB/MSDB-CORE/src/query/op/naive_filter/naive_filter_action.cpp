#include <op/naive_filter/naive_filter_action.h>
#include <array/memBlockArray.h>

namespace msdb
{
naive_filter_action::naive_filter_action()
{
}

naive_filter_action::~naive_filter_action()
{
}

const char* naive_filter_action::name()
{
	return "naive_filter";
}

pArray naive_filter_action::execute(std::vector<pArray>& inputArrays, pQuery q)
{
	pArray inputArray = inputArrays[0];
	
	for (auto attrDesc : *inputArray->getDesc()->getAttrDescs())
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			this->filter<char>(inputArray);
			break;
		case eleType::INT8:
			this->filter<int8_t>(inputArray);
			break;
		case eleType::INT16:
			this->filter<int16_t>(inputArray);
			break;
		case eleType::INT32:
			this->filter<int32_t>(inputArray);
			break;
		case eleType::INT64:
			this->filter<int64_t>(inputArray);
			break;
		case eleType::UINT8:
			this->filter<uint8_t>(inputArray);
			break;
		case eleType::UINT16:
			this->filter<uint16_t>(inputArray);
			break;
		case eleType::UINT32:
			this->filter<uint32_t>(inputArray);
			break;
		case eleType::UINT64:
			this->filter<uint64_t>(inputArray);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	pArrayDesc arrDesc = inputArray->getDesc();
	pArray outputArray = std::make_shared<memBlockArray>(arrDesc);

	return outputArray;
}
}