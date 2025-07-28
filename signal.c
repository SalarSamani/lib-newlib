#include <signal.h>
#include <string.h>
#include <stdlib.h>

/* Maximum number of supported signals */
#define MAX_SIGNALS 32

/* Table of registered signal actions */
static struct sigaction sig_table[MAX_SIGNALS];

/*
 * sigaction - register or retrieve a signal handler
 */
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    if (signum <= 0 || signum >= MAX_SIGNALS) {
        return -1;
    }

    /* Retrieve old action if requested */
    if (oldact) {
        memcpy(oldact, &sig_table[signum], sizeof(struct sigaction));
    }

    /* Register new action if provided */
    if (act) {
        memcpy(&sig_table[signum], act, sizeof(struct sigaction));
    }

    return 0;
}

/*
 * signal - simplified signal() API using sigaction
 */
void (*signal(int signum, void (*handler)(int)))(int)
{
    struct sigaction sa, oldsa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;

    if (sigaction(signum, &sa, &oldsa) < 0) {
        return SIG_ERR;
    }

    return oldsa.sa_handler;
}

/*
 * raise - invoke the signal handler for the calling thread
 */
int raise(int sig)
{
    struct sigaction act;

    if (sig <= 0 || sig >= MAX_SIGNALS) {
        return -1;
    }

    act = sig_table[sig];

    /* If handler is SIG_IGN, ignore */
    if (act.sa_handler == SIG_IGN) {
        return 0;
    }

    /* If custom handler, call it */
    if (act.sa_handler && act.sa_handler != SIG_DFL) {
        act.sa_handler(sig);
        return 0;
    }

    /* Default actions */
    switch (sig) {
    case SIGABRT:
        abort();
        break;
    case SIGINT:
    case SIGTERM:
        exit(0);
        break;
    default:
        /* Unknown signals: ignore */
        break;
    }

    return -1;
}