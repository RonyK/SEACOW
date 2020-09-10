#include <parse/term.h>

namespace msdb
{
term::term(pExpression lhs, pExpression rhs, termType tType)
	: lhs_(lhs), rhs_(rhs), tType_(tType), 
	evaluateFunc(nullptr), evaluateNodeFunc(nullptr)
{
}
void term::setEvaluateFunc(eleType eType)
{
	switch(eType)
	{
	case eleType::BOOL:
		this->evaluateFunc = this->findEvaluateFunc<bool>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<bool>(this->tType_);
		break;
	case eleType::CHAR:
		this->evaluateFunc = this->findEvaluateFunc<char>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<char>(this->tType_);
		break;
	case eleType::INT8:
		this->evaluateFunc = this->findEvaluateFunc<int8_t>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<int8_t>(this->tType_);
		break;
	case eleType::INT16:
		this->evaluateFunc = this->findEvaluateFunc<int16_t>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<int16_t>(this->tType_);
		break;
	case eleType::INT32:
		this->evaluateFunc = this->findEvaluateFunc<int32_t>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<int32_t>(this->tType_);
		break;
	case eleType::INT64:
		this->evaluateFunc = this->findEvaluateFunc<int64_t>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<int64_t>(this->tType_);
		break;
	case eleType::UINT8:
		this->evaluateFunc = this->findEvaluateFunc<uint8_t>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<uint8_t>(this->tType_);
		break;
	case eleType::UINT16:
		this->evaluateFunc = this->findEvaluateFunc<uint16_t>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<uint16_t>(this->tType_);
		break;
	case eleType::UINT32:
		this->evaluateFunc = this->findEvaluateFunc<uint32_t>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<uint32_t>(this->tType_);
		break;
	case eleType::UINT64:
		this->evaluateFunc = this->findEvaluateFunc<uint64_t>(this->tType_);
		this->evaluateNodeFunc = this->findEvaluateNodeFunc<uint64_t>(this->tType_);
		break;
	//case eleType::FLOAT:
	//	this->evaluateFunc = this->findEvaluateFunc<bool>(this->tType_);
	//case eleType::DOUBLE:
	//	this->evaluateFunc = this->findEvaluateFunc<bool>(this->tType_);
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
	
}
//termEqual::termEqual(pExpression lhs, pExpression rhs)
//	: term(lhs, rhs)
//{
//}
//bool termEqual::evaluate(pItemItr iit)
//{
//	// TODO::Evaluate
//	return false;
//}
//termGreater::termGreater(pExpression lhs, pExpression rhs)
//	: term(lhs, rhs)
//{
//}
//bool termGreater::evaluate(pItemItr iit)
//{
//	// TODO::Evaluate
//	return false;
//}
}	// msdb