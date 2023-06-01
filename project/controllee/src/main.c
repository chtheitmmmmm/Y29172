#include <stdio.h>
#include "init/init.h"
#include <hv/hloop.h>

extern hio_t * main_hio;
extern hloop_t * main_loop;

int main(int argc, const char ** argv) {
    init(argv);
    hloop_run(main_loop);
    hloop_free(&main_loop);
    return 0;
}
