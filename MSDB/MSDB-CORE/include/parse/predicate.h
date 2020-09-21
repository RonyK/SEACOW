#pragma once
#ifndef _MSDB_PREDICATE_H_
#define _MSDB_PREDICATE_H_

#include <util/coordinate.h>
#include <parse/term.h>
#include <memory>
#include <index/mmtNode.h>

namespace msdb
{
class predicate;
using pPredicate = std::shared_ptr<predicate>;

class predicate : public std::enable_shared_from_this<predicate>
{
public:
	predicate(pTerm lhs, pTerm rhs);
	~predicate();

public:
	virtual void setEvaluateFunc(eleType eType) = 0;
	virtual bool evaluate(pItemItr iit) = 0;
	virtual bool evaluateNode(pMmtNode node) = 0;

protected:
	pTerm lTerm;
	pTerm rTerm;
};

class singlePredicate : public predicate
{
public:
	singlePredicate(pTerm mit);

public:
	virtual void setEvaluateFunc(eleType eType) override;
	virtual bool evaluate(pItemItr iit) override;
	virtual bool evaluateNode(pMmtNode node) override;

	template <typename Ty_>
	bool evaluateImpl(pItemItr iit)
	{
		return this->lTerm->evaluate<Ty_>(iit);
	}

	template <typename Ty_>
	bool evaluateNodeImpl(pMmtNode node)
	{
		return this->lTerm->evaluateNode<Ty_>(node);
	}

private:
	typedef bool(singlePredicate::* eFunc)(pItemItr);
	typedef bool(singlePredicate::* enFunc)(pMmtNode);
	eFunc findEvaluateFunc(eleType type);
	enFunc findEvaluateNodeFunc(eleType type);
	bool (singlePredicate::* evaluateFunc)(pItemItr);
	bool (singlePredicate::* evaluateNodeFunc)(pMmtNode);
};

class andPredicate : public predicate
{
public:
	andPredicate(pTerm lhs, pTerm rhs);

public:
	virtual void setEvaluateFunc(eleType eType) override;
	virtual bool evaluate(pItemItr iit) override;
	virtual bool evaluateNode(pMmtNode node) override;

	template <typename Ty_>
	bool evaluateImpl(pItemItr iit)
	{
		return this->lTerm->evaluate<Ty_>(iit) && this->rTerm->evaluate<Ty_>(iit);
	}

	template <typename Ty_>
	bool evaluateNodeImpl(pMmtNode node)
	{
		return this->lTerm->evaluateNode<Ty_>(node) && this->rTerm->evaluateNode<Ty_>(node);
	}

private:
	typedef bool(andPredicate::* eFunc)(pItemItr);
	typedef bool(andPredicate::* enFunc)(pMmtNode);
	eFunc findEvaluateFunc(eleType type);
	enFunc findEvaluateNodeFunc(eleType type);
	bool (andPredicate::* evaluateFunc)(pItemItr);
	bool (andPredicate::* evaluateNodeFunc)(pMmtNode);
};

class orPredicate : public predicate
{
public:
	orPredicate(pTerm lhs, pTerm rhs);

public:
	virtual void setEvaluateFunc(eleType eType) override;

	virtual bool evaluate(pItemItr iit) override;
	virtual bool evaluateNode(pMmtNode node) override;

	template <typename Ty_>
	bool evaluateImpl(pItemItr iit)
	{
		return this->lTerm->evaluate<Ty_>(iit) || this->rTerm->evaluate<Ty_>(iit);
	}

	template <typename Ty_>
	bool evaluateNodeImpl(pMmtNode node)
	{
		return this->lTerm->evaluateNode<Ty_>(node) && this->rTerm->evaluateNode<Ty_>(node);
	}

private:
	typedef bool(orPredicate::* eFunc)(pItemItr);
	typedef bool(orPredicate::* enFunc)(pMmtNode);
	eFunc findEvaluateFunc(eleType type);
	enFunc findEvaluateNodeFunc(eleType type);
	bool (orPredicate::* evaluateFunc)(pItemItr);
	bool (orPredicate::* evaluateNodeFunc)(pMmtNode);
};
}		// msdb
#endif	// _MSDB_PREDICATE_H_
