#pragma once
#ifndef _MSDB_TERM_H_
#define _MSDB_TERM_H_

#include <util/coordinate.h>
#include <parse/expression.h>
#include <index/mmtNode.h>
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
		// TODO:: change fixed values position
		// Current: attributeVaue(left), int64(right)
		return (this->*evaluateFunc)((**iit).get<Ty_>(), boost::any_cast<int64_t>(rhs_->getValue()));
	}

	template <typename Ty_>
	bool evaluateNode(pMmtNode node)
	{
		return (this->*evaluateNodeFunc)(node, boost::any_cast<int64_t>(rhs_->getValue()));
	}

	template <typename Ty_>
	bool evaluateEqual(const int64_t v1, const int64_t v2)
	{
		return v1 == v2;
	}

	template <typename Ty_>
	bool evaluateNotEqual(const int64_t v1, const int64_t v2)
	{
		return v1 != v2;
	}

	template <typename Ty_>
	bool evaluateGreater(const int64_t v1, const int64_t v2)
	{
		return v1 < v2;
	}

	template <typename Ty_>
	bool evaluateGreaterEqual(const int64_t v1, const int64_t v2)
	{
		return v1 <= v2;
	}

	template <typename Ty_>
	bool evaluateLess(const int64_t v1, const int64_t v2)
	{
		return v1 > v2;
	}

	template <typename Ty_>
	bool evaluateLessEqual(const int64_t v1, const int64_t v2)
	{
		return v1 >= v2;
	}

	template <typename Ty_>
	bool evaluateNodeEqual(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMin<Ty_>() <= v2 && v2 <= v1->getMax<Ty_>();
	}

	template <typename Ty_>
	bool evaluateNodeNotEqual(const pMmtNode v1, const int64_t v2)
	{
		return v2 < v1->getMin<Ty_>() || v1->getMax<Ty_>() < v2;
	}

	template <typename Ty_>
	bool evaluateNodeGreater(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMin<Ty_>() < v2 ;
	}

	template <typename Ty_>
	bool evaluateNodeGreaterEqual(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMin<Ty_>() <= v2;
	}

	template <typename Ty_>
	bool evaluateNodeLess(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMax<Ty_>() > v2;
	}

	template <typename Ty_>
	bool evaluateNodeLessEqual(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMax<Ty_>() >= v2;
	}

protected:
	pExpression lhs_;
	pExpression rhs_;
	bool result_;

private:
	termType tType_;

	typedef bool(term::* eFunc)(const int64_t, const int64_t);
	bool (term::* evaluateFunc)(const int64_t, const int64_t);
	template <typename Ty_>
	eFunc findEvaluateFunc(termType type)
	{
		static bool (term:: * func_ptr[6])(const int64_t, const int64_t) = {
			&term::evaluateEqual<Ty_>,
			&term::evaluateNotEqual<Ty_>,
			& term::evaluateGreater<Ty_>,
			& term::evaluateGreaterEqual<Ty_>,
			& term::evaluateLess<Ty_>,
			& term::evaluateLessEqual<Ty_>
		};

		return func_ptr[static_cast<int>(type)];
	}

	typedef bool(term::* enFunc)(const pMmtNode, const int64_t);
	bool (term::* evaluateNodeFunc)(const pMmtNode, const int64_t);
	template <typename Ty_>
	enFunc findEvaluateNodeFunc(termType type)
	{
		static bool (term:: * func_ptr[6])(const pMmtNode, const int64_t) = {
			&term::evaluateNodeEqual<Ty_>,
			&term::evaluateNodeNotEqual<Ty_>,
			&term::evaluateNodeGreater<Ty_>,
			&term::evaluateNodeGreaterEqual<Ty_>,
			&term::evaluateNodeLess<Ty_>,
			&term::evaluateNodeLessEqual<Ty_>
		};

		return func_ptr[static_cast<int>(type)];
	}
	
};
}		// msdb
#endif	// _MSDB_TERM_H_
