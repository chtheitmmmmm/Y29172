//
// Created by cmtheit on 23-5-27.
//

#ifndef SERVER_OPTION_H
#define SERVER_OPTION_H
#define OPTION_TYPE_NONE 0
#define OPTION_TYPE_SOME 1

#define Option(T) struct { \
     int type;  \
     T value;\
}

#define option_new_none() { \
    .type = OPTION_TYPE_NONE,           \
};

#define option_new_some(v) { \
    .type  = OPTION_TYPE_SOME,                               \
    .value = v\
}

// if option is option_type_none
#define option_is_none(option) (option.type == OPTION_TYPE_NONE)

// if option is option_type_some
#define option_is_some(option) (option.type == OPTION_TYPE_SOME)

// unwrap option's value
#define option_unwrap(option)  (option.value)

// turn option to option_type_some
#define option_turn_some(option, v) { \
    option.type = OPTION_TYPE_SOME;   \
    option.value = v;                 \
}

// turn option to option_type_none
#define option_turn_none(option) {       \
    option.type = OPTION_TYPE_NONE;       \
}

#endif //SERVER_OPTION_H
