#include "NaiveMemoryMapper.h"
#include "common/ExpressionVisitor.h"

void NaiveMemoryMapper::buildMap(ProgramMetadata &metadata, const std::deque<std::shared_ptr<Expression>> &queue) {
    Command::Reference current = 1;
    for (auto &param : metadata.inputParams) {
        param.address = current;
        m_variable[param.name] = std::make_shared<ParameterMetadata>(param);
        current += param.size();
    }
    for (auto &param : metadata.outputParams) {
        param.address = current;
        m_variable[param.name] = std::make_shared<ParameterMetadata>(param);
        current += param.size();
    }
    ExpressionMapperVisitor visitor(*this, current);
    for (auto cmd : queue) {
        cmd->accept(visitor);
    }
    m_memorySize = current - 1;
    metadata.memorySize = m_memorySize;
}

uint64_t NaiveMemoryMapper::memorySize() const {
    return m_memorySize;
}

Command::Reference NaiveMemoryMapper::resolveResultRef(Expression &expression) {
    struct Visitor : ExpressionVisitor {
        Visitor(NaiveMemoryMapper &mapper) : m_mapper(mapper) {}

        void visit(Operation &op) override {
            auto pExpression = static_cast<Expression *>(&op);
            m_result = m_mapper.m_commandResultMap[pExpression];
        }

        void visit(Variable &variable) override {
            m_result = m_mapper.calculateVariableAddress(variable);
        }

        NaiveMemoryMapper &m_mapper;
        Command::Reference m_result = 0;
    } visitor(*this);

    expression.accept(visitor);
    return visitor.m_result;
}

Command::Reference NaiveMemoryMapper::calculateVariableAddress(Variable &variable) const {
    auto meta = m_variable.at(variable.variableName());
    if (variable.row() == 0 || variable.row() > meta->rows) {
        throw std::runtime_error("row index out of bounds for parameter \""
                                 + meta->name + "\": see " + variable.variableName() + "("
                                 + std::to_string(variable.row()) + ", "
                                 + std::to_string(variable.column()) + ")");
    }
    if (variable.column() == 0 || variable.column() > meta->columns) {
        throw std::runtime_error("column index out of bounds for parameter \""
                                 + meta->name + "\": see " + variable.variableName() + "("
                                 + std::to_string(variable.row()) + ", "
                                 + std::to_string(variable.column()) + ")");
    }
    return meta->address + meta->columns * (variable.row() - 1) + (variable.column() - 1);
}

NaiveMemoryMapper::ExpressionMapperVisitor::ExpressionMapperVisitor(NaiveMemoryMapper &mapper,
                                                                    Command::Reference &current)
        : m_mapper(mapper), m_current(current) {}

void NaiveMemoryMapper::ExpressionMapperVisitor::visit(Operation &operation) {
    auto address = static_cast<Expression *>(&operation);
    m_mapper.m_commandResultMap[address] = m_current++;
}

void NaiveMemoryMapper::ExpressionMapperVisitor::visit(NonConditionalTerm &term) {
    auto address = term.expression().get();
    m_mapper.m_commandResultMap[address] = m_mapper.calculateVariableAddress(term);

    // Ожидается, что предыдущий обрабатываемый элемент был вершиной выражения терма,
    // т.к. произошло переназначение адреса, то ячейка, которую занимало это выражение
    // освободилась, а значит её можно занять другим выражением.
    m_current--;
}
