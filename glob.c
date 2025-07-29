#include <glob.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fnmatch.h>

static int match_char(char p, char s, int flags) {
    return p == s;
}

static int internal_fnmatch(const char *pattern, const char *string, int flags) {
    while (*pattern) {
        switch (*pattern) {
            case '?':
                if (*string == '\0' || ((flags & FNM_PATHNAME) && *string == '/'))
                    return FNM_NOMATCH;
                ++pattern;
                ++string;
                break;

            case '*': {
                pattern++;
                if (!*pattern)
                    return ((flags & FNM_PATHNAME) && strchr(string, '/')) ? FNM_NOMATCH : 0;

                while (*string) {
                    if (!internal_fnmatch(pattern, string, flags))
                        return 0;
                    if ((flags & FNM_PATHNAME) && *string == '/')
                        break;
                    string++;
                }
                return FNM_NOMATCH;
            }

            case '\\':
                if (!(flags & FNM_NOESCAPE)) {
                    ++pattern;
                    if (!*pattern)
                        return FNM_NOMATCH;
                }
                /* fallthrough */

            default:
                if (!match_char(*pattern, *string, flags))
                    return FNM_NOMATCH;
                ++pattern;
                ++string;
                break;
        }
    }

    return *string ? FNM_NOMATCH : 0;
}

int fnmatch(const char *pattern, const char *string, int flags) {
    if (!pattern || !string)
        return FNM_NOMATCH;
    return internal_fnmatch(pattern, string, flags);
}

int glob(const char *pattern, int flags, int (*errfunc)(const char *, int), glob_t *pglob) {
    DIR *dir;
    struct dirent *entry;
    const char *slash = strrchr(pattern, '/');
    char dirname[256] = ".";
    char prefix[256] = "";
    int pathc = 0;

    if (!(flags & GLOB_APPEND)) {
        pglob->gl_pathc = 0;
        pglob->gl_matchc = 0;
        pglob->gl_pathv = NULL;
    }

    pglob->gl_flags = flags;
    pglob->gl_errfunc = errfunc;

    if (slash) {
        size_t len = slash - pattern;
        if (len >= sizeof(dirname)) return GLOB_NOSPACE;
        memcpy(dirname, pattern, len);
        dirname[len] = '\0';
        pattern = slash + 1;
        snprintf(prefix, sizeof(prefix), "%s/", dirname);
    }

    dir = opendir(dirname);
    if (!dir) {
        if (errfunc && (flags & GLOB_ERR)) errfunc(dirname, errno);
        return GLOB_ABEND;
    }

    while ((entry = readdir(dir))) {
        if (fnmatch(pattern, entry->d_name, 0) == 0) {
            char *match;
            size_t len = strlen(prefix) + strlen(entry->d_name) + 2;
            char **newv;

            match = malloc(len);
            if (!match) {
                closedir(dir);
                return GLOB_NOSPACE;
            }
            snprintf(match, len, "%s%s%s", prefix, entry->d_name,
                     (flags & GLOB_MARK) ? "/" : "");

            newv = realloc(pglob->gl_pathv, sizeof(char *) * (pglob->gl_pathc + pglob->gl_offs + 2));
            if (!newv) {
                free(match);
                closedir(dir);
                return GLOB_NOSPACE;
            }
            pglob->gl_pathv = newv;

            if (flags & GLOB_DOOFFS) {
                if (pglob->gl_pathc == 0) {
                    for (int i = 0; i < pglob->gl_offs; i++) {
                        pglob->gl_pathv[i] = NULL;
                    }
                }
                pglob->gl_pathv[pglob->gl_offs + pglob->gl_pathc] = match;
                pglob->gl_pathv[pglob->gl_offs + pglob->gl_pathc + 1] = NULL;
            } else {
                pglob->gl_pathv[pglob->gl_pathc] = match;
                pglob->gl_pathv[pglob->gl_pathc + 1] = NULL;
            }

            pglob->gl_pathc++;
            pglob->gl_matchc++;
            pathc++;
        }
    }

    closedir(dir);

    if (pathc == 0) {
        if (flags & GLOB_NOCHECK) {
            size_t len = strlen(pattern) + 1;
            char *copy = malloc(len);
            if (!copy) return GLOB_NOSPACE;
            memcpy(copy, pattern, len);
            char **newv = realloc(pglob->gl_pathv, sizeof(char *) * (pglob->gl_pathc + pglob->gl_offs + 2));
            if (!newv) {
                free(copy);
                return GLOB_NOSPACE;
            }
            pglob->gl_pathv = newv;
            if (flags & GLOB_DOOFFS) {
                pglob->gl_pathv[pglob->gl_offs + pglob->gl_pathc] = copy;
                pglob->gl_pathv[pglob->gl_offs + pglob->gl_pathc + 1] = NULL;
            } else {
                pglob->gl_pathv[pglob->gl_pathc] = copy;
                pglob->gl_pathv[pglob->gl_pathc + 1] = NULL;
            }
            pglob->gl_pathc++;
            return 0;
        }
        return GLOB_NOMATCH;
    }

    return 0;
}

void globfree(glob_t *pglob) {
    if (!pglob || !pglob->gl_pathv) return;
    int start = (pglob->gl_flags & GLOB_DOOFFS) ? pglob->gl_offs : 0;
    for (int i = start; i < pglob->gl_pathc + start; i++) {
        free(pglob->gl_pathv[i]);
    }
    free(pglob->gl_pathv);
    pglob->gl_pathv = NULL;
    pglob->gl_pathc = 0;
    pglob->gl_matchc = 0;
}