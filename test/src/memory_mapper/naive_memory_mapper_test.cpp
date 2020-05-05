#include <gtest/gtest.h>

#include <deque>

#include "memory_mapper/NaiveMemoryMapper.h"

TEST(naive_memory_mapper, for_non_conditional_term) {
    auto rightOperand = std::make_shared<Literal>(1);
    auto leftOperand = std::make_shared<Variable>("x", 1, 1);
    auto operation = std::make_shared<AddOperation>();
    operation->setLeftSubtree(leftOperand);
    operation->setRightSubtree(rightOperand);
    auto term = std::make_shared<NonConditionalTerm>("y", 1, 1, operation);
    auto determinant = std::make_shared<Determinant>();
    determinant->addTerm(term);

    std::deque<std::shared_ptr<Expression>> queue;
    queue.push_back(rightOperand);
    queue.push_back(leftOperand);
    queue.push_back(operation);
    queue.push_back(term);
    queue.push_back(determinant);

    ProgramMetadata metadata;
    metadata.inputParams.emplace_back(ParameterMetadata {
        "x", "the x", 0, 1, 1
    });
    metadata.outputParams.emplace_back(ParameterMetadata {
            "y", "the y", 0, 1, 1
    });

    NaiveMemoryMapper mapper;
    EXPECT_NO_THROW(mapper.buildMap(metadata, queue));

    // sizeof(x) == 1, sizeof(y) == 1, одна операция, которая должна заполнить y.
    // Итого - 2 ячейки.
    EXPECT_EQ(2, mapper.memorySize());
    EXPECT_EQ(1, mapper.resolveResultRef(*leftOperand));
    EXPECT_EQ(2, mapper.resolveResultRef(*operation));
}

TEST(naive_memory_mapper, for_conditional_term) {
    auto operationRightOperand = std::make_shared<Literal>(1);
    auto operationLeftOperand = std::make_shared<Variable>("x", 1, 1);
    auto operation = std::make_shared<AddOperation>();
    operation->setLeftSubtree(operationLeftOperand);
    operation->setRightSubtree(operationRightOperand);

    auto conditionRightOperand = std::make_shared<Literal>(1);
    auto conditionLeftOperand = std::make_shared<Variable>("x", 1, 1);
    auto condition = std::make_shared<EqOperation>();
    condition->setLeftSubtree(conditionLeftOperand);
    condition->setRightSubtree(conditionRightOperand);

    auto term = std::make_shared<ConditionalTerm>("y", 1, 1, condition, operation);
    auto determinant = std::make_shared<Determinant>();
    determinant->addTerm(term);

    std::deque<std::shared_ptr<Expression>> queue;
    queue.push_back(operationRightOperand);
    queue.push_back(operationLeftOperand);
    queue.push_back(conditionRightOperand);
    queue.push_back(conditionLeftOperand);
    queue.push_back(operation);
    queue.push_back(condition);
    queue.push_back(term);
    queue.push_back(determinant);

    ProgramMetadata metadata;
    metadata.inputParams.emplace_back(ParameterMetadata {
            "x", "the x", 0, 1, 1
    });
    metadata.outputParams.emplace_back(ParameterMetadata {
            "y", "the y", 0, 1, 1
    });

    NaiveMemoryMapper mapper;
    EXPECT_NO_THROW(mapper.buildMap(metadata, queue));

    EXPECT_EQ(4, mapper.memorySize());
    EXPECT_EQ(1, mapper.resolveResultRef(*operationLeftOperand));
    EXPECT_EQ(1, mapper.resolveResultRef(*conditionLeftOperand));
    EXPECT_EQ(3, mapper.resolveResultRef(*operation));
    EXPECT_EQ(4, mapper.resolveResultRef(*condition));
    EXPECT_EQ(2, mapper.resolveResultRef(*term));
}
