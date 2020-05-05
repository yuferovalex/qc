#include <gtest/gtest.h>

#include "collectors/ReverseBfsChildrenCollector.h"

TEST(reverse_bfs_children_collector, for_non_conditional_term) {
    auto rightOperand = std::make_shared<Literal>(1);
    auto leftOperand = std::make_shared<Variable>("x", 0, 0);
    auto operation = std::make_shared<AddOperation>();
    operation->setLeftSubtree(leftOperand);
    operation->setRightSubtree(rightOperand);
    auto term = std::make_shared<NonConditionalTerm>("y", 0, 0, operation);
    auto determinant = std::make_shared<Determinant>();
    determinant->addTerm(term);

    ReverseBfsChildrenCollector collector;
    auto result = collector.collect(determinant);

    ASSERT_EQ(5, result.size());
    auto begin = result.cbegin();
    EXPECT_EQ(rightOperand, *begin++);
    EXPECT_EQ(leftOperand, *begin++);
    EXPECT_EQ(operation, *begin++);
    EXPECT_EQ(term, *begin++);
    EXPECT_EQ(determinant, *begin++);
}

TEST(reverse_bfs_children_collector, for_conditional_terms) {
    auto operationRightOperand = std::make_shared<Literal>(1);
    auto operationLeftOperand = std::make_shared<Variable>("x", 0, 0);
    auto operation = std::make_shared<AddOperation>();
    operation->setLeftSubtree(operationLeftOperand);
    operation->setRightSubtree(operationRightOperand);

    auto conditionRightOperand = std::make_shared<Literal>(1);
    auto conditionLeftOperand = std::make_shared<Variable>("x", 0, 0);
    auto condition = std::make_shared<EqOperation>();
    condition->setLeftSubtree(conditionLeftOperand);
    condition->setRightSubtree(conditionRightOperand);

    auto term = std::make_shared<ConditionalTerm>("y", 0, 0, condition, operation);
    auto determinant = std::make_shared<Determinant>();
    determinant->addTerm(term);

    ReverseBfsChildrenCollector collector;
    auto result = collector.collect(determinant);


    ASSERT_EQ(8, result.size());
    auto begin = result.cbegin();
    EXPECT_EQ(operationRightOperand, *begin++);
    EXPECT_EQ(operationLeftOperand, *begin++);
    EXPECT_EQ(conditionRightOperand, *begin++);
    EXPECT_EQ(conditionLeftOperand, *begin++);
    EXPECT_EQ(operation, *begin++);
    EXPECT_EQ(condition, *begin++);
    EXPECT_EQ(term, *begin++);
    EXPECT_EQ(determinant, *begin++);
}
