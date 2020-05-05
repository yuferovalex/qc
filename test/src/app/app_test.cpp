#include <gtest/gtest.h>

#include "app/CompilerApplication.h"

TEST(app_test, test) {
    auto args = std::vector {
        "qvm", "-m", "resources/app/metadata.yaml", "-d", "resources/app/input.txt"
    };

    CompilerApplication app(args.size(), args.data());
    EXPECT_NO_THROW({
        EXPECT_EQ(EXIT_SUCCESS, app.run());
    });
}


TEST(app_test, test2) {
    auto args = std::vector {
        "qvm", "-m", "resources/app/zhordan/metadata.yaml", "-d", "resources/app/zhordan/input.txt",
               "-o", "zhordan.qvm"
    };

    CompilerApplication app(args.size(), args.data());
    EXPECT_NO_THROW({
        EXPECT_EQ(EXIT_SUCCESS, app.run());
    });
}