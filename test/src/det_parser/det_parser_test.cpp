#include <gtest/gtest.h>

#include <fstream>
#include <functional>

#include "det_parser/DeterminantParser.h"

using ::testing::Test;
using ::testing::WithParamInterface;
using ::testing::Values;

typedef std::function<bool (Expression &expression)> IsInstanceOf_t;
typedef std::tuple<std::string, IsInstanceOf_t> Params;

template <typename T>
bool IsInstanceOf(Expression &expression) {
    return dynamic_cast<T *>(&expression);
}

class DeterminantParserOperation : public Test, public WithParamInterface<Params> {};

INSTANTIATE_TEST_CASE_P(
        DeterminantParserOperation,
        DeterminantParserOperation,
        Values(
                Params("abs_operator.json", IsInstanceOf<AbsOperation>),
                Params("add_operator.json", IsInstanceOf<AddOperation>),
                Params("and_operator.json", IsInstanceOf<AndOperation>),
                Params("div_operator.json", IsInstanceOf<DivOperation>),
                Params("eq_operator.json", IsInstanceOf<EqOperation>),
                Params("ge_operator.json", IsInstanceOf<GeOperation>),
                Params("gt_operator.json", IsInstanceOf<GtOperation>),
                Params("le_operator.json", IsInstanceOf<LeOperation>),
                Params("lt_operator.json", IsInstanceOf<LtOperation>),
                Params("mul_operator.json", IsInstanceOf<MulOperation>),
                Params("ne_operator.json", IsInstanceOf<NeOperation>),
                Params("neg_operator.json", IsInstanceOf<NegOperation>),
                Params("or_operator.json", IsInstanceOf<OrOperation>),
                Params("sqrt_operator.json", IsInstanceOf<SqrtOperation>),
                Params("sub_operator.json", IsInstanceOf<SubOperation>)
        ));

TEST_P(DeterminantParserOperation, test) {
    auto [fileName, instanceOf] = GetParam();
    std::ifstream stream("resources/det_parser/expressions/operators/" + fileName);

    std::shared_ptr<Expression> result;
    ASSERT_NO_THROW(result = DeterminantParser().parseExpression(stream));
    ASSERT_TRUE(instanceOf(*result));
}

TEST(DeterminantParser, parse_variable_and_literals) {
    std::ifstream stream("resources/det_parser/expressions/variable_and_literals.json");

    std::shared_ptr<Expression> result;
    ASSERT_NO_THROW(result = DeterminantParser().parseExpression(stream));
    ASSERT_NE(nullptr, result);
    auto operation = std::dynamic_pointer_cast<AddOperation>(result);
    ASSERT_NE(nullptr, operation);

    auto variable = std::dynamic_pointer_cast<Variable>(operation->leftSubtree());
    ASSERT_NE(nullptr, variable);
    EXPECT_EQ("C1", variable->variableName());
    EXPECT_EQ(123, variable->row());
    EXPECT_EQ(321, variable->column());

    auto literal = std::dynamic_pointer_cast<Literal>(operation->rightSubtree());
    ASSERT_NE(nullptr, literal);
    EXPECT_DOUBLE_EQ(123.12, literal->value());
}

TEST(DeterminantParser, parse_nested_expression) {
    std::ifstream stream("resources/det_parser/expressions/nested_expression.json");

    std::shared_ptr<Expression> result;
    ASSERT_NO_THROW(result = DeterminantParser().parseExpression(stream));
    ASSERT_NE(nullptr, result);
    auto operation = std::dynamic_pointer_cast<AddOperation>(result);
    ASSERT_NE(nullptr, operation);

    auto left = std::dynamic_pointer_cast<AbsOperation>(operation->leftSubtree());
    ASSERT_NE(nullptr, left);
    auto right = std::dynamic_pointer_cast<AbsOperation>(operation->rightSubtree());
    ASSERT_NE(nullptr, right);

    std::shared_ptr<AbsOperation> args[] = { left, right };
    for (auto arg : args) {
        auto subtree = std::dynamic_pointer_cast<Literal>(arg->subtree());
        ASSERT_NE(nullptr, subtree);
        EXPECT_DOUBLE_EQ(-321, subtree->value());
    }
}

TEST(DeterminantParser, parse_single_conditional_term) {
    std::ifstream stream("resources/det_parser/single_conditional_term.txt");

    std::shared_ptr<Term> term;
    ASSERT_NO_THROW(term = DeterminantParser().parseTerm(stream));

    auto conditional = dynamic_cast<ConditionalTerm *>(term.get());
    ASSERT_NE(nullptr, conditional);
    EXPECT_EQ("x1", term->variableName());
    EXPECT_EQ(1, term->row());
    EXPECT_EQ(1, term->column());
    EXPECT_NE(nullptr, term->expression());
    EXPECT_NE(nullptr, conditional->condition());
}

TEST(DeterminantParser, parse_single_nonconditional_term) {
    std::ifstream stream("resources/det_parser/single_nonconditional_term.txt");

    std::shared_ptr<Term> term;
    ASSERT_NO_THROW(term = DeterminantParser().parseTerm(stream));

    auto nonConditional = dynamic_cast<NonConditionalTerm *>(term.get());
    ASSERT_NE(nullptr, nonConditional);
    EXPECT_EQ("C", term->variableName());
    EXPECT_EQ(1, term->row());
    EXPECT_EQ(2, term->column());
    EXPECT_NE(nullptr, term->expression());
}

TEST(DeterminantParser, parse_determinant) {
    std::ifstream stream("resources/det_parser/determinant.txt");

    std::shared_ptr<Determinant> result;
    ASSERT_NO_THROW(result = DeterminantParser().parseDeterminant(stream));
    EXPECT_EQ(2, result->terms().size());
}
