#ifndef QC_PROGRAMFILEWRITERV1_H
#define QC_PROGRAMFILEWRITERV1_H

#include <queue>
#include <memory>
#include <fstream>

#include <boost/filesystem/path.hpp>
#include <memory_mapper/MemoryMapper.h>

#include "common/Expressions.h"
#include "common/ExpressionVisitor.h"
#include "common/ProgramMetadata.h"
#include "utils/LinkedHashSet.h"

class ProgramFileWriterV1 {
public:
    explicit ProgramFileWriterV1(const boost::filesystem::path &path, MemoryMapper &mapper);

    void write(const ProgramMetadata &metadata, std::deque<std::shared_ptr<Expression>> &&program);

private:
    void writeHeader();

    void writeProgramMetadata(const ProgramMetadata &metadata);

    void writeParameterMetadata(const ParameterMetadata &metadata);

    void writeStrings(const LinkedHashSet<std::string> &map);

    template<typename T>
    void write(T &&value) {
        m_stream.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }
private:
    std::ofstream m_stream;
    MemoryMapper &m_memoryMapper;
};


#endif //QC_PROGRAMFILEWRITERV1_H
