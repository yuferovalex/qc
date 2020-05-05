#include "CompilerApplication.h"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <program_writer/ProgramFileWriterV1.h>

#include "app/Config.h"
#include "meta_parser/YamlMetadataParser.h"
#include "det_parser/DeterminantParser.h"
#include "collectors/ReverseBfsChildrenCollector.h"
#include "memory_mapper/NaiveMemoryMapper.h"

CompilerApplication::CompilerApplication(int argn, const char **argv)
    : m_collector(std::make_unique<ReverseBfsChildrenCollector>())
    , m_memoryMapper(std::make_unique<NaiveMemoryMapper>())
{
    setupOptionsDescription(argn, argv);
}

int CompilerApplication::run() {
    try {
        if (m_version) {
            showVersion();
            return EXIT_SUCCESS;
        }

        if (m_help) {
            showHelp();
            return EXIT_SUCCESS;
        }

        auto programMetadata = YamlMetadataParser::parse(m_metaPath);
        DeterminantParser determinantParser;
        auto determinant = determinantParser.parse(m_inputPath);
        programMetadata.commandsCount = determinant->commandsCount();
        programMetadata.strings = std::move(determinantParser.strings);
        auto commandsQueue = m_collector->collect(determinant);
        m_memoryMapper->buildMap(programMetadata, commandsQueue);
        ProgramFileWriterV1 writer(m_outputPath, *m_memoryMapper);
        writer.write(programMetadata, std::move(commandsQueue));

        return EXIT_SUCCESS;
    }
    catch (std::exception &e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
    }
    return EXIT_FAILURE;
}

void CompilerApplication::setupOptionsDescription(int argn, const char **argv) {
    boost::program_options::options_description description("QC options");
    description.add_options()
            (
                    "help,h",
                    boost::program_options::bool_switch(&m_help)->default_value(false),
                    "show this help"
            )
            (
                    "version,v",
                    boost::program_options::bool_switch(&m_version)->default_value(false),
                    "show version of QC"
            )
            (
                    "determinant,d",
                    boost::program_options::value(&m_inputPath)->default_value("input.txt"),
                    "path to determinant for compilation"
            )
            (
                    "output,o",
                    boost::program_options::value(&m_outputPath)->default_value("program.qvm"),
                    "path for output compiled program"
            )
            (
                    "metadata,m",
                    boost::program_options::value(&m_metaPath)->default_value("metadata.yaml"),
                    "metadata of program"
            )
            ;
    m_options.add(description);

    boost::program_options::store(
            boost::program_options::parse_command_line(argn, argv, description),
            m_variablesMap
    );
    boost::program_options::notify(m_variablesMap);
}

void CompilerApplication::showHelp() {
    std::cout << m_options;
}

void CompilerApplication::showVersion() {
    std::cout << PROJECT_NAME << " " << PROJECT_VER << std::endl;
}
