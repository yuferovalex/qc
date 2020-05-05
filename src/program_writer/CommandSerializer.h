#ifndef QC_COMMANDSERIALIZER_H
#define QC_COMMANDSERIALIZER_H

#include <cstring>

#include "qvm/ProgramFile.h"
#include "common/ExpressionVisitor.h"
#include "memory_mapper/MemoryMapper.h"
#include "utils/LinkedHashMap.h"

template <typename WriteFuncT>
class CommandSerializer final : public ExpressionVisitor {
public:
    CommandSerializer(WriteFuncT &&write, MemoryMapper &memMapper, const LinkedHashMap<std::string> &strings)
            : m_write(std::move(write)), m_memMapper(memMapper), m_strings(strings) {}

    void visit(Expression &expression) override {
        if (!m_command) {
            throw std::logic_error("unknown expression given for serialization (" + expression.name() + ")");
        }
        m_command->resultRef = m_memMapper.resolveResultRef(expression);
        m_write(*m_command);
        m_command.reset();
    }

    void visit(UnaryOperation &op) override {
        if (!m_command) {
            throw std::logic_error("unknown unary operation given for serialization (" + op.name() + ")");
        }
        if (auto literal = std::dynamic_pointer_cast<Literal>(op.subtree())) {
            m_command->lhsOperand.value.number = literal->value();
        } else {
            m_command->operation |= Command::Flags::LHS_OP_REF;
            m_command->lhsOperand.ref = m_memMapper.resolveResultRef(*op.subtree());
        }
        visit(static_cast<Expression &>(op));
    }

    void visit(BinaryOperation &op) override {
        if (!m_command) {
            throw std::logic_error("unknown binary operation given for serialization (" + op.name() + ")");
        }
        if (auto literal = std::dynamic_pointer_cast<Literal>(op.leftSubtree())) {
            m_command->lhsOperand.value.number = literal->value();
        } else {
            m_command->operation |= Command::Flags::LHS_OP_REF;
            m_command->lhsOperand.ref = m_memMapper.resolveResultRef(*op.leftSubtree());
        }
        if (auto literal = std::dynamic_pointer_cast<Literal>(op.rightSubtree())) {
            m_command->rhsOperand.value.number = literal->value();
        } else {
            m_command->operation |= Command::Flags::RHS_OP_REF;
            m_command->rhsOperand.ref = m_memMapper.resolveResultRef(*op.rightSubtree());
        }
        visit(static_cast<Expression &>(op));
    }

    void visit(AddOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::ADD;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(MulOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::MUL;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(SubOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::SUB;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(DivOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::DIV;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(EqOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::EQ;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(NeOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::NE;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(AndOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::AND;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(OrOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::OR;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(LtOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::LT;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(LeOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::LE;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(GtOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::GT;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(GeOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::GE;
        visit(static_cast<BinaryOperation &>(op));
    }

    void visit(NegOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::NEG;
        visit(static_cast<UnaryOperation &>(op));
    }

    void visit(AbsOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::ABS;
        visit(static_cast<UnaryOperation &>(op));
    }

    void visit(SqrtOperation &op) override {
        createCommand();
        m_command->operation = Command::Operation::SQRT;
        visit(static_cast<UnaryOperation &>(op));
    }

    void visit(ConditionalTerm &term) override {
        createCommand();
        m_command->operation = Command::Operation::LQT;
        if (auto literal = std::dynamic_pointer_cast<Literal>(term.condition())) {
            m_command->lhsOperand.value.boolean = literal->value();
        } else {
            m_command->operation |= Command::Flags::LHS_OP_REF;
            m_command->lhsOperand.ref = m_memMapper.resolveResultRef(*term.condition());
        }
        if (auto literal = std::dynamic_pointer_cast<Literal>(term.expression())) {
            m_command->rhsOperand.value.number = literal->value();
        } else if (auto halt = std::dynamic_pointer_cast<HaltExpression>(term.expression())) {
            m_command->rhsOperand.value.stringIndex = m_strings.indexOf(halt->reason());
        } else {
            m_command->operation |= Command::Flags::RHS_OP_REF;
            m_command->rhsOperand.ref = m_memMapper.resolveResultRef(*term.expression());
        }
        visit(static_cast<Expression &>(term));
    }

    void visit(Determinant &determinant) override {
    }

    void visit(TerminalExpression &expression) override {
    }

    void visit(Literal &number) override {
    }

    void visit(Variable &variable) override {
    }

    void visit(Term &term) override {
    }

    void visit(NonConditionalTerm &term) override {
    }

    void visit(HaltExpression &) override {
    }

private:
    void createCommand() {
        ProgramFileV1::CommandWithHints command;
        memset(&command, 0, sizeof(ProgramFileV1::CommandWithHints));
        m_command = command;
    }

    std::optional<ProgramFileV1::CommandWithHints> m_command;

    const LinkedHashMap<std::string> &m_strings;
    WriteFuncT m_write;
    MemoryMapper &m_memMapper;
};

#endif //QC_COMMANDSERIALIZER_H
