#include "ProgramFileWriterV1.h"

#include <qvm/ProgramFile.h>
#include "program_writer/CommandSerializer.h"

ProgramFileWriterV1::ProgramFileWriterV1(const boost::filesystem::path &path, MemoryMapper &mapper)
    : m_stream(path.string(), std::ios::binary)
    , m_memoryMapper(mapper)
{}

void ProgramFileWriterV1::write(const ProgramMetadata &metadata, std::deque<std::shared_ptr<Expression>> &&program) {
    writeHeader();
    writeProgramMetadata(metadata);
    for (const auto &param : metadata.inputParams) {
        writeParameterMetadata(param);
    }
    for (const auto &param : metadata.outputParams) {
        writeParameterMetadata(param);
    }
    writeStrings(metadata.strings);
    auto serializer = CommandSerializer {[this](auto cmd) { write(cmd); }, m_memoryMapper, metadata.strings };
    while (!program.empty()) {
        program.front()->accept(serializer);
        program.pop_front();
    }
}

void ProgramFileWriterV1::writeHeader() {
    ProgramFileV1::FileHeader header;
    memset(&header, 0, sizeof(header));
    header.magic = ProgramFileV1::MAGIC_NUMBER;
    strcpy(header.version, "1.0.0");
    write(header);
}

void ProgramFileWriterV1::writeProgramMetadata(const ProgramMetadata &metadata) {
    ProgramFileV1::ProgramMetadataMarshaled metadataMarshaled;
    memset(&metadataMarshaled, 0, sizeof(metadataMarshaled));
    strcpy(metadataMarshaled.description, metadata.description.c_str());
    metadataMarshaled.memorySize = metadata.memorySize;
    metadataMarshaled.inputParametersCount = metadata.inputParams.size();
    metadataMarshaled.outputParametersCount = metadata.outputParams.size();
    metadataMarshaled.commandsCount = metadata.commandsCount;
    metadataMarshaled.stringConstantsCount = metadata.strings.size();
    write(metadataMarshaled);
}

void ProgramFileWriterV1::writeParameterMetadata(const ParameterMetadata &metadata) {
    ProgramFileV1::ParameterMetadataMarshaled metadataMarshaled;
    memset(&metadataMarshaled, 0, sizeof(metadataMarshaled));
    strcpy(metadataMarshaled.name, metadata.name.c_str());
    strcpy(metadataMarshaled.description, metadata.description.c_str());
    metadataMarshaled.memoryAddress = metadata.address;
    metadataMarshaled.rows = metadata.rows;
    metadataMarshaled.columns = metadata.columns;
    write(metadataMarshaled);
}

void ProgramFileWriterV1::writeStrings(const LinkedHashMap<std::string> &strings) {
    size_t bytesWritten = 0;
    for (auto &&pair : strings) {
        auto &&str = pair.second;
        auto len = str.size() + 1;
        bytesWritten += len;
        m_stream.write(str.c_str(), len);
    }
    if (bytesWritten != 0) {
        std::fill_n(std::ostream_iterator<char>(m_stream), 16 - (bytesWritten % 16), '\0');
    }
}
