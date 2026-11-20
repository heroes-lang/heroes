/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_tls.h — the platform layer's TLS: a connection whose bytes are
 * encrypted and whose peer is checked, handed out as a `hero_stream`
 * (M-rebel-library; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h`
 * is: its functions are defined in `platform/hero_tls.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_tls.h"`, with no `link` word and no flag.
 *
 * THE SYSTEM'S OWN TLS ON EACH PLATFORM (the author's choice of
 * 2026-11-16, the route of Rust's `native-tls`), so that a certificate is
 * checked against the system's own store of authorities and a security
 * update of the system reaches every program:
 *
 *   - Windows: SChannel, through SSPI, the chain checked by
 *     `CertGetCertificateChain` and `CertVerifyCertificateChainPolicy` with
 *     the SSL policy and the host name. Only a program that takes the
 *     member links `secur32`, `crypt32`, `ncrypt` and `bcrypt`. SChannel
 *     takes a server's key only from the key store: a listener's key is
 *     stored under a name of its own while the listener lives and removed
 *     when it closes, and a process killed meanwhile leaves it there.
 *   - macOS: Network.framework, whose connection makes the socket itself,
 *     and the Security framework's trust evaluation. Nothing deprecated:
 *     Secure Transport, Apple's one API that runs TLS over a socket made by
 *     somebody else, is deprecated since macOS 10.15, so on macOS a TLS
 *     connection is made only by `hero_tls_connect` and `hero_tls_accept`,
 *     and `hero_tls_client` and `hero_tls_server`, which run TLS over a
 *     stream the program already holds, answer `HERO_TLS_UNSUPPORTED`.
 *   - Linux: OpenSSL 3, `libssl.so.3`, loaded when a program first asks for
 *     TLS and never named on the link line, so the layer compiles and links
 *     on a machine with no OpenSSL at all, and there every call answers
 *     `HERO_TLS_UNSUPPORTED`.
 *
 * TLS 1.2 AT THE LEAST, ON BOTH SIDES, AND NOTHING WEAKER; TLS 1.3 where
 * both ends offer it. No call takes a version, a cipher or an option that
 * turns a check off, but the one constant whose name says it is for tests.
 *
 * A CERTIFICATE REFUSED SAYS WHY: past its dates, for another host, signed
 * by no authority trusted, revoked, or otherwise unfit, each its own kind
 * below, and `hero_tls_last_message` gives the system's own words.
 *
 * TEXT AND BYTES. A host name is UTF-8 text; a certificate, a chain and a
 * key are handed as PEM, the text OpenSSL, Go and Python read, as bytes and
 * their count. A list of application protocols (ALPN) is text, the names
 * separated by `,`, as `"h2,http/1.1"`; "" offers none.
 *
 * NO PLATFORM TYPE AND NO CONDITIONAL BUT THE GUARD: a group sees
 * `int64_t`, `uint64_t`, `void *`, `const char *`, the stream of
 * `hero_stream.h` and the listener below.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a stream
 * or a listener answers NULL, one answering a count -1, and
 * `hero_tls_last_error()` then names the kind. A listener is a handle the
 * layer issues and checks, so one closed already, NULL, or one this layer
 * never issued is `HERO_TLS_NOT_OPEN`, never a read of freed memory. A TLS
 * stream is a `hero_stream`, checked by that layer, read, written and
 * closed by `hero_stream_read`, `hero_stream_write` and `hero_stream_close`,
 * whose failures `hero_stream_last_error()` names.
 */

#ifndef HERO_TLS_H
#define HERO_TLS_H

#include <stdint.h>

#include "hero_stream.h"

/* A socket listening for TLS connections: a group declares it as a handle,
 * `record Listener tag hero_tls_listener`, which `hero_tls_close_listener`
 * gives back. */
typedef struct hero_tls_listener hero_tls_listener;

/* The kinds of failure, the same numbers on every platform; the platform's
 * own number for the last failure is `hero_tls_last_os_error()`. The kinds
 * a stream also has keep `hero_stream.h`'s numbers. */
#define HERO_TLS_OK 0
/* An argument the layer refuses: no host, a port outside 0 to 65535, a
 * time below -1 (`HERO_TLS_FOREVER`), a count with no bytes behind it, PEM that holds no
 * certificate or no key, a key of a kind no platform here takes, or a key
 * that is not the certificate's. */
#define HERO_TLS_INVALID 1
/* The stream or the listener is none this layer holds open. */
#define HERO_TLS_NOT_OPEN 2
/* The host name has no address. */
#define HERO_TLS_NOT_FOUND 3
/* The system refused the port or the address to this program. */
#define HERO_TLS_DENIED 5
/* The process or the machine has no memory, socket or handle left. */
#define HERO_TLS_LIMIT 8
/* The peer closed the connection, or reset it, before the handshake ended. */
#define HERO_TLS_CLOSED_PEER 10
/* This platform has no such thing: no TLS library on this Linux machine,
 * or TLS over a stream the program holds on macOS (above). */
#define HERO_TLS_UNSUPPORTED 11
#define HERO_TLS_OTHER 12
/* The time given passed before the connection and its handshake were made. */
#define HERO_TLS_TIMED_OUT 16
/* The program asked to stop (`hero_net_ask_stop`, or Ctrl-C after
 * `hero_net_catch_stop`) while the call waited. */
#define HERO_TLS_STOPPED 17
/* Nothing listens at that address and port. */
#define HERO_TLS_REFUSED 20
/* No route reaches the host. */
#define HERO_TLS_UNREACHABLE 21
/* Something listens at that address and port already. */
#define HERO_TLS_IN_USE 22
/* The peer's certificate, or one of its chain, is past its last day or
 * before its first. */
#define HERO_TLS_CERT_EXPIRED 30
/* The peer's certificate names other hosts than the one asked for. */
#define HERO_TLS_CERT_HOST 31
/* No authority the check trusts signed the peer's certificate: one signed
 * by itself, or by an authority neither the system nor the roots given
 * hold. */
#define HERO_TLS_CERT_UNTRUSTED 32
/* The authority that signed the peer's certificate has revoked it. */
#define HERO_TLS_CERT_REVOKED 33
/* The peer's certificate was refused for any other reason: a signature that
 * does not hold, a use it is not for, bytes that are no certificate. */
#define HERO_TLS_CERT_INVALID 34
/* The handshake failed: the two ends share no version or cipher, the peer
 * sent an alert, or what it sent is not TLS. */
#define HERO_TLS_HANDSHAKE 35
/* A step of the exchange before TLS was not met: the peer answered other
 * bytes than those expected, or a line or a step passed its bound. The words
 * show the bytes expected and the bytes heard, escaped. */
#define HERO_TLS_PREAMBLE 36

/* THE EXCHANGE BEFORE TLS (`hero_tls_connect_after`, `hero_tls_listen_after`):
 * what Postgres's SSLRequest, SMTP's and IMAP's STARTTLS and a proxy's
 * CONNECT write and read in plain text before TLS begins on the same
 * connection. It runs on the connection the layer makes, below TLS: on
 * macOS inside the one Network.framework connection (a framer between TCP
 * and TLS), elsewhere on the socket before TLS starts over it. No byte of
 * TLS leaves before the last step is met, and none of the peer's bytes past
 * that step is read as the exchange's.
 *
 * The steps are one byte string: each step a kind, one byte, the length of
 * its bytes, four bytes, most significant first, then those bytes. */
/* Write the bytes to the peer. */
#define HERO_TLS_STEP_SEND 1
/* Read exactly as many bytes, which must be these. */
#define HERO_TLS_STEP_EXPECT 2
/* Read a line, up to and with its `\n`, which must begin with these bytes;
 * a line that begins with them and then `-` is a continuation (SMTP's
 * `250-`), read past, until the line that does not. */
#define HERO_TLS_STEP_EXPECT_LINE 3
/* The most steps, the most bytes a step sends or expects, the most bytes of
 * one line read, its end included, the most bytes one step reads in every
 * line of it, and the most bytes of a line's beginning. */
#define HERO_TLS_STEPS_MOST UINT64_C(64)
#define HERO_TLS_STEP_BYTES_MOST UINT64_C(65536)
#define HERO_TLS_LINE_MOST UINT64_C(4096)
#define HERO_TLS_PREFIX_MOST UINT64_C(64)

/* What `checks` takes: every check, which is what a program means. */
#define HERO_TLS_CHECK_ALL 1
/* FOR TESTS ONLY: the peer's certificate is not checked at all, neither
 * its authority, nor its dates, nor its host, so anybody between the two
 * ends reads and changes every byte. A program that is not a test never
 * passes it. */
#define HERO_TLS_CHECK_NOTHING_FOR_TESTS 2

/* What a door taking a time in milliseconds takes for NO LIMIT: the
 * handshake of `hero_tls_connect`, `hero_tls_connect_after` and
 * `hero_tls_accept` given all the time it takes, the deadline of
 * `hero_tls_set_deadline` and `hero_tls_set_accept_deadline` taken away. 0
 * is then a time already passed, so a call that would wait answers at once,
 * and a time below -1 is `HERO_TLS_INVALID`: `hero_net.h`'s
 * `HERO_NET_FOREVER`, the same number. Since 2026-11-20 (M-rebel-library,
 * step 83a): until then 0 was no limit and -1 was refused, so no time said
 * "do not wait". */
#define HERO_TLS_FOREVER (-1)

/* The longest name of an application protocol, which ALPN allows, and so
 * the buffer `hero_tls_protocol` needs at most, with its zero byte. */
#define HERO_TLS_PROTOCOL_MOST UINT64_C(256)

/* 1 where this machine has the platform's TLS library, 0 where it has not
 * (a Linux machine with no `libssl.so.3`). Never fails. */
int64_t hero_tls_available(void);

/* A TLS connection to `host` at `port`, made and its handshake ended within
 * `ms` milliseconds, `HERO_TLS_FOREVER` for no limit; 0 waits not at all,
 * and a handshake always waits for the peer, so it answers
 * `HERO_TLS_TIMED_OUT` at once. `host` is a name or an address: the
 * name is looked up, sent to the peer (SNI, not for an address) and checked
 * against the peer's certificate. `roots`, `roots_count` bytes of PEM, are
 * the only authorities trusted where `roots_count` is not 0; with 0 the
 * system's own store is. `protocols` is the ALPN list offered. `checks` is
 * one of `HERO_TLS_CHECK_*`.
 *
 * Fails `HERO_TLS_NOT_FOUND`, `HERO_TLS_REFUSED`, `HERO_TLS_UNREACHABLE` and
 * `HERO_TLS_TIMED_OUT` as a connection does, and the kinds of a certificate
 * refused and `HERO_TLS_HANDSHAKE` as TLS does. The stream's reads and
 * writes then wait as long as the peer takes. */
hero_stream *hero_tls_connect(const char *host, int64_t port, int64_t ms, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks);

/* A socket listening for TLS connections on `address` (`127.0.0.1`, `::1`,
 * or "" for every address of the machine) at `port`, 0 asking the system
 * for a free one, which `hero_tls_port` reads back. Every connection
 * accepted presents `chain`, the PEM of the certificate first and of the
 * authorities that signed it after, and proves it holds `key`, the PEM of
 * its private key (PKCS #8, or RSA's or EC's own form), RSA or ECDSA on
 * P-256 or P-384. `protocols` is the ALPN list it takes, the first of it the
 * client also offers chosen. */
hero_tls_listener *hero_tls_listen(const char *address, int64_t port, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols);

/* `hero_tls_connect`, with the exchange of `steps`, `steps_count` bytes in
 * the form above, run on the connection before TLS begins; `ms` bounds the
 * connection, the exchange and the handshake together, as
 * `hero_tls_connect`'s does, `HERO_TLS_FOREVER` none. Fails as
 * `hero_tls_connect` does, `HERO_TLS_INVALID` where the steps are not in
 * that form or past a bound, and `HERO_TLS_PREAMBLE` where a step is not
 * met; `HERO_TLS_CLOSED_PEER` where the peer closes during the exchange. */
hero_stream *hero_tls_connect_after(const char *host, int64_t port, int64_t ms, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks, const void *steps, uint64_t steps_count);

/* `hero_tls_listen`, every connection it accepts running the exchange of
 * `steps` from the server's side before TLS begins, within `hero_tls_accept`'s
 * time. A connection whose exchange fails is closed and its failure answered,
 * as one whose handshake fails is. */
hero_tls_listener *hero_tls_listen_after(const char *address, int64_t port, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols, const void *steps, uint64_t steps_count);

/* The port the listener listens on, or -1. */
int64_t hero_tls_port(hero_tls_listener *l);

/* The next connection the listener holds, waiting for one, as a stream once
 * its handshake has ended, which must end within `ms` milliseconds of the
 * connection, `HERO_TLS_FOREVER` for no limit; with 0 the connection taken
 * fails its handshake `HERO_TLS_TIMED_OUT` at once, as any whose time
 * passes. The wait for a connection is `hero_tls_set_accept_deadline`'s. A
 * connection whose handshake fails is closed and its failure answered: the
 * caller accepts again. A close of the
 * listener on another thread ends the wait, `HERO_TLS_NOT_OPEN`. */
hero_stream *hero_tls_accept(hero_tls_listener *l, int64_t ms);

/* Closes the listener and gives its handle back, whatever is answered: 0,
 * or -1 where it was not open. Connections accepted from it stay open. */
int64_t hero_tls_close_listener(hero_tls_listener *l);

/* TLS as the client over `inner`, a stream already connected to `host`:
 * what Postgres and a proxy's tunnel need, where a protocol of their own
 * comes before TLS. `host`, `roots`, `protocols` and `checks` are
 * `hero_tls_connect`'s.
 *
 * FROM THIS CALL THE INNER STREAM'S LIFE IS THE TLS STREAM'S, where one is
 * answered: its close closes `inner`, once. A group says so with
 * `transfers hero_stream_close`. NULL, and `inner` stays the caller's, open,
 * where the handshake was not begun (an argument refused, no TLS library);
 * NULL, and `inner` closed, where it was begun and failed, since a stream
 * half through a handshake is good for nothing. On macOS always
 * `HERO_TLS_UNSUPPORTED` (above), `inner` untouched. */
hero_stream *hero_tls_client(hero_stream *inner, const char *host, const void *roots, uint64_t roots_count, const char *protocols, int64_t checks);

/* TLS as the server over `inner`, a stream already connected to a client,
 * presenting `chain` and `key` as `hero_tls_listen`'s connections do. The
 * inner stream's life is handed as `hero_tls_client` hands it; on macOS
 * always `HERO_TLS_UNSUPPORTED`. */
hero_stream *hero_tls_server(hero_stream *inner, const void *chain, uint64_t chain_count, const void *key, uint64_t key_count, const char *protocols);

/* The version of TLS the stream speaks, 12 for 1.2 and 13 for 1.3, or -1
 * with `HERO_TLS_NOT_OPEN` where `s` is no open TLS stream. */
int64_t hero_tls_version(hero_stream *s);

/* The application protocol the two ends chose (ALPN) written into `buffer`
 * with a zero byte after it, and its length without that byte: 0 where none
 * was chosen. -1 with `HERO_TLS_NOT_OPEN` where `s` is no open TLS stream,
 * and with `HERO_TLS_INVALID` where `count` is below
 * `HERO_TLS_PROTOCOL_MOST`. */
int64_t hero_tls_protocol(hero_stream *s, void *buffer, uint64_t count);

/* A deadline on `s`, a TLS stream of this layer, as `hero_net_set_deadline`
 * sets one on a socket: from this call every read or write of `s` that
 * waits ends by `ms` milliseconds from now, answering -1 with
 * `HERO_STREAM_TIMED_OUT`, a read already waiting included; 0 is a deadline
 * already passed, so every read or write that would wait ends at once, and
 * `HERO_TLS_FOREVER` takes it away. A read the deadline ended loses nothing:
 * the next read goes on where
 * it stopped. A write it ended may have sent part of a record, and every
 * later write then fails `HERO_STREAM_OTHER`: the stream is good for nothing
 * but reading what is left and closing, as Go rules of its own TLS. Answers
 * 0, or -1: `HERO_TLS_INVALID` where `ms` is below -1, `HERO_TLS_NOT_OPEN`
 * where `s` is no open TLS stream, `HERO_TLS_UNSUPPORTED` where it runs over
 * a stream the program handed that is no socket. On macOS a wait looks at
 * the deadline every 50 ms, so a read already waiting takes a new one
 * within about 50 ms. */
int64_t hero_tls_set_deadline(hero_stream *s, int64_t ms);

/* A deadline on the listener's wait for a client: every `hero_tls_accept`
 * waiting on `l`, and every later one, ends by `ms` milliseconds from now,
 * NULL with `HERO_TLS_TIMED_OUT`; 0 is a deadline already passed, so an
 * accept with no connection waiting ends at once, and `HERO_TLS_FOREVER`
 * takes it away. The handshake's own time stays `hero_tls_accept`'s. Answers
 * 0, or -1: `HERO_TLS_INVALID` where `ms` is below -1, `HERO_TLS_NOT_OPEN`
 * where `l` is no open listener. */
int64_t hero_tls_set_accept_deadline(hero_tls_listener *l, int64_t ms);

/* -- A TLS STREAM'S TWO ADDRESSES (M-rebel-library, step 83b, 2026-11-20) --
 *
 * The address the stream's own end stands on, `hero_tls_local`, and the
 * peer's, `hero_tls_remote`, told as `hero_net.h`'s `hero_net_local` and
 * `hero_net_remote` tell a socket's: the text into `address`, a number
 * (`127.0.0.1`, `::1`, with the scope the system writes after a `%` where
 * the address has one) ended by a zero byte the answer does not count, the
 * port into `*port`, and the answer the text's bytes; or -1, nothing
 * written. `address_count` is at least `HERO_TLS_ADDRESS_MOST`, the same
 * number as `HERO_NET_ADDRESS_MOST`, else `HERO_TLS_INVALID` before anything
 * else, as for a NULL `address` or `port`; `HERO_TLS_NOT_OPEN` where `s` is
 * no open TLS stream. Neither waits, so neither is ended by the stop.
 *
 * On Windows and Linux they are asked of the socket under the stream at the
 * call: the one `hero_tls_connect` or `hero_tls_accept` made, or the one
 * under a `hero_stream_net.h` stream handed to `hero_tls_client` or
 * `hero_tls_server`, whose failure is answered as this layer's kind; a
 * stream handed there that stands on no socket (a pipe, a file, another
 * member's stream) fails `HERO_TLS_UNSUPPORTED`. On macOS they are the
 * addresses of the path Network.framework's connection took, read once when
 * its handshake ended, and an end it told no address for fails
 * `HERO_TLS_OTHER`. */
#define HERO_TLS_ADDRESS_MOST UINT64_C(64)

int64_t hero_tls_local(hero_stream *s, void *address, uint64_t address_count, int64_t *port);
int64_t hero_tls_remote(hero_stream *s, void *address, uint64_t address_count, int64_t *port);

/* The kind of the last call's failure on this thread, `HERO_TLS_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused). */
int64_t hero_tls_last_error(void);
int64_t hero_tls_last_os_error(void);

/* The last failure on this thread told in words, the system's own where it
 * gave some (OpenSSL's reason, the Security framework's description, the
 * Windows message of its number), written into `buffer` with a zero byte
 * after it, cut at a character's end where it does not fit; answers the
 * length written without that byte, 0 where there is none, -1 with
 * `HERO_TLS_INVALID` where `count` is 0. */
int64_t hero_tls_last_message(void *buffer, uint64_t count);

#endif
