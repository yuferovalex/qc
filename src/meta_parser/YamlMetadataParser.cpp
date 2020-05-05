#include "YamlMetadataParser.h"

#include <yaml-cpp/yaml.h>

ParameterMetadata parseParameterMetadata(const YAML::Node &paramNode) {
    ParameterMetadata parameterMetadata;
    parameterMetadata.name = paramNode["name"].as<std::string>();
    parameterMetadata.description = paramNode["description"].as<std::string>();
    parameterMetadata.rows = paramNode["rows"].IsDefined()
                             ? paramNode["rows"].as<uint64_t>()
                             : 1;
    parameterMetadata.columns = paramNode["columns"].IsDefined()
                                ? paramNode["columns"].as<uint64_t>()
                                : 1;
    return parameterMetadata;
}

ProgramMetadata YamlMetadataParser::parse(const boost::filesystem::path &path) {
    auto root = YAML::LoadFile(path.string());
    ProgramMetadata programMetadata;
    programMetadata.description = root["description"].as<std::string>();
    for (auto paramNode : root["input parameters"]) {
        programMetadata.inputParams.push_back(parseParameterMetadata(paramNode));
    }
    for (auto paramNode : root["output parameters"]) {
        programMetadata.outputParams.push_back(parseParameterMetadata(paramNode));
    }
    return programMetadata;
}
