/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * libnewlib glue code
 *
 * Authors: Salar Samani <soltanisamani@gmail.com>
 *
 * Copyright (c) 2017, NEC Europe Ltd., NEC Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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

#include <signal.h>
#include <errno.h>
#include <sys/select.h>

int sigsuspend(const sigset_t *mask)
{
    if (!mask) {
        errno = EINVAL; return -1;
    } else {
        return 0;
    }
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

