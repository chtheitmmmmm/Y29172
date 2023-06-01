#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    char* executablePath = argv[0];
    char* executableDir = dirname(executablePath);

    printf("%s\n", executableDir);
    return 0;
}