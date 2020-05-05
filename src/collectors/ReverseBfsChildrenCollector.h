#ifndef QC_REVERSEBFSCHILDRENCOLLECTOR_H
#define QC_REVERSEBFSCHILDRENCOLLECTOR_H

#include "collectors/Collector.h"
#include "common/ExpressionVisitor.h"

class ReverseBfsChildrenCollector final : public Collector, public ExpressionVisitor {
public:
    ReverseBfsChildrenCollector() = default;

    std::deque<std::shared_ptr<Expression>>
    collect(const std::shared_ptr<Expression> &root) override;

    void visit(Determinant &determinant) override;

    void visit(ConditionalTerm &term) override;

    void visit(NonConditionalTerm &term) override;

    void visit(UnaryOperation &op) override;

    void visit(BinaryOperation &op) override;

    void visit(TerminalExpression &expression) override;

private:
    std::deque<std::shared_ptr<Expression>> m_collected;
    std::queue<std::shared_ptr<Expression>> m_queue;
};

#endif //QC_REVERSEBFSCHILDRENCOLLECTOR_H
