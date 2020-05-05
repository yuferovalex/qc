#ifndef QC_NAIVEMEMORYMAPPER_H
#define QC_NAIVEMEMORYMAPPER_H

#include <deque>
#include <unordered_map>

#include "common/ExpressionVisitor.h"
#include "memory_mapper/MemoryMapper.h"

class NaiveMemoryMapper final : public MemoryMapper {
public:
    NaiveMemoryMapper() = default;

    void buildMap(ProgramMetadata &metadata,
                  const std::deque<std::shared_ptr<Expression>> &queue) override;

    uint64_t memorySize() const override;

    Command::Reference resolveResultRef(Expression &expression) override;

private:
    Command::Reference calculateVariableAddress(Variable &variable) const;

    class ExpressionMapperVisitor final : public ExpressionVisitor {
    public:
        ExpressionMapperVisitor(NaiveMemoryMapper &mapper, Command::Reference &current);

        void visit(Operation &operation) override;

        void visit(NonConditionalTerm &term) override;

    private:
        NaiveMemoryMapper &m_mapper;
        Command::Reference &m_current;
    };

private:
    std::unordered_map<std::string, std::shared_ptr<ParameterMetadata>> m_variable;
    std::unordered_map<Expression *, Command::Reference> m_commandResultMap;
    uint64_t m_memorySize;
};

#endif //QC_NAIVEMEMORYMAPPER_H
