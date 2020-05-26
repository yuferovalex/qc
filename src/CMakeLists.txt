add_library(qc_lib STATIC
        common/Expressions.cpp
        common/Expressions.h
        common/ExpressionVisitor.h
        det_parser/DeterminantParser.cpp
        det_parser/DeterminantParser.h
        det_parser/NodeParser.cpp
        det_parser/NodeParser.h
        det_parser/ParserUtils.cpp
        det_parser/ParserUtils.h
        collectors/ReverseBfsChildrenCollector.cpp
        collectors/ReverseBfsChildrenCollector.h
        collectors/Collector.h
        meta_parser/YamlMetadataParser.cpp
        meta_parser/YamlMetadataParser.h
        app/CompilerApplication.cpp
        app/CompilerApplication.h
        common/ProgramMetadata.h
        program_writer/ProgramFileWriterV1.cpp
        program_writer/ProgramFileWriterV1.h
        memory_mapper/MemoryMapper.h
        memory_mapper/NaiveMemoryMapper.cpp
        memory_mapper/NaiveMemoryMapper.h
        program_writer/CommandSerializer.h utils/LinkedHashSet.h)

target_link_libraries(qc_lib
        ${Boost_LIBRARIES}
        yaml-cpp
        )

