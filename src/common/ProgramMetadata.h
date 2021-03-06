#ifndef QC_PROGRAMMETADATA_H
#define QC_PROGRAMMETADATA_H

#include <vector>

#include "qvm/ParameterMetadata.h"
#include "utils/LinkedHashSet.h"

struct ProgramMetadata {
    uint64_t memorySize;
    uint64_t commandsCount;
    std::string description;
    LinkedHashSet<std::string> strings;
    std::vector<ParameterMetadata> inputParams;
    std::vector<ParameterMetadata> outputParams;
};

#endif //QC_PROGRAMMETADATA_H
