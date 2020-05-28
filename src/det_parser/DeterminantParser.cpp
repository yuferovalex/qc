#include "DeterminantParser.h"

#include <fstream>
#include <sstream>
#include <regex>

#include <boost/algorithm/string/trim.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "ParserUtils.h"

static std::vector<NodeParser::Fabric> s_factories = {};

std::shared_ptr<Determinant> DeterminantParser::parse(const boost::filesystem::path &path) {
    std::ifstream stream(path.string());
    return parseDeterminant(stream);
}

void DeterminantParser::registerNodeParserFabric(NodeParser::Fabric factory) {
    s_factories.emplace_back(std::move(factory));
}

std::shared_ptr<Determinant> DeterminantParser::parseDeterminant(std::istream &istream) {
    std::shared_ptr<Determinant> determinant = std::make_shared<Determinant>();
    while (istream.peek() != EOF) {
        determinant->addTerm(parseTerm(istream));
    }
    return determinant;
}

std::shared_ptr<Term> DeterminantParser::parseTerm(std::istream &istream) {
    using std::getline;

    std::string termName;
    getline(istream, termName, '=');
    boost::trim(termName);
    auto id = ::Details::parseIdentifier(termName);

    std::string conditionString;
    getline(istream, conditionString, ';');
    boost::trim(conditionString);

    std::string expressionString;
    getline(istream, expressionString);
    boost::trim(expressionString);

    std::stringstream expressionStream(expressionString);
    auto expression = parseExpression(expressionStream);
    if (conditionString.empty()) {
        return std::make_unique<NonConditionalTerm>(id.name, id.row, id.col, std::move(expression));
    }

    std::stringstream conditionStream(conditionString);
    auto condition = parseExpression(conditionStream);
    return std::make_unique<ConditionalTerm>(id.name, id.row, id.col, std::move(condition), std::move(expression));
}

std::shared_ptr<NodeParser> findNodeParser(const boost::property_tree::ptree &root) {
    for (auto factory : s_factories) {
        if (auto parser = factory(root)) {
            return parser;
        }
    }
    throw std::runtime_error("unknown token given");
}

std::shared_ptr<Expression> tryParseTerminal(std::istream &istream) {
    std::string string;
    std::getline(istream, string);
    if (Details::isIdentifier(string)) {
        auto id = Details::parseIdentifier(string);
        return std::make_shared<Variable>(id.name, id.row, id.col);
    }
    try {
        return std::make_shared<Literal>(std::stod(string));
    } catch (std::exception &) {
        return nullptr;
    }
}

std::shared_ptr<Expression> DeterminantParser::parseExpression(std::istream &istream) {
    boost::property_tree::ptree tree;
    try {
        boost::property_tree::read_json(istream, tree);
    } catch (std::exception &e) {
        if (auto terminal = tryParseTerminal(istream)) {
            return terminal;
        }
        throw e;
    }

    std::stack<std::shared_ptr<NodeParser>> stack;
    stack.push(findNodeParser(tree));

    while (!stack.empty()) {
        auto stackTop = stack.top();
        if (auto subtree = stackTop->nextSubtree()) {
            auto nodeParser = findNodeParser(*subtree);
            stack.push(nodeParser);
        } else {
            stack.pop();
            if (stack.empty()) {
                auto expression = stackTop->expression();
                prepareExpression(expression);
                return expression;
            }
            stack.top()->setParsedSubtree(stackTop->expression());
        }
    }
}

void DeterminantParser::prepareExpression(const std::shared_ptr<Expression> &expression) {
    ::Details::StringCollector stringCollector;

    // Прогрев кешей
    expression->nestingLevel();
    expression->commandsCount();

    // Сбор строк
    if (auto string = stringCollector.collect(expression)) {
        strings.push(*string);
    }
}

DeterminantParser::DeterminantParser() {
    ::Details::registerNodeParsers();
}
