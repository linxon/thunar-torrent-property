#include <stdlib.h>
#include <stdio.h>

#include "human-size.h"

char* human_size(double size) {
    int i = 0;
    const char* units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }

    int bufsize = snprintf(NULL, 0, "%.1f %s", size, units[i]);
    char* buf = malloc(bufsize + 1);
    sprintf(buf, "%.1f %s", size, units[i]);

    return buf;
}
