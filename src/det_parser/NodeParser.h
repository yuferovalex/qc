#ifndef QC_NODEPARSER_H
#define QC_NODEPARSER_H

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>

#include "common/Expressions.h"

class NodeParser {
public:
    typedef std::function<std::shared_ptr<NodeParser>(const boost::property_tree::ptree &)> Fabric;

    virtual boost::optional<const boost::property_tree::ptree &> nextSubtree() const = 0;

    virtual std::shared_ptr<Expression> expression() const = 0;

    virtual void setParsedSubtree(const std::shared_ptr<Expression> &expr);
};

template<typename Operation>
class UnaryOperationNodeParser final : public NodeParser {
public:
    static_assert(std::is_base_of<UnaryOperation, Operation>::value, "Operation must be derived from UnaryOperation");

    static Fabric createFabric() {
        return [](const boost::property_tree::ptree &root) -> std::shared_ptr<NodeParser> {
            if (auto actualSymbol = root.get_optional<std::string>("op")) {
                if (boost::iequals(*actualSymbol, Operation::SYMBOL)) {
                    return std::make_shared<UnaryOperationNodeParser<Operation>>(root);
                }
            }
            return nullptr;
        };
    }

    explicit UnaryOperationNodeParser(const boost::property_tree::ptree &root)
            : m_operation(std::make_shared<Operation>()), m_root(root) {}

    boost::optional<const boost::property_tree::ptree &> nextSubtree() const final {
        if (!m_operation->subtree()) {
            return m_root.get_child_optional("od");
        }
        return boost::optional<const boost::property_tree::ptree &>();
    }

    void setParsedSubtree(const std::shared_ptr<Expression> &expr) final {
        if (!m_operation->subtree()) {
            m_operation->setSubtree(expr);
            return;
        }
        NodeParser::setParsedSubtree(expr);
    }

    std::shared_ptr<Expression> expression() const final {
        m_operation->commandsCount();
        m_operation->nestingLevel();
        return m_operation;
    }

private:
    std::shared_ptr<UnaryOperation> m_operation;
    const boost::property_tree::ptree &m_root;
};

template<typename Operation>
class BinaryOperationNodeParser final : public NodeParser {
public:
    static_assert(std::is_base_of<BinaryOperation, Operation>::value, "Operation must be derived from BinaryOperation");

    static Fabric createFabric() {
        return [](const boost::property_tree::ptree &root) -> std::shared_ptr<NodeParser> {
            if (auto actualSymbol = root.get_optional<std::string>("op")) {
                if (boost::iequals(*actualSymbol, Operation::SYMBOL)) {
                    return std::make_shared<BinaryOperationNodeParser<Operation>>(root);
                }
            }
            return nullptr;
        };
    }

    explicit BinaryOperationNodeParser(const boost::property_tree::ptree &root)
            : m_operation(std::make_unique<Operation>()), m_root(root) {}

    boost::optional<const boost::property_tree::ptree &> nextSubtree() const final {
        if (!m_operation->leftSubtree()) {
            return m_root.get_child_optional("fO");
        }
        if (!m_operation->rightSubtree()) {
            return m_root.get_child_optional("sO");
        }
        return boost::optional<const boost::property_tree::ptree &>();
    }

    void setParsedSubtree(const std::shared_ptr<Expression> &expr) final {
        if (!m_operation->leftSubtree()) {
            m_operation->setLeftSubtree(expr);
            return;
        }
        if (!m_operation->rightSubtree()) {
            m_operation->setRightSubtree(expr);
            return;
        }
        NodeParser::setParsedSubtree(expr);
    }

    std::shared_ptr<Expression> expression() const final {
        m_operation->commandsCount();
        m_operation->nestingLevel();
        return m_operation;
    }

private:
    std::shared_ptr<BinaryOperation> m_operation;
    const boost::property_tree::ptree &m_root;
};

NodeParser::Fabric createUnaryMinusAndSubOperationNodeParserFabric();
NodeParser::Fabric createLiteralAndVariableNodeParserFabric();

class LiteralNodeParser final : public NodeParser {
public:
    explicit LiteralNodeParser(double value);

    boost::optional<const boost::property_tree::ptree &> nextSubtree() const override;

    std::shared_ptr<Expression> expression() const override;

private:
    std::shared_ptr<Literal> m_literal;
};

class VariableNodeParser final : public NodeParser {
public:
    explicit VariableNodeParser(const std::string &value);

    boost::optional<const boost::property_tree::ptree &> nextSubtree() const final;

    std::shared_ptr<Expression> expression() const final;

private:
    std::shared_ptr<Expression> m_variable;
};

class HaltNodeParser final : public NodeParser {
public:
    static Fabric createFabric();

    explicit HaltNodeParser(const std::string &reason);

    boost::optional<const boost::property_tree::ptree &> nextSubtree() const final;

    std::shared_ptr<Expression> expression() const final;

private:
    std::shared_ptr<Expression> m_halt;
};

#endif //QC_NODEPARSER_H
