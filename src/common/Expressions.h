#ifndef QC_EXPRESSIONS_H
#define QC_EXPRESSIONS_H

#include <memory>
#include <optional>
#include <vector>

class ExpressionVisitor;

class Expression : public std::enable_shared_from_this<Expression> {
public:
    virtual ~Expression() = default;

    virtual std::string name() const {
        return typeid(*this).name();
    }

    virtual long long nestingLevel() const = 0;

    virtual long long commandsCount() const = 0;

    virtual void accept(ExpressionVisitor &visitor) = 0;
};

class TerminalExpression : public Expression {
public:
    long long nestingLevel() const override;

    long long int commandsCount() const override;
};

class Operation : public Expression {
};

class UnaryOperation : public Operation {
public:
    const std::shared_ptr<Expression> &subtree() const;

    void setSubtree(const std::shared_ptr<Expression> &expression);

    long long nestingLevel() const override;

    long long commandsCount() const override;

private:
    std::shared_ptr<Expression> m_expression;
    mutable std::optional<long long> m_nestingLevelCache;
    mutable std::optional<long long> m_subexpressionsCount;
};

class BinaryOperation : public Operation {
public:
    long long nestingLevel() const override;

    long long commandsCount() const override;

    const std::shared_ptr<Expression> &leftSubtree() const;

    const std::shared_ptr<Expression> &rightSubtree() const;

    void setLeftSubtree(const std::shared_ptr<Expression> &expression);

    void setRightSubtree(const std::shared_ptr<Expression> &expression);

private:
    std::shared_ptr<Expression> m_leftOperand;
    std::shared_ptr<Expression> m_rightOperand;
    mutable std::optional<long long> m_nestingLevelCache;
    mutable std::optional<long long> m_subexpressionsCount;
};

class Literal : public TerminalExpression {
public:
    explicit Literal(double value);

    double value() const;

    void accept(ExpressionVisitor &visitor) override;

private:
    const double m_value;
};

class Variable : public TerminalExpression {
public:
    explicit Variable(const std::string &name, size_t row, size_t column);

    std::string variableName() const;

    size_t column() const;

    size_t row() const;

    void accept(ExpressionVisitor &visitor) override;

private:
    const std::string m_name;
    const size_t m_row;
    const size_t m_column;
};

class AddOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "+";

    void accept(ExpressionVisitor &visitor) override;
};

class MulOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "*";

    void accept(ExpressionVisitor &visitor) override;
};

class SubOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "-";

    void accept(ExpressionVisitor &visitor) override;
};

class DivOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "/";

    void accept(ExpressionVisitor &visitor) override;
};

class EqOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "==";

    void accept(ExpressionVisitor &visitor) override;
};

class NeOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "!=";

    void accept(ExpressionVisitor &visitor) override;
};

class AndOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "&";

    void accept(ExpressionVisitor &visitor) override;
};

class OrOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "|";

    void accept(ExpressionVisitor &visitor) override;
};

class LtOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "<";

    void accept(ExpressionVisitor &visitor) override;
};

class LeOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = "<=";

    void accept(ExpressionVisitor &visitor) override;
};

class GtOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = ">";

    void accept(ExpressionVisitor &visitor) override;
};

class GeOperation : public BinaryOperation {
public:
    inline static constexpr const char *SYMBOL = ">=";

    void accept(ExpressionVisitor &visitor) override;
};

class NegOperation : public UnaryOperation {
public:
    inline static constexpr const char *SYMBOL = "-";

    void accept(ExpressionVisitor &visitor) override;
};

class AbsOperation : public UnaryOperation {
public:
    inline static constexpr const char *SYMBOL = "abs";

    void accept(ExpressionVisitor &visitor) override;
};

class SqrtOperation : public UnaryOperation {
public:
    inline static constexpr const char *SYMBOL = "sqrt";

    void accept(ExpressionVisitor &visitor) override;
};

class Term : public Variable {
public:
    Term(const std::string &name, size_t row, size_t column, const std::shared_ptr<Expression> &expression);

    const std::shared_ptr<Expression> &expression() const;

    virtual void accept(ExpressionVisitor &visitor) = 0;

protected:
    std::shared_ptr<Expression> m_expression;
};

class HaltExpression final : public Expression {
public:
    explicit HaltExpression(const std::string &reason);

    const std::string &reason() const;

    long long int nestingLevel() const override;

    long long int commandsCount() const override;

    void accept(ExpressionVisitor &visitor) override;

private:
    std::string m_reason;
};

class ConditionalTerm : public Term {
public:
    ConditionalTerm(
            const std::string &name,
            size_t row,
            size_t col,
            const std::shared_ptr<Expression> &condition,
            const std::shared_ptr<Expression> &expression);

    const std::shared_ptr<Expression> &condition() const;

    void accept(ExpressionVisitor &visitor) override;

    long long int nestingLevel() const override;

    long long int commandsCount() const override;

private:
    std::shared_ptr<Expression> m_condition;
    mutable std::optional<long long> m_nestingLevelCache;
    mutable std::optional<long long> m_subexpressionsCount;
};

class NonConditionalTerm : public Term {
public:
    NonConditionalTerm(
            const std::string &name,
            size_t row,
            size_t col,
            const std::shared_ptr<Expression> &expression);

    long long nestingLevel() const override;

    long long commandsCount() const override;

    void accept(ExpressionVisitor &visitor) override;
};

class Determinant : public Expression {
public:
    long long nestingLevel() const final;

    long long int commandsCount() const override;

    void accept(ExpressionVisitor &visitor) override;

    const std::vector<std::shared_ptr<Term>> &terms() const;

    void addTerm(const std::shared_ptr<Term> &term);

private:
    std::vector<std::shared_ptr<Term>> m_terms;
    mutable std::optional<long long> m_nestingLevelCache;
    mutable std::optional<long long> m_subexpressionsCount;
};

#endif //QC_EXPRESSIONS_H
