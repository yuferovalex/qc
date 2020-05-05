#include "ParserUtils.h"

#include <regex>

#include "common/Expressions.h"
#include "det_parser/DeterminantParser.h"

static std::regex ID_REGEX(R"(([[:alpha:]][[:alnum:]]*)\s*(?:\(\s*([[:digit:]]*)\s*(?:,\s*([[:digit:]]*))?\s*\))?)");

bool Details::isIdentifier(const std::string &term) {
    std::smatch results;
    return std::regex_match(term, ID_REGEX);
}

Details::TermParams Details::parseIdentifier(const std::string &term) {
    std::smatch results;
    std::regex_match(term, results, ID_REGEX);

    TermParams params;
    auto name = results.str(1);
    if (name.empty()) {
        throw std::runtime_error("wrong term name");
    }
    params.name = name;

    auto row = results.str(2);
    if (!row.empty()) {
        params.row = std::stoi(row);
    }

    auto col = results.str(3);
    if (!col.empty()) {
        params.col = std::stoi(col);
    }

    return params;
}

void Details::registerNodeParsers() {
    static bool isInit = false;
    if (isInit) {
        return;
    }
    isInit = true;

    DeterminantParser::registerNodeParserFabric(UnaryOperationNodeParser<AbsOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<AddOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<AndOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<DivOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<EqOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<GeOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<GtOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<LeOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<LtOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<MulOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<NeOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(BinaryOperationNodeParser<OrOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(UnaryOperationNodeParser<SqrtOperation>::createFabric());
    DeterminantParser::registerNodeParserFabric(createUnaryMinusAndSubOperationNodeParserFabric());
    DeterminantParser::registerNodeParserFabric(createLiteralAndVariableNodeParserFabric());
    DeterminantParser::registerNodeParserFabric(HaltNodeParser::createFabric());
}

std::optional<std::string> Details::StringCollector::collect(const std::shared_ptr<Expression> &expr) {
    expr->accept(*this);
    return std::move(string);
}

void Details::StringCollector::visit(HaltExpression &halt) {
    string = halt.reason();
}
