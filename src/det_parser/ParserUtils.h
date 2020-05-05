#ifndef QC_PARSERUTILS_H
#define QC_PARSERUTILS_H

#include <string>
#include "common/ExpressionVisitor.h"

namespace Details {
    struct TermParams {
        std::string name;
        size_t row = 1;
        size_t col = 1;
    };

    bool isIdentifier(const std::string &term);

    TermParams parseIdentifier(const std::string &term);

    void registerNodeParsers();

    struct StringCollector : ExpressionVisitor {
        std::optional<std::string> string;

        std::optional<std::string> collect(const std::shared_ptr<Expression> &expr);

        void visit(HaltExpression &halt) override;
    };
}

#endif //QC_PARSERUTILS_H
