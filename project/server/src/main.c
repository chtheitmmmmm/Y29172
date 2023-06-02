#include "config/config.h"
#include "init/init.h"
#include "util/printlog.h"
#include "event/event.h"
#include "run/run.h"

int main(int argc, const char ** argv) {
    InitResult init_result = init(argc, argv);
    if (result_is_ok(init_result)) {
        global_events_trigger_init_event();
        run();
    } else {
        switch (result_unwrap(init_result)) {
            case InitResultErrInitEventHeap:
                event2_print_log(EVENT_LOG_ERR, "事件初始化失败（可能因为堆内存不足）.");
                break;
            case InitResultErrInitEventOther:
                event2_print_log(EVENT_LOG_ERR, "事件初始化失败。");
                break;
            case InitResultErrVictimOther:
                event2_print_log(EVENT_LOG_ERR, "受害者表初始化失败。");
                break;
            case InitResultErrServerGetIP:
                event2_print_log(EVENT_LOG_ERR, "无法找到本机内网 IP，请检查 ifconfig 命令输出。");
                break;
            case InitResultErrOther:
            default:
                event2_print_log(EVENT_LOG_ERR, "初始化失败。");
                break;
        }
    }
    return 0;
}