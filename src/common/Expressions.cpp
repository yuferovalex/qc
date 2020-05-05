#include "Expressions.h"
#include "ExpressionVisitor.h"

Literal::Literal(double value) : m_value(value) {}

double Literal::value() const {
    return m_value;
}

void Literal::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

Variable::Variable(const std::string &name, size_t row, size_t column)
        : m_name(name), m_row(row), m_column(column) {}

void Variable::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

std::string Variable::variableName() const {
    return m_name;
}

size_t Variable::column() const {
    return m_column;
}

size_t Variable::row() const {
    return m_row;
}

const std::shared_ptr<Expression> &UnaryOperation::subtree() const {
    return m_expression;
}

long long UnaryOperation::nestingLevel() const {
    if (!m_nestingLevelCache) {
        m_nestingLevelCache.emplace(m_expression->nestingLevel());
    }
    return m_nestingLevelCache.value();
}

void UnaryOperation::setSubtree(const std::shared_ptr<Expression> &expression) {
    m_expression = expression;
}

long long UnaryOperation::commandsCount() const {
    if (!m_subexpressionsCount) {
        m_subexpressionsCount = m_expression->commandsCount() + 1;
    }
    return *m_subexpressionsCount;
}

const std::shared_ptr<Expression> &BinaryOperation::leftSubtree() const {
    return m_leftOperand;
}

const std::shared_ptr<Expression> &BinaryOperation::rightSubtree() const {
    return m_rightOperand;
}

long long BinaryOperation::nestingLevel() const {
    if (!m_nestingLevelCache) {
        auto value = std::max(leftSubtree()->nestingLevel(), rightSubtree()->nestingLevel()) + 1;
        m_nestingLevelCache.emplace(value);
    }
    return m_nestingLevelCache.value();
}

void BinaryOperation::setLeftSubtree(const std::shared_ptr<Expression> &expression) {
    m_leftOperand = expression;
}

void BinaryOperation::setRightSubtree(const std::shared_ptr<Expression> &expression) {
    m_rightOperand = std::move(expression);
}

long long BinaryOperation::commandsCount() const {
    if (!m_subexpressionsCount) {
        m_subexpressionsCount = m_leftOperand->commandsCount()
                                + m_rightOperand->commandsCount() + 1;
    }
    return *m_subexpressionsCount;
}

long long Determinant::nestingLevel() const {
    if (!m_nestingLevelCache) {
        auto level = 0LL;
        for (auto term : m_terms) {
            level = std::min(term->nestingLevel(), level);
        }
        m_nestingLevelCache = level;
    }
    return *m_nestingLevelCache;
}

void Determinant::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

const std::vector<std::shared_ptr<Term>> &Determinant::terms() const {
    return m_terms;
}

void Determinant::addTerm(const std::shared_ptr<Term> &term) {
    m_terms.push_back(term);
}

long long int Determinant::commandsCount() const {
    if (!m_subexpressionsCount) {
        long long value = 0;
        for (auto term : m_terms) {
            value += term->commandsCount();
        }
        m_subexpressionsCount = value;
    }
    return *m_subexpressionsCount;
}

Term::Term(
        const std::string &name,
        size_t row,
        size_t column,
        const std::shared_ptr<Expression> &expression)
        : Variable(name, row, column), m_expression(expression) {}

const std::shared_ptr<Expression> &Term::expression() const {
    return m_expression;
}

NonConditionalTerm::NonConditionalTerm(
        const std::string &name,
        size_t row,
        size_t col,
        const std::shared_ptr<Expression> &expression
)
        : Term(name, row, col, expression) {}

void NonConditionalTerm::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

long long NonConditionalTerm::nestingLevel() const {
    return m_expression->nestingLevel();
}

long long NonConditionalTerm::commandsCount() const {
    return m_expression->commandsCount();
}

ConditionalTerm::ConditionalTerm(
        const std::string &name,
        size_t row,
        size_t col,
        const std::shared_ptr<Expression> &condition,
        const std::shared_ptr<Expression> &expression
)
        : Term(name, row, col, expression), m_condition(condition) {}

const std::shared_ptr<Expression> &ConditionalTerm::condition() const {
    return m_condition;
}

void ConditionalTerm::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

long long ConditionalTerm::nestingLevel() const {
    if (!m_nestingLevelCache) {
        m_nestingLevelCache = std::max(m_condition->nestingLevel(), m_expression->nestingLevel());
    }
    return *m_nestingLevelCache;
}

long long ConditionalTerm::commandsCount() const {
    if (!m_subexpressionsCount) {
        m_subexpressionsCount = m_condition->commandsCount() + m_expression->commandsCount() + 1;
    }
    return *m_subexpressionsCount;
}

void AddOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void MulOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void SubOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void DivOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

long long TerminalExpression::nestingLevel() const {
    return 0;
}

long long TerminalExpression::commandsCount() const {
    return 0;
}

void EqOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void NeOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void AndOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void OrOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void LtOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void LeOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void GtOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void GeOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void NegOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void AbsOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

void SqrtOperation::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}

HaltExpression::HaltExpression(const std::string &reason)
    : m_reason(reason)
{}

const std::string &HaltExpression::reason() const {
    return m_reason;
}

long long int HaltExpression::nestingLevel() const {
    return 0;
}

long long int HaltExpression::commandsCount() const {
    return 0;
}

void HaltExpression::accept(ExpressionVisitor &visitor) {
    visitor.visit(*this);
}
