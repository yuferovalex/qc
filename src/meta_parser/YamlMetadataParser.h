#ifndef QC_YAMLMETADATAPARSER_H
#define QC_YAMLMETADATAPARSER_H

#include <boost/filesystem/path.hpp>

#include "common/ProgramMetadata.h"

namespace YamlMetadataParser {
    ProgramMetadata parse(const boost::filesystem::path &path);
}


#endif //QC_YAMLMETADATAPARSER_H
