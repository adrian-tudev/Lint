#include "utils/error.h"

#include <stdarg.h>
#include <stdio.h>

#include "utils/color.h"

static bool g_error_enabled = true;

void error_set_enabled(bool enabled) { g_error_enabled = enabled; }
bool error_is_enabled(void) { return g_error_enabled; }

void error_log(const char *fmt, ...) {
    if (!g_error_enabled) return;

    va_list args;
    va_start(args, fmt);

    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);

    fprintf(stderr, "%s%s%s",
            color("Error: ", ColorRED),
            color(buf, ColorRED),
            color("", ColorWHT));

    va_end(args);
}