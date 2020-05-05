#include "NodeParser.h"
#include "ParserUtils.h"

VariableNodeParser::VariableNodeParser(const std::string &value) {
    auto id = Details::parseIdentifier(value);
    m_variable = std::make_shared<Variable>(id.name, id.row, id.col);
}

boost::optional<const boost::property_tree::ptree &> VariableNodeParser::nextSubtree() const {
    return boost::optional<const boost::property_tree::ptree &>();
}

std::shared_ptr<Expression> VariableNodeParser::expression() const {
    return m_variable;
}

void NodeParser::setParsedSubtree(const std::shared_ptr<Expression> &) {
    throw std::logic_error("to many subtrees passed");
}

NodeParser::Fabric createUnaryMinusAndSubOperationNodeParserFabric() {
    return [](const boost::property_tree::ptree &root) -> std::shared_ptr<NodeParser> {
        auto actualSymbol = root.get_optional<std::string>("op");
        if (actualSymbol && boost::iequals(*actualSymbol, SubOperation::SYMBOL)) {
            if (root.count("od")) {
                return std::make_shared<UnaryOperationNodeParser<NegOperation>>(root);
            }
            return std::make_shared<BinaryOperationNodeParser<SubOperation>>(root);
        }
        return nullptr;
    };
}

NodeParser::Fabric createLiteralAndVariableNodeParserFabric() {
    return [](const boost::property_tree::ptree &root) -> std::shared_ptr<NodeParser> {
        if (auto value = root.get_value_optional<double>()) {
            return std::make_shared<LiteralNodeParser>(*value);
        }
        if (auto value = root.get_value_optional<std::string>()) {
            if (Details::isIdentifier(*value)) {
                return std::make_shared<VariableNodeParser>(*value);
            }
        }
        return nullptr;
    };
}

LiteralNodeParser::LiteralNodeParser(double value)
        : m_literal(std::make_shared<Literal>(value)) {}

boost::optional<const boost::property_tree::ptree &> LiteralNodeParser::nextSubtree() const {
    return boost::optional<const boost::property_tree::ptree &>();
}

std::shared_ptr<Expression> LiteralNodeParser::expression() const {
    return m_literal;
}

NodeParser::Fabric HaltNodeParser::createFabric() {
    return [](const boost::property_tree::ptree &root) -> std::shared_ptr<NodeParser> {
        if (auto reason = root.get_optional<std::string>("v")) {
            return std::make_shared<HaltNodeParser>(*reason);
        }
        return nullptr;
    };
}

HaltNodeParser::HaltNodeParser(const std::string &reason)
    : m_halt(std::make_shared<HaltExpression>(reason))
{}

std::shared_ptr<Expression> HaltNodeParser::expression() const {
    return m_halt;
}

boost::optional<const boost::property_tree::ptree &> HaltNodeParser::nextSubtree() const {
    return boost::optional<const boost::property_tree::ptree &>();
}
