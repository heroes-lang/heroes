/* The C the case of defect 2696 calls. On Windows the stop's bell of
 * `hero_poll.h` is a loopback datagram socket: bound and never connected, one
 * datagram from any other socket made it readable, and every later wait
 * failed `stopped` with no stop asked. Here a set is made, so the bell is,
 * and a socket of its own sends one datagram to every UDP port this process
 * holds on the loopback, read from the system's table (`GetExtendedUdpTable`,
 * loaded by name so no library is linked); the set's next wait must answer
 * its entries, a timer fallen due, and not fail. Answers 1 where every step
 * held, or on Darwin and Linux, whose bell is a pipe the process alone holds;
 * else the negative number of the step that did not. */
#ifndef FIXEDBUGS_2696_STRANGERS_H
#define FIXEDBUGS_2696_STRANGERS_H

#include "hero_poll.h"

#include <stdint.h>
#include <string.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iphlpapi.h>

typedef DWORD(WINAPI *strangers_table_fn)(PVOID, PDWORD, BOOL, ULONG, UDP_TABLE_CLASS, ULONG);

/* How many loopback ports of this process got a stranger's datagram, or -1. */
static int64_t strangers_sent(void) {
    HMODULE ip = LoadLibraryA("iphlpapi.dll");
    strangers_table_fn table_of = ip == NULL ? NULL : (strangers_table_fn)(void (*)(void))GetProcAddress(ip, "GetExtendedUdpTable");
    DWORD size = 0;
    int64_t sent = 0;

    if (table_of == NULL) {
        return -1;
    }
    (void)table_of(NULL, &size, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0);
    /* The table, and room for the rows a socket made meanwhile adds. */
    static unsigned char room[65536];
    MIB_UDPTABLE_OWNER_PID *table = (MIB_UDPTABLE_OWNER_PID *)(void *)room;

    size = (DWORD)sizeof room;
    if (table_of(table, &size, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0) != NO_ERROR) {
        return -1;
    }
    SOCKET stranger = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (stranger == INVALID_SOCKET) {
        return -1;
    }
    for (DWORD i = 0; i < table->dwNumEntries; i += 1) {
        const MIB_UDPROW_OWNER_PID *row = &table->table[i];
        struct sockaddr_in to;

        if (row->dwOwningPid != GetCurrentProcessId() || row->dwLocalAddr != htonl(INADDR_LOOPBACK)) {
            continue;
        }
        memset(&to, 0, sizeof to);
        to.sin_family = AF_INET;
        to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        to.sin_port = (u_short)row->dwLocalPort;
        if (sendto(stranger, "x", 1, 0, (const struct sockaddr *)&to, (int)sizeof to) == 1) {
            sent += 1;
        }
    }
    (void)closesocket(stranger);
    Sleep(100);
    return sent;
}
#endif

static int64_t strangers_leave_the_wait(void) {
#if defined(_WIN32)
    hero_poll_set *set = hero_poll_make(1);
    int64_t events[3];
    int64_t answer = 1;

    if (set == NULL) {
        return -1;
    }
    int64_t timer = hero_poll_add_timer(set, 0, 0);

    if (timer <= 0) {
        answer = -2;
    } else if (strangers_sent() < 1) {
        answer = -3;
    } else if (hero_poll_wait(set, 0, events, 3) != 1 || events[0] != timer) {
        answer = -4;
    } else if (hero_poll_wait(set, 0, events, 3) != 0) {
        answer = -5;
    }
    (void)hero_poll_close(set);
    return answer;
#else
    return 1;
#endif
}

#endif
