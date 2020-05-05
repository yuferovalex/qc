#ifndef QC_COMPILERAPPLICATION_H
#define QC_COMPILERAPPLICATION_H

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>

#include "collectors/Collector.h"
#include "memory_mapper/MemoryMapper.h"

class CompilerApplication {
public:
    CompilerApplication(int argn, const char **argv);
    void showHelp();
    void showVersion();
    int run();

private:
    void setupOptionsDescription(int argn, const char **argv);

    boost::program_options::options_description m_options;
    boost::program_options::variables_map m_variablesMap;
    std::unique_ptr<Collector> m_collector;
    std::unique_ptr<MemoryMapper> m_memoryMapper;
    boost::filesystem::path m_metaPath;
    boost::filesystem::path m_inputPath;
    boost::filesystem::path m_outputPath;
    bool m_help;
    bool m_version;
};


#endif //QC_COMPILERAPPLICATION_H
