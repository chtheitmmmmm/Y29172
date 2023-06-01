//
// Created by cmtheit on 23-6-1.
//

#include "victim.h"
#include "../util/printlog.h"

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
        hio_setcb_close(io, victim_on_close);
    }
    return v;
}

void victim_on_close(hio_t * io) {
    Victim * v = victim_get_by_io_ptr(io);
    if (v) {
        pthread_mutex_lock(&v->lock);
        int leave_id = v->id;
        if (v->pprev) {
            struct Victim * prev= (struct Victim *)((uintptr_t) v->pprev - offsetof(struct Victim, next));
            pthread_mutex_lock(&prev->lock);
            *(v->pprev) = v->next;
            pthread_mutex_unlock(&prev->lock);
        }
        pthread_mutex_unlock(&v->lock);
        pthread_mutex_destroy(&v->lock);
        free(v);
        print_logl("受害者 "CHALK_YELLOW("%d")" 断开连接", leave_id);
    } else {
        event2_print_log(EVENT_LOG_ERR, "无法按照 fd 找到受害者，服务器内部错误。");
    }
}

void victim_free(Victim * victim) {
    if (victim) {
        victim_free(victim->next);
        pthread_mutex_destroy(&victim->lock);
        hio_close(victim->io);
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
}

void victims_quit() {
    pthread_mutex_lock(&victims_lock);
    victim_free(victims);
    pthread_mutex_unlock(&victims_lock);
    pthread_mutex_destroy(&victims_lock);
}

