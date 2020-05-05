#include "ReverseBfsChildrenCollector.h"

std::deque<std::shared_ptr<Expression>>
ReverseBfsChildrenCollector::collect(const std::shared_ptr<Expression> &root) {
    m_queue.push(root);
    while (!m_queue.empty()) {
        auto front = m_queue.front();
        m_queue.pop();
        front->accept(*this);
    }
    return std::move(m_collected);
}

void ReverseBfsChildrenCollector::visit(Determinant &determinant) {
    for (auto term : determinant.terms()) {
        m_queue.push(term);
    }
    m_collected.push_front(determinant.shared_from_this());
}

void ReverseBfsChildrenCollector::visit(ConditionalTerm &term) {
    m_queue.push(term.condition());
    m_queue.push(term.expression());
    m_collected.push_front(term.shared_from_this());
}

void ReverseBfsChildrenCollector::visit(NonConditionalTerm &term) {
    m_queue.push(term.expression());
    m_collected.push_front(term.shared_from_this());
}

void ReverseBfsChildrenCollector::visit(UnaryOperation &op) {
    m_queue.push(op.subtree());
    m_collected.push_front(op.shared_from_this());
}

void ReverseBfsChildrenCollector::visit(BinaryOperation &op) {
    m_queue.push(op.leftSubtree());
    m_queue.push(op.rightSubtree());
    m_collected.push_front(op.shared_from_this());
}

void ReverseBfsChildrenCollector::visit(TerminalExpression &expression) {
    m_collected.push_front(expression.shared_from_this());
}
