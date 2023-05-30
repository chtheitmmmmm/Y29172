#include "config/config.h"
#include "init/init.h"
#include "util/printlog.h"
#include "event/event.h"
#include "run/run.h"

extern Config config;
extern GlobalEvents global_events;

int main(int argc, const char ** argv) {
    InitResult init_result = init(argc, argv);
    if (result_is_ok(init_result)) {
        global_events_trigger_init_event();
        run();
    } else {
        switch (result_unwrap(init_result)) {
            case InitResultErrorInitEventHeap:
                event2_print_log(EVENT_LOG_ERR, "event init heap cannot alloc.");
                break;
            case InitResultErrorInitEventOther:
                event2_print_log(EVENT_LOG_ERR, "event init error.");
                break;
            case InitResultOther:
            default:
                event2_print_log(EVENT_LOG_ERR, "init error.");
                break;
        }
    }
    return 0;
}