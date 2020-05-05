#include "app/CompilerApplication.h"

int main(int argn, const char *argv[]) {
    CompilerApplication app(argn, argv);
    return app.run();
}
