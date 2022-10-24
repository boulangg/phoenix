#pragma once

#include "stdio.h"

#define VA_ARGS(...) , ##__VA_ARGS__
#define DEBUG_LOG(fmt, ...) \
        do { fprintf(stderr, "## %s(): " fmt, \
                     __func__ VA_ARGS(__VA_ARGS__)); } while (0)
