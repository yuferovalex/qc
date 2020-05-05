#ifndef QC_MEMORYMAPPER_H
#define QC_MEMORYMAPPER_H

#include <qvm/Command.h>

#include "common/ProgramMetadata.h"
#include "common/Expressions.h"

class MemoryMapper {
public:
    virtual ~MemoryMapper() = default;

    virtual uint64_t memorySize() const = 0;

    virtual Command::Reference resolveResultRef(Expression &expr) = 0;

    virtual void buildMap(ProgramMetadata &metadata,
                          const std::deque<std::shared_ptr<Expression>> &queue) = 0;
};

#endif //QC_MEMORYMAPPER_H
