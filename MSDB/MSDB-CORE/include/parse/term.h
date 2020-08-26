#pragma once
#ifndef _MSDB_TERM_H_
#define _MSDB_TERM_H_

#include <util/coordinate.h>
#include <parse/expression.h>
#include <memory>

namespace msdb
{
class term;
using pTerm = std::shared_ptr<term>;

enum class termType
{
	EQUAL,
	NOT_EQUAL,
	GREATER,
	GREATER_EQUAL,
	LESS,
	LESS_EQUAL
};


class term : std::enable_shared_from_this<term>
{
public:
	term(pExpression lhs, pExpression rhs, termType tType);

public:
	void setEvaluateFunc(eleType eType);

	template <typename Ty_>
	bool evaluate(pItemItr iit)
	{
		return (this->*evaluateFunc)(iit);
	}

	template <typename Ty_>
	bool evaluateEqual(pItemItr iit)
	{
		return (**iit).get<Ty_>() == boost::any_cast<int64_t>(rhs_->getValue());
	}

	template <typename Ty_>
	bool evaluateNotEqual(pItemItr iit)
	{
		return (**iit).get<Ty_>() == boost::any_cast<int64_t>(rhs_->getValue());
	}

	template <typename Ty_>
	bool evaluateGreater(pItemItr iit)
	{
		return (**iit).get<Ty_>() == boost::any_cast<int64_t>(rhs_->getValue());
	}

	template <typename Ty_>
	bool evaluateGreaterEqual(pItemItr iit)
	{
		return (**iit).get<Ty_>() == boost::any_cast<int64_t>(rhs_->getValue());
	}

	template <typename Ty_>
	bool evaluateLess(pItemItr iit)
	{
		return (**iit).get<Ty_>() == boost::any_cast<int64_t>(rhs_->getValue());
	}

	template <typename Ty_>
	bool evaluateLessEqual(pItemItr iit)
	{
		return (**iit).get<Ty_>() == boost::any_cast<int64_t>(rhs_->getValue());
	}

protected:
	pExpression lhs_;
	pExpression rhs_;
	bool result_;

private:
	termType tType_;
	typedef bool(term::* eFunc)(pItemItr);

	template <typename Ty_>
	eFunc findEvaluateFunc(termType type)
	{
		static bool (term:: * func_ptr[6])(pItemItr) = {
			&term::evaluateEqual<Ty_>,
			&term::evaluateNotEqual<Ty_>,
			& term::evaluateGreater<Ty_>,
			& term::evaluateGreaterEqual<Ty_>,
			& term::evaluateLess<Ty_>,
			& term::evaluateLessEqual<Ty_>
		};

		return func_ptr[static_cast<int>(type)];
	}
	bool (term::* evaluateFunc)(pItemItr);
};

//class termEqual : public term
//{
//public:
//	termEqual(pExpression lhs, pExpression rhs);
//
//public:
//	template <typename Ty_>
//	bool evaluate(pItemItr iit)
//	{
//		
//	}
//};
//
//class termGreater : public term
//{
//public:
//	termGreater(pExpression lhs, pExpression rhs);
//
//public:
//	virtual bool evaluate(pItemItr iit);
//};
//
//class termLess : public term
//{
//public:
//	termLess(pExpression lhs, pExpression rhs);
//
//public:
//	virtual bool evaluate(pItemItr iit);
//};
}		// msdb
#endif	// _MSDB_TERM_H_
