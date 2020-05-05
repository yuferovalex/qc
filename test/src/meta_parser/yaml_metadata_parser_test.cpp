#include <gtest/gtest.h>

#include "meta_parser/YamlMetadataParser.h"

TEST(yaml_metadata_parser, full_meta_test) {
    ProgramMetadata result;
    EXPECT_NO_THROW(
            result = YamlMetadataParser::parse(
                    "resources/meta_parser/program-meta.yaml")
    );

    EXPECT_EQ("This is a description of program",
              result.description);

    ASSERT_EQ(2, result.inputParams.size());
    EXPECT_EQ("firstParameter", result.inputParams[0].name);
    EXPECT_EQ("This is a description of the first parameter (scalar)",
              result.inputParams[0].description);
    EXPECT_EQ(1, result.inputParams[0].rows);
    EXPECT_EQ(1, result.inputParams[0].columns);

    EXPECT_EQ("secondParameter", result.inputParams[1].name);
    EXPECT_EQ("This is a description of the second parameter (matrix)",
              result.inputParams[1].description);
    EXPECT_EQ(2, result.inputParams[1].rows);
    EXPECT_EQ(3, result.inputParams[1].columns);

    ASSERT_EQ(1, result.outputParams.size());
    EXPECT_EQ("thirdParameter", result.outputParams[0].name);
    EXPECT_EQ("This is a description of the third parameter (scalar)",
              result.outputParams[0].description);
    EXPECT_EQ(1, result.outputParams[0].rows);
    EXPECT_EQ(1, result.outputParams[0].columns);
}

TEST(yaml_metadata_parser, meta_without_params) {
    ProgramMetadata result;
    EXPECT_NO_THROW(
            result = YamlMetadataParser::parse(
                    "resources/meta_parser/program-meta-without-params.yaml")
    );

    EXPECT_EQ("This is a description of program", result.description);

    EXPECT_EQ(0, result.inputParams.size());
    EXPECT_EQ(0, result.outputParams.size());
}