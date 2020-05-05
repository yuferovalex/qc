#ifndef QC_EXPRESSIONVISITOR_H
#define QC_EXPRESSIONVISITOR_H

#include "Expressions.h"

class ExpressionVisitor {
public:
    virtual ~ExpressionVisitor() = default;

    virtual void visit(Expression &expression) {}

    virtual void visit(Determinant &determinant) {
        visit(static_cast<Expression &>(determinant));
    }

    virtual void visit(Operation &op) {
        visit(static_cast<Expression &>(op));
    }

    virtual void visit(UnaryOperation &op) {
        visit(static_cast<Operation &>(op));
    }

    virtual void visit(BinaryOperation &op) {
        visit(static_cast<Operation &>(op));
    }

    virtual void visit(TerminalExpression &expression) {
        visit(static_cast<Expression &>(expression));
    }

    virtual void visit(Literal &number) {
        visit(static_cast<TerminalExpression &>(number));
    }

    virtual void visit(Variable &variable) {
        visit(static_cast<TerminalExpression &>(variable));
    }

    virtual void visit(AddOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(MulOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(SubOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(DivOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(EqOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(NeOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(AndOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(OrOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(LtOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(LeOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(GtOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(GeOperation &op) {
        visit(static_cast<BinaryOperation &>(op));
    }

    virtual void visit(NegOperation &op) {
        visit(static_cast<UnaryOperation &>(op));
    }

    virtual void visit(AbsOperation &op) {
        visit(static_cast<UnaryOperation &>(op));
    }

    virtual void visit(SqrtOperation &op) {
        visit(static_cast<UnaryOperation &>(op));
    }

    virtual void visit(Term &term) {
        visit(static_cast<Variable &>(term));
    }

    virtual void visit(ConditionalTerm &term) {
        visit(static_cast<Term &>(term));
    }

    virtual void visit(NonConditionalTerm &term) {
        visit(static_cast<Term &>(term));
    }

    virtual void visit(HaltExpression &halt) {
        visit(static_cast<Expression &>(halt));
    }
};

#endif //QC_EXPRESSIONVISITOR_H
