#include <string.h>

char *dirname(char *path) {
    static char dot[] = ".";
    char *last_slash;

    if (path == NULL || *path == '\0')
        return dot;

    /* Remove trailing slashes */
    size_t len = strlen(path);
    while (len > 1 && path[len - 1] == '/')
        path[--len] = '\0';

    last_slash = strrchr(path, '/');
    if (!last_slash)
        return dot;

    /* Remove trailing slashes from the dirname part */
    while (last_slash > path && *last_slash == '/')
        *last_slash-- = '\0';

    if (last_slash == path && *last_slash == '/')
        last_slash[1] = '\0';
    else if (last_slash > path)
        *last_slash = '\0';
    else
        return dot;

    return path;
}
