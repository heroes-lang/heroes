/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_odbc.c — the member behind `hero_odbc.h`: the system's ODBC
 * driver manager, loaded at the first call that needs it (M-rebel-library).
 *
 * WHY LOADED AND NEVER LINKED, as `hero_tls.c` says it for `libssl`: the
 * layer's archive is compiled for every program a machine builds, so a member
 * that included `<sql.h>` would stop every build on a machine without
 * unixODBC's headers, and one linked `-lodbc` every program on one without
 * the library. The member declares the types, numbers and functions of ODBC
 * 3 it calls below from the ODBC reference (Microsoft's "ODBC API Reference"
 * and its appendix of `sql.h` and `sqlext.h` values), finds each function
 * once by name, and never closes the library, so no pointer outlives it. One
 * function missing is the manager missing: every call then answers
 * `HERO_ODBC_UNSUPPORTED`, the message naming it.
 *
 *   - Windows: `odbc32.dll`, from the system's folder alone
 *     (`LOAD_LIBRARY_SEARCH_SYSTEM32`, so no file planted beside the program
 *     is loaded in its place), found once under `InitOnceExecuteOnce`. Every
 *     call that takes or gives text is the wide one, `SQLDriverConnectW` and
 *     so on, its text UTF-16, converted from and to UTF-8 by
 *     `MultiByteToWideChar` and `WideCharToMultiByte`, which refuse a text
 *     that is not UTF-8 or UTF-16.
 *   - Linux and macOS: unixODBC's `libodbc`, else iODBC's `libiodbc`, by
 *     `dlopen` under `pthread_once`. Every call is the narrow one and text
 *     crosses as UTF-8 bytes, which both managers hand to a driver as they
 *     are: the wide calls would need the width of a wide character, two bytes
 *     in unixODBC and four in iODBC.
 *
 * ONE ENVIRONMENT FOR THE PROCESS, made with the library and kept, set to
 * ODBC 3's behaviour (`SQL_ATTR_ODBC_VERSION`), from which every connection
 * is made.
 *
 * ONE TABLE OF SLOTS FOR BOTH HANDLES, under one lock and one tag
 * (`hero_handle_tag.h`'s `HERO_HANDLE_TAG_ODBC`): a slot is a connection or a
 * statement, and a handle names its slot, its life and the tag, so one
 * closed, NULL, or never issued is `HERO_ODBC_NOT_OPEN`, and one of the other
 * kind is too. The table is fixed, `HERO_ODBC_SLOTS` handles, its static
 * bytes touched only as handles are made; a program past it is told
 * `HERO_ODBC_LIMIT`.
 *
 * A CALL IN FLIGHT KEEPS ITS HANDLE. A call marks its slot busy, and a call
 * on a statement marks its connection busy too; a close meanwhile marks the
 * slot closing, and the last call to leave gives the driver's handle back.
 * So a connection is disconnected only when no call on it or on any of its
 * statements may still pass its handle to the driver. Its statements are
 * freed before it, and each answers `HERO_ODBC_NOT_OPEN` from its close on.
 * Two calls on ONE statement at once would share its parameters and its
 * cell, so the second is refused `HERO_ODBC_INVALID` and never waits.
 *
 * A STATEMENT'S OWN MEMORY (`hero_layer_alloc.h`): its parameters' values,
 * copied when bound and handed to the driver by `SQLBindParameter` just
 * before each run, then unbound from the driver at once, so the driver never
 * keeps an address of this memory past a call; and its cell, the last column
 * read, grown as a value needs.
 *
 * NO CONDITIONAL BUT THE PLATFORM'S ARMS, and the arms differ only in how the
 * library is loaded, how a lock is taken and how text is converted.
 */

#include "hero_odbc.h"

#include "hero_handle_tag.h"
#include "hero_layer_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#include <pthread.h>
#endif

_Static_assert(sizeof(void *) == 8 && sizeof(uintptr_t) == 8, "a handle carries its slot and its life in 64 bits");
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
/* An attribute is read into eight zeroed bytes, which a driver writing four
 * or eight fills from the first: the same number on a little-endian machine
 * alone. */
_Static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "an attribute's four bytes are the low half of eight");
#endif

/* -- ODBC'S TYPES AND NUMBERS, FROM ITS REFERENCE ----------------------------
 *
 * Widths, not names: `SQLSMALLINT` is 16 bits, `SQLINTEGER` 32 on every
 * 64-bit manager (Windows' `long`, unixODBC's and iODBC's `int`), `SQLLEN`
 * and `SQLULEN` 64, a handle a pointer. The numbers below are the reference's;
 * the probe of this member checks each against `sql.h` and `sqlext.h` where a
 * machine has them (MinGW-w64's and unixODBC's). */

typedef int16_t hero_sql_return;
typedef int16_t hero_sql_small;
typedef uint16_t hero_sql_usmall;
typedef int32_t hero_sql_int;
typedef uint32_t hero_sql_uint;
typedef int64_t hero_sql_len;
typedef uint64_t hero_sql_ulen;
typedef void *hero_sql_handle;

/* The unit of a text the driver manager reads and writes: a UTF-16 code unit
 * on Windows (`SQLWCHAR`), a byte elsewhere (`SQLCHAR`). */
#if defined(_WIN32)
typedef uint16_t hero_odbc_unit;
#else
typedef unsigned char hero_odbc_unit;
#endif

typedef struct hero_sql_timestamp {
    hero_sql_small year;
    hero_sql_usmall month;
    hero_sql_usmall day;
    hero_sql_usmall hour;
    hero_sql_usmall minute;
    hero_sql_usmall second;
    hero_sql_uint fraction;
} hero_sql_timestamp;

_Static_assert(sizeof(hero_sql_timestamp) == 16, "TIMESTAMP_STRUCT is sixteen bytes");

#define HERO_SQL_HANDLE_ENV 1
#define HERO_SQL_HANDLE_DBC 2
#define HERO_SQL_HANDLE_STMT 3
#define HERO_SQL_SUCCESS 0
#define HERO_SQL_SUCCESS_WITH_INFO 1
#define HERO_SQL_NO_DATA 100
#define HERO_SQL_ERROR (-1)
#define HERO_SQL_INVALID_HANDLE (-2)
#define HERO_SQL_NTS (-3)
#define HERO_SQL_NULL_DATA (-1)
#define HERO_SQL_NO_TOTAL (-4)
#define HERO_SQL_IS_UINTEGER (-5)
#define HERO_SQL_ATTR_ODBC_VERSION 200
#define HERO_SQL_OV_ODBC3 3
#define HERO_SQL_ATTR_ACCESS_MODE 101
#define HERO_SQL_ATTR_AUTOCOMMIT 102
#define HERO_SQL_ATTR_LOGIN_TIMEOUT 103
#define HERO_SQL_ATTR_TXN_ISOLATION 108
#define HERO_SQL_ATTR_CONNECTION_TIMEOUT 113
#define HERO_SQL_ATTR_CONNECTION_DEAD 1209
#define HERO_SQL_ATTR_QUERY_TIMEOUT 0
#define HERO_SQL_DRIVER_NOPROMPT 0
#define HERO_SQL_COMMIT 0
#define HERO_SQL_ROLLBACK 1
#define HERO_SQL_CLOSE 0
#define HERO_SQL_RESET_PARAMS 3
#define HERO_SQL_PARAM_INPUT 1
#define HERO_SQL_FETCH_NEXT 1
#define HERO_SQL_FETCH_FIRST 2
#define HERO_SQL_FETCH_FIRST_USER 31
#define HERO_SQL_FETCH_FIRST_SYSTEM 32
#define HERO_SQL_DESC_TYPE_NAME 14
#define HERO_SQL_ALL_TYPES 0
#define HERO_SQL_BEST_ROWID 1
#define HERO_SQL_ROWVER 2
#define HERO_SQL_SCOPE_CURROW 0
#define HERO_SQL_NULLABLE 1
#define HERO_SQL_INDEX_UNIQUE 0
#define HERO_SQL_INDEX_ALL 1
#define HERO_SQL_QUICK 0

/* The C types a value crosses as. */
#define HERO_SQL_C_CHAR 1
#define HERO_SQL_C_WCHAR (-8)
#define HERO_SQL_C_DOUBLE 8
#define HERO_SQL_C_BIT (-7)
#define HERO_SQL_C_BINARY (-2)
#define HERO_SQL_C_SBIGINT (-25)
#define HERO_SQL_C_TYPE_TIMESTAMP 93

/* The SQL types a parameter is described as. */
#define HERO_SQL_VARCHAR 12
#define HERO_SQL_LONGVARCHAR (-1)
#define HERO_SQL_WVARCHAR (-9)
#define HERO_SQL_WLONGVARCHAR (-10)
#define HERO_SQL_VARBINARY (-3)
#define HERO_SQL_LONGVARBINARY (-4)
#define HERO_SQL_BIGINT (-5)
#define HERO_SQL_BIT (-7)
#define HERO_SQL_DOUBLE 8
#define HERO_SQL_TYPE_TIMESTAMP 93

/* Past these a parameter's text or bytes is described as a long one, the
 * widths SQL Server's `varchar` and `varbinary` stop at, which every driver
 * reads as a long value of the same kind. */
#define HERO_ODBC_SHORT_TEXT_MOST 4000
#define HERO_ODBC_SHORT_BYTES_MOST 8000

/* -- THE LAST FAILURE, PER THREAD --------------------------------------------- */

/* The words kept of a failure, with their zero byte. */
#define HERO_ODBC_WORDS_MOST 4096

static _Thread_local int64_t hero_odbc_kind_said;
static _Thread_local int64_t hero_odbc_native_said;
static _Thread_local char hero_odbc_state_said[6];
static _Thread_local char hero_odbc_words[HERO_ODBC_WORDS_MOST];

/* `length` bytes of `text` copied into `into` after its first `at`, `most`
 * bytes with the zero, cut where a character ends; answers the new length. */
static size_t hero_odbc_append(char *into, size_t most, size_t at, const char *text, size_t length) {
    if (most == 0 || at >= most - 1) {
        return at;
    }
    if (length > most - 1 - at) {
        length = most - 1 - at;
        /* `text[length]` is the first byte left out: while it continues a
         * character, the character it belongs to is left out whole. */
        while (length > 0 && ((unsigned char)text[length] & 0xC0u) == 0x80u) {
            length -= 1;
        }
    }
    if (length > 0) {
        memcpy(into + at, text, length);
    }
    into[at + length] = '\0';
    return at + length;
}

/* The failure this call answers: its kind, and the layer's own words. */
static void hero_odbc_said(int64_t kind, const char *words) {
    hero_odbc_kind_said = kind;
    hero_odbc_native_said = 0;
    hero_odbc_state_said[0] = '\0';
    hero_odbc_words[0] = '\0';
    if (words != NULL) {
        (void)hero_odbc_append(hero_odbc_words, sizeof hero_odbc_words, 0, words, strlen(words));
    }
}

static void hero_odbc_fine(void) {
    hero_odbc_said(HERO_ODBC_OK, NULL);
}

/* `text` into `buffer` by the fill form: the whole length, the bytes copied
 * only where they fit. */
static int64_t hero_odbc_fill(void *buffer, uint64_t count, const void *text, size_t length) {
    if (length > 0 && count >= (uint64_t)length && buffer != NULL) {
        memcpy(buffer, text, length);
    }
    return (int64_t)length;
}

int64_t hero_odbc_last_error(void) {
    return hero_odbc_kind_said;
}

int64_t hero_odbc_last_os_error(void) {
    return hero_odbc_native_said;
}

int64_t hero_odbc_last_state(void *buffer, uint64_t count) {
    return hero_odbc_fill(buffer, count, hero_odbc_state_said, strlen(hero_odbc_state_said));
}

int64_t hero_odbc_last_message(void *buffer, uint64_t count) {
    return hero_odbc_fill(buffer, count, hero_odbc_words, strlen(hero_odbc_words));
}

/* -- A GROWING RUN OF BYTES ----------------------------------------------------- */

typedef struct hero_odbc_bytes {
    unsigned char *at;
    size_t count;
    size_t room;
} hero_odbc_bytes;

/* Room for `more` bytes past the count; false with `HERO_ODBC_LIMIT`. */
static bool hero_odbc_room(hero_odbc_bytes *b, size_t more) {
    if (more > (size_t)HERO_ODBC_TEXT_MOST * 4 || b->count > (size_t)HERO_ODBC_TEXT_MOST * 4) {
        hero_odbc_said(HERO_ODBC_LIMIT, "a value passed the most this layer holds, 4 GiB");
        return false;
    }
    size_t needed = b->count + more;

    if (needed <= b->room) {
        return true;
    }
    size_t room = b->room < 256 ? 256 : b->room;

    while (room < needed) {
        room *= 2;
    }
    unsigned char *grown = hero_layer_grow(b->at, room);

    if (grown == NULL) {
        hero_odbc_said(HERO_ODBC_LIMIT, "no memory left for a value");
        return false;
    }
    b->at = grown;
    b->room = room;
    return true;
}

static bool hero_odbc_put(hero_odbc_bytes *b, const void *bytes, size_t count) {
    if (!hero_odbc_room(b, count)) {
        return false;
    }
    if (count > 0) {
        memcpy(b->at + b->count, bytes, count);
    }
    b->count += count;
    return true;
}

static void hero_odbc_bytes_free(hero_odbc_bytes *b) {
    hero_layer_free(b->at);
    b->at = NULL;
    b->count = 0;
    b->room = 0;
}

/* -- TEXT BETWEEN UTF-8 AND THE MANAGER'S UNITS ------------------------------- */

/* A text in the manager's units, with a zero unit after it, in the layer's
 * memory; `units` counts them without the zero. */
typedef struct hero_odbc_native {
    hero_odbc_unit *at;
    size_t units;
} hero_odbc_native;

static void hero_odbc_native_free(hero_odbc_native *n) {
    hero_layer_free(n->at);
    n->at = NULL;
    n->units = 0;
}

/* `count` bytes of UTF-8 at `text` in the manager's units; false with
 * `HERO_ODBC_INVALID` where it is not UTF-8 (Windows), past
 * `HERO_ODBC_TEXT_MOST`, or `HERO_ODBC_LIMIT`. */
static bool hero_odbc_to_native(const char *text, size_t count, hero_odbc_native *out, const char *what) {
    out->at = NULL;
    out->units = 0;
    if (count > HERO_ODBC_TEXT_MOST) {
        hero_odbc_said(HERO_ODBC_INVALID, what);
        return false;
    }
#if defined(_WIN32)
    int units = 0;

    if (count > 0) {
        units = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, (int)count, NULL, 0);
        if (units <= 0) {
            hero_odbc_said(HERO_ODBC_INVALID, "a text handed to ODBC is not UTF-8");
            return false;
        }
    }
    out->at = hero_layer_alloc(((size_t)units + 1) * sizeof(hero_odbc_unit));
    if (out->at == NULL) {
        hero_odbc_said(HERO_ODBC_LIMIT, "no memory left for a text");
        return false;
    }
    if (units > 0 && MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text, (int)count, (LPWSTR)out->at, units) != units) {
        hero_odbc_native_free(out);
        hero_odbc_said(HERO_ODBC_INVALID, "a text handed to ODBC is not UTF-8");
        return false;
    }
    out->at[units] = 0;
    out->units = (size_t)units;
#else
    out->at = hero_layer_alloc(count + 1);
    if (out->at == NULL) {
        hero_odbc_said(HERO_ODBC_LIMIT, "no memory left for a text");
        return false;
    }
    if (count > 0) {
        memcpy(out->at, text, count);
    }
    out->at[count] = 0;
    out->units = count;
#endif
    return true;
}

/* A zero-ended C text in the manager's units; NULL stays NULL. */
static bool hero_odbc_name_to_native(const char *text, hero_odbc_native *out) {
    if (text == NULL) {
        out->at = NULL;
        out->units = 0;
        return true;
    }
    return hero_odbc_to_native(text, strlen(text), out, "a name handed to ODBC passed the most bytes the layer takes");
}

/* `units` units of the manager's at `text` appended to `into` as UTF-8;
 * false with `HERO_ODBC_INVALID` where it is not UTF-16 (Windows), or
 * `HERO_ODBC_LIMIT`. */
static bool hero_odbc_from_native(const hero_odbc_unit *text, size_t units, hero_odbc_bytes *into) {
#if defined(_WIN32)
    if (units == 0) {
        return true;
    }
    if (units > (size_t)HERO_ODBC_TEXT_MOST) {
        hero_odbc_said(HERO_ODBC_LIMIT, "a text from ODBC passed the most this layer holds");
        return false;
    }
    int bytes = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, (LPCWCH)text, (int)units, NULL, 0, NULL, NULL);

    if (bytes <= 0) {
        hero_odbc_said(HERO_ODBC_INVALID, "a text from ODBC is not UTF-16: it holds a half of a surrogate pair alone");
        return false;
    }
    if (!hero_odbc_room(into, (size_t)bytes)) {
        return false;
    }
    if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, (LPCWCH)text, (int)units, (LPSTR)(into->at + into->count), bytes, NULL, NULL) != bytes) {
        hero_odbc_said(HERO_ODBC_INVALID, "a text from ODBC is not UTF-16");
        return false;
    }
    into->count += (size_t)bytes;
    return true;
#else
    return hero_odbc_put(into, text, units);
#endif
}

/* The units of a zero-ended text of the manager's, at most `most`. */
static size_t hero_odbc_units(const hero_odbc_unit *text, size_t most) {
    size_t n = 0;

    while (n < most && text[n] != 0) {
        n += 1;
    }
    return n;
}

/* -- THE DRIVER MANAGER, LOADED ONCE -------------------------------------------- */

/* The functions this member calls, found once. A text's unit is the
 * manager's: the wide call on Windows, the narrow one elsewhere. */
typedef struct hero_odbc_calls {
    hero_sql_return (*AllocHandle)(hero_sql_small type, hero_sql_handle input, hero_sql_handle *output);
    hero_sql_return (*FreeHandle)(hero_sql_small type, hero_sql_handle handle);
    hero_sql_return (*SetEnvAttr)(hero_sql_handle env, hero_sql_int attribute, void *value, hero_sql_int length);
    hero_sql_return (*SetConnectAttr)(hero_sql_handle dbc, hero_sql_int attribute, void *value, hero_sql_int length);
    hero_sql_return (*GetConnectAttr)(hero_sql_handle dbc, hero_sql_int attribute, void *value, hero_sql_int length, hero_sql_int *written);
    hero_sql_return (*SetStmtAttr)(hero_sql_handle stmt, hero_sql_int attribute, void *value, hero_sql_int length);
    hero_sql_return (*DriverConnect)(hero_sql_handle dbc, void *window, hero_odbc_unit *in, hero_sql_small in_length, hero_odbc_unit *out, hero_sql_small out_room, hero_sql_small *out_length, hero_sql_usmall completion);
    hero_sql_return (*Connect)(hero_sql_handle dbc, hero_odbc_unit *dsn, hero_sql_small dsn_length, hero_odbc_unit *user, hero_sql_small user_length, hero_odbc_unit *password, hero_sql_small password_length);
    hero_sql_return (*Disconnect)(hero_sql_handle dbc);
    hero_sql_return (*GetInfo)(hero_sql_handle dbc, hero_sql_usmall item, void *value, hero_sql_small room, hero_sql_small *length);
    hero_sql_return (*EndTran)(hero_sql_small type, hero_sql_handle handle, hero_sql_small completion);
    hero_sql_return (*Prepare)(hero_sql_handle stmt, hero_odbc_unit *text, hero_sql_int length);
    hero_sql_return (*NumParams)(hero_sql_handle stmt, hero_sql_small *count);
    hero_sql_return (*BindParameter)(hero_sql_handle stmt, hero_sql_usmall number, hero_sql_small direction, hero_sql_small c_type, hero_sql_small sql_type, hero_sql_ulen size, hero_sql_small digits, void *value, hero_sql_len room, hero_sql_len *indicator);
    hero_sql_return (*Execute)(hero_sql_handle stmt);
    hero_sql_return (*ExecDirect)(hero_sql_handle stmt, hero_odbc_unit *text, hero_sql_int length);
    hero_sql_return (*NumResultCols)(hero_sql_handle stmt, hero_sql_small *count);
    hero_sql_return (*DescribeCol)(hero_sql_handle stmt, hero_sql_usmall column, hero_odbc_unit *name, hero_sql_small room, hero_sql_small *name_length, hero_sql_small *type, hero_sql_ulen *size, hero_sql_small *digits, hero_sql_small *nullable);
    hero_sql_return (*ColAttribute)(hero_sql_handle stmt, hero_sql_usmall column, hero_sql_usmall field, void *text, hero_sql_small room, hero_sql_small *length, hero_sql_len *number);
    hero_sql_return (*RowCount)(hero_sql_handle stmt, hero_sql_len *rows);
    hero_sql_return (*Fetch)(hero_sql_handle stmt);
    hero_sql_return (*MoreResults)(hero_sql_handle stmt);
    hero_sql_return (*FreeStmt)(hero_sql_handle stmt, hero_sql_usmall option);
    hero_sql_return (*GetData)(hero_sql_handle stmt, hero_sql_usmall column, hero_sql_small c_type, void *value, hero_sql_len room, hero_sql_len *indicator);
    hero_sql_return (*GetDiagRec)(hero_sql_small type, hero_sql_handle handle, hero_sql_small record, hero_odbc_unit *state, hero_sql_int *native, hero_odbc_unit *text, hero_sql_small room, hero_sql_small *length);
    hero_sql_return (*Drivers)(hero_sql_handle env, hero_sql_usmall direction, hero_odbc_unit *name, hero_sql_small name_room, hero_sql_small *name_length, hero_odbc_unit *attributes, hero_sql_small attributes_room, hero_sql_small *attributes_length);
    hero_sql_return (*DataSources)(hero_sql_handle env, hero_sql_usmall direction, hero_odbc_unit *name, hero_sql_small name_room, hero_sql_small *name_length, hero_odbc_unit *description, hero_sql_small description_room, hero_sql_small *description_length);
    hero_sql_return (*Tables)(hero_sql_handle stmt, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length, hero_odbc_unit *d, hero_sql_small d_length);
    hero_sql_return (*Columns)(hero_sql_handle stmt, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length, hero_odbc_unit *d, hero_sql_small d_length);
    hero_sql_return (*PrimaryKeys)(hero_sql_handle stmt, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length);
    hero_sql_return (*ForeignKeys)(hero_sql_handle stmt, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length, hero_odbc_unit *d, hero_sql_small d_length, hero_odbc_unit *e, hero_sql_small e_length, hero_odbc_unit *f, hero_sql_small f_length);
    hero_sql_return (*Statistics)(hero_sql_handle stmt, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length, hero_sql_usmall unique, hero_sql_usmall reserved);
    hero_sql_return (*Procedures)(hero_sql_handle stmt, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length);
    hero_sql_return (*ProcedureColumns)(hero_sql_handle stmt, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length, hero_odbc_unit *d, hero_sql_small d_length);
    hero_sql_return (*GetTypeInfo)(hero_sql_handle stmt, hero_sql_small type);
    hero_sql_return (*SpecialColumns)(hero_sql_handle stmt, hero_sql_usmall identifier, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length, hero_sql_usmall scope, hero_sql_usmall nullable);
    hero_sql_return (*TablePrivileges)(hero_sql_handle stmt, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length);
    hero_sql_return (*ColumnPrivileges)(hero_sql_handle stmt, hero_odbc_unit *a, hero_sql_small a_length, hero_odbc_unit *b, hero_sql_small b_length, hero_odbc_unit *c, hero_sql_small c_length, hero_odbc_unit *d, hero_sql_small d_length);
} hero_odbc_calls;

/* The functions and the environment, found and made once under
 * `hero_odbc_once`, and read only after it. */
static hero_odbc_calls hero_odbc_call;
static hero_sql_handle hero_odbc_env;
static bool hero_odbc_loaded;
/* What was loaded, or why nothing was: words kept for every later call. */
static char hero_odbc_loaded_name[256];
static char hero_odbc_load_failure[1024];

/* One more name of `found` appended to `hero_odbc_load_failure`. */
static void hero_odbc_load_note(const char *text) {
    size_t at = strlen(hero_odbc_load_failure);

    (void)hero_odbc_append(hero_odbc_load_failure, sizeof hero_odbc_load_failure, at, text, strlen(text));
}

/* The name a function has in the library: the wide one on Windows where it
 * takes or gives text. */
#if defined(_WIN32)
#define HERO_ODBC_TEXT_NAME(name) name "W"
#else
#define HERO_ODBC_TEXT_NAME(name) name
#endif

#if defined(_WIN32)

static INIT_ONCE hero_odbc_once = INIT_ONCE_STATIC_INIT;
static HMODULE hero_odbc_library;

/* One function by name into `*slot`; false where the library has none. A
 * function's address is copied by its bytes, `FARPROC` and the slot's type
 * both being function pointers of one size. */
static bool hero_odbc_find_function(const char *name, void *slot, size_t slot_size) {
    FARPROC found = GetProcAddress(hero_odbc_library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

static bool hero_odbc_open_library(void) {
    hero_odbc_library = LoadLibraryExW(L"odbc32.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hero_odbc_library == NULL) {
        hero_odbc_load_note("no ODBC driver manager could be loaded: odbc32.dll, from the system's folder, would not load");
        return false;
    }
    (void)hero_odbc_append(hero_odbc_loaded_name, sizeof hero_odbc_loaded_name, 0, "odbc32.dll", 10);
    return true;
}

#else

static pthread_once_t hero_odbc_once = PTHREAD_ONCE_INIT;
static void *hero_odbc_library;

/* One function by name into `*slot`; false where the library has none. C11
 * has no conversion between an object pointer and a function pointer, so
 * the address is copied by its bytes, which POSIX makes the same for both
 * (`dlsym`'s own page). */
static bool hero_odbc_find_function(const char *name, void *slot, size_t slot_size) {
    void *found = dlsym(hero_odbc_library, name);

    if (found == NULL || slot_size != sizeof found) {
        return false;
    }
    memcpy(slot, &found, sizeof found);
    return true;
}

static bool hero_odbc_open_library(void) {
#if defined(__APPLE__)
    static const char *const names[] = {
        "libodbc.2.dylib", "/opt/homebrew/lib/libodbc.2.dylib", "/usr/local/lib/libodbc.2.dylib",
        "libiodbc.2.dylib", "/opt/homebrew/lib/libiodbc.2.dylib", "/usr/local/lib/libiodbc.2.dylib",
    };
#else
    static const char *const names[] = { "libodbc.so.2", "libodbc.so.1", "libiodbc.so.2" };
#endif
    for (size_t i = 0; i < sizeof names / sizeof names[0]; i += 1) {
        hero_odbc_library = dlopen(names[i], RTLD_NOW | RTLD_LOCAL);
        if (hero_odbc_library != NULL) {
            (void)hero_odbc_append(hero_odbc_loaded_name, sizeof hero_odbc_loaded_name, 0, names[i], strlen(names[i]));
            return true;
        }
    }
    hero_odbc_load_note("no ODBC driver manager is installed: none of ");
    for (size_t i = 0; i < sizeof names / sizeof names[0]; i += 1) {
        hero_odbc_load_note(i == 0 ? "" : ", ");
        hero_odbc_load_note(names[i]);
    }
    hero_odbc_load_note(" would load (unixODBC's or iODBC's library)");
    return false;
}

#endif

#define HERO_ODBC_FIND(field, name)                                                               \
    do {                                                                                          \
        if (ok && !hero_odbc_find_function(name, &hero_odbc_call.field, sizeof hero_odbc_call.field)) { \
            hero_odbc_load_note("the ODBC driver manager ");                                      \
            hero_odbc_load_note(hero_odbc_loaded_name);                                           \
            hero_odbc_load_note(" has no function ");                                             \
            hero_odbc_load_note(name);                                                            \
            ok = false;                                                                           \
        }                                                                                         \
    } while (0)

static void hero_odbc_load_once(void) {
    bool ok = hero_odbc_open_library();

    HERO_ODBC_FIND(AllocHandle, "SQLAllocHandle");
    HERO_ODBC_FIND(FreeHandle, "SQLFreeHandle");
    HERO_ODBC_FIND(SetEnvAttr, "SQLSetEnvAttr");
    HERO_ODBC_FIND(SetConnectAttr, HERO_ODBC_TEXT_NAME("SQLSetConnectAttr"));
    HERO_ODBC_FIND(GetConnectAttr, HERO_ODBC_TEXT_NAME("SQLGetConnectAttr"));
    HERO_ODBC_FIND(SetStmtAttr, HERO_ODBC_TEXT_NAME("SQLSetStmtAttr"));
    HERO_ODBC_FIND(DriverConnect, HERO_ODBC_TEXT_NAME("SQLDriverConnect"));
    HERO_ODBC_FIND(Connect, HERO_ODBC_TEXT_NAME("SQLConnect"));
    HERO_ODBC_FIND(Disconnect, "SQLDisconnect");
    HERO_ODBC_FIND(GetInfo, HERO_ODBC_TEXT_NAME("SQLGetInfo"));
    HERO_ODBC_FIND(EndTran, "SQLEndTran");
    HERO_ODBC_FIND(Prepare, HERO_ODBC_TEXT_NAME("SQLPrepare"));
    HERO_ODBC_FIND(NumParams, "SQLNumParams");
    HERO_ODBC_FIND(BindParameter, "SQLBindParameter");
    HERO_ODBC_FIND(Execute, "SQLExecute");
    HERO_ODBC_FIND(ExecDirect, HERO_ODBC_TEXT_NAME("SQLExecDirect"));
    HERO_ODBC_FIND(NumResultCols, "SQLNumResultCols");
    HERO_ODBC_FIND(DescribeCol, HERO_ODBC_TEXT_NAME("SQLDescribeCol"));
    HERO_ODBC_FIND(ColAttribute, HERO_ODBC_TEXT_NAME("SQLColAttribute"));
    HERO_ODBC_FIND(RowCount, "SQLRowCount");
    HERO_ODBC_FIND(Fetch, "SQLFetch");
    HERO_ODBC_FIND(MoreResults, "SQLMoreResults");
    HERO_ODBC_FIND(FreeStmt, "SQLFreeStmt");
    HERO_ODBC_FIND(GetData, "SQLGetData");
    HERO_ODBC_FIND(GetDiagRec, HERO_ODBC_TEXT_NAME("SQLGetDiagRec"));
    HERO_ODBC_FIND(Drivers, HERO_ODBC_TEXT_NAME("SQLDrivers"));
    HERO_ODBC_FIND(DataSources, HERO_ODBC_TEXT_NAME("SQLDataSources"));
    HERO_ODBC_FIND(Tables, HERO_ODBC_TEXT_NAME("SQLTables"));
    HERO_ODBC_FIND(Columns, HERO_ODBC_TEXT_NAME("SQLColumns"));
    HERO_ODBC_FIND(PrimaryKeys, HERO_ODBC_TEXT_NAME("SQLPrimaryKeys"));
    HERO_ODBC_FIND(ForeignKeys, HERO_ODBC_TEXT_NAME("SQLForeignKeys"));
    HERO_ODBC_FIND(Statistics, HERO_ODBC_TEXT_NAME("SQLStatistics"));
    HERO_ODBC_FIND(Procedures, HERO_ODBC_TEXT_NAME("SQLProcedures"));
    HERO_ODBC_FIND(ProcedureColumns, HERO_ODBC_TEXT_NAME("SQLProcedureColumns"));
    HERO_ODBC_FIND(GetTypeInfo, HERO_ODBC_TEXT_NAME("SQLGetTypeInfo"));
    HERO_ODBC_FIND(SpecialColumns, HERO_ODBC_TEXT_NAME("SQLSpecialColumns"));
    HERO_ODBC_FIND(TablePrivileges, HERO_ODBC_TEXT_NAME("SQLTablePrivileges"));
    HERO_ODBC_FIND(ColumnPrivileges, HERO_ODBC_TEXT_NAME("SQLColumnPrivileges"));
    if (!ok) {
        return;
    }
    hero_sql_return rc = hero_odbc_call.AllocHandle(HERO_SQL_HANDLE_ENV, NULL, &hero_odbc_env);

    if (rc != HERO_SQL_SUCCESS && rc != HERO_SQL_SUCCESS_WITH_INFO) {
        hero_odbc_load_note("the ODBC driver manager ");
        hero_odbc_load_note(hero_odbc_loaded_name);
        hero_odbc_load_note(" would not make an environment");
        hero_odbc_env = NULL;
        return;
    }
    rc = hero_odbc_call.SetEnvAttr(hero_odbc_env, HERO_SQL_ATTR_ODBC_VERSION, (void *)(uintptr_t)HERO_SQL_OV_ODBC3, 0);
    if (rc != HERO_SQL_SUCCESS && rc != HERO_SQL_SUCCESS_WITH_INFO) {
        hero_odbc_load_note("the ODBC driver manager ");
        hero_odbc_load_note(hero_odbc_loaded_name);
        hero_odbc_load_note(" refused ODBC 3's behaviour");
        (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_ENV, hero_odbc_env);
        hero_odbc_env = NULL;
        return;
    }
    hero_odbc_loaded = true;
}

#if defined(_WIN32)
static BOOL CALLBACK hero_odbc_load_win(PINIT_ONCE once, PVOID parameter, PVOID *context) {
    (void)once;
    (void)parameter;
    (void)context;
    hero_odbc_load_once();
    return TRUE;
}
#endif

/* Whether the manager is loaded; where not, the call fails
 * `HERO_ODBC_UNSUPPORTED` with the words of the first try. */
static bool hero_odbc_ready(void) {
#if defined(_WIN32)
    (void)InitOnceExecuteOnce(&hero_odbc_once, hero_odbc_load_win, NULL, NULL);
#else
    (void)pthread_once(&hero_odbc_once, hero_odbc_load_once);
#endif
    if (!hero_odbc_loaded) {
        hero_odbc_said(HERO_ODBC_UNSUPPORTED, hero_odbc_load_failure);
        return false;
    }
    return true;
}

static bool hero_odbc_succeeded(hero_sql_return rc) {
    return rc == HERO_SQL_SUCCESS || rc == HERO_SQL_SUCCESS_WITH_INFO;
}

/* The failure the manager or the driver reported on `handle`: the kind
 * `HERO_ODBC_DRIVER`, the first record's SQLSTATE and native error, and the
 * words of every record, the first's alone then each other's after `; `
 * with its SQLSTATE in brackets. `what` names the call where no record says
 * anything; an invalid handle is the layer's own failure. */
/* `units` units of a diagnostic's words as UTF-8 into `into`, at most `most`
 * bytes, a unit that is no character replaced: words for a person, never a
 * failure of their own. Answers the bytes written. */
static size_t hero_odbc_words_of(const hero_odbc_unit *text, size_t units, char *into, size_t most) {
#if defined(_WIN32)
    if (units == 0 || units > INT32_MAX || most > INT32_MAX) {
        return 0;
    }
    int n = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)text, (int)units, into, (int)most, NULL, NULL);

    return n > 0 ? (size_t)n : 0;
#else
    size_t n = units < most ? units : most;

    if (n > 0) {
        memcpy(into, text, n);
    }
    return n;
#endif
}

static void hero_odbc_driver_failed(hero_sql_small type, hero_sql_handle handle, hero_sql_return rc, const char *what) {
    hero_odbc_unit state[6];
    hero_odbc_unit text[1024];
    char words[4 * 1024];
    size_t at = 0;
    bool any = false;

    hero_odbc_said(rc == HERO_SQL_INVALID_HANDLE ? HERO_ODBC_OTHER : HERO_ODBC_DRIVER, NULL);
    for (hero_sql_small record = 1; record <= 8 && handle != NULL; record += 1) {
        hero_sql_int native = 0;
        hero_sql_small length = 0;

        memset(state, 0, sizeof state);
        memset(text, 0, sizeof text);
        hero_sql_return got = hero_odbc_call.GetDiagRec(type, handle, record, state, &native, text, (hero_sql_small)(sizeof text / sizeof text[0]), &length);

        if (!hero_odbc_succeeded(got)) {
            break;
        }
        char state_text[6];

        for (size_t i = 0; i < 5; i += 1) {
            /* A SQLSTATE is five letters and digits of ASCII; anything else is
             * kept out of the code a program reads. */
            hero_odbc_unit u = state[i];

            state_text[i] = (u >= '0' && u <= '9') || (u >= 'A' && u <= 'Z') ? (char)u : '?';
        }
        state_text[5] = '\0';
        size_t units = length < 0 ? 0 : (size_t)length;

        if (units > sizeof text / sizeof text[0] - 1) {
            units = hero_odbc_units(text, sizeof text / sizeof text[0] - 1);
        }
        size_t said = hero_odbc_words_of(text, units, words, sizeof words);

        if (!any) {
            memcpy(hero_odbc_state_said, state_text, sizeof state_text);
            hero_odbc_native_said = native;
            at = hero_odbc_append(hero_odbc_words, sizeof hero_odbc_words, 0, words, said);
            any = true;
        } else {
            /* A warning (class 01) may stand before the failure it comes
             * with, as unixODBC's 01000 before a driver that would not load:
             * the state told is the first record's that is no warning. */
            if (hero_odbc_state_said[0] == '0' && hero_odbc_state_said[1] == '1' && !(state_text[0] == '0' && state_text[1] == '1')) {
                memcpy(hero_odbc_state_said, state_text, sizeof state_text);
                hero_odbc_native_said = native;
            }
            at = hero_odbc_append(hero_odbc_words, sizeof hero_odbc_words, at, "; [", 3);
            at = hero_odbc_append(hero_odbc_words, sizeof hero_odbc_words, at, state_text, 5);
            at = hero_odbc_append(hero_odbc_words, sizeof hero_odbc_words, at, "] ", 2);
            at = hero_odbc_append(hero_odbc_words, sizeof hero_odbc_words, at, words, said);
        }
    }
    if (!any) {
        const char *said = rc == HERO_SQL_INVALID_HANDLE ? " was handed a handle the driver manager does not know" : " failed and the driver said nothing";

        at = hero_odbc_append(hero_odbc_words, sizeof hero_odbc_words, 0, what, strlen(what));
        (void)hero_odbc_append(hero_odbc_words, sizeof hero_odbc_words, at, said, strlen(said));
    }
}

/* -- THE TABLE OF HANDLES --------------------------------------------------------- */

/* A handle carries this table's tag, a slot and a life in one pointer's 64
 * bits: `HERO_HANDLE_TAG(HERO_HANDLE_TAG_ODBC) | (life << HERO_ODBC_INDEX_BITS)
 * | (index + 1)`, 15 bits for an index plus one, so no handle is NULL and
 * none is another member's. */
#define HERO_ODBC_INDEX_BITS 15
#define HERO_ODBC_INDEX_MASK ((UINT64_C(1) << HERO_ODBC_INDEX_BITS) - 1)
#define HERO_ODBC_SLOTS 16384
#define HERO_ODBC_LIFE_MOST HERO_HANDLE_LIFE_MOST(HERO_ODBC_INDEX_BITS)
_Static_assert(HERO_ODBC_SLOTS <= HERO_ODBC_INDEX_MASK, "every slot's index plus one fits its bits");
_Static_assert((HERO_ODBC_LIFE_MOST << HERO_ODBC_INDEX_BITS) < (UINT64_C(1) << HERO_HANDLE_TAG_SHIFT), "a life stays below the tag");

#define HERO_ODBC_KIND_CONNECTION 1
#define HERO_ODBC_KIND_STATEMENT 2

/* One parameter's value, the layer's copy. */
typedef struct hero_odbc_param {
    bool set;
    hero_sql_small c_type;
    hero_sql_small sql_type;
    hero_sql_ulen size;
    hero_sql_small digits;
    hero_sql_len indicator;
    int64_t integer;
    double number;
    unsigned char bit;
    hero_sql_timestamp stamp;
    /* Text in the manager's units, or bytes; NULL for the other kinds. */
    void *bytes;
    size_t count;
} hero_odbc_param;

/* A statement's own memory: its parameters, and the cell last read. */
typedef struct hero_odbc_work {
    hero_odbc_param *params;
    size_t param_count;
    int64_t cell_kind;
    bool cell_null;
    int64_t cell_integer;
    double cell_float;
    hero_sql_timestamp cell_stamp;
    hero_odbc_bytes cell;
    hero_odbc_bytes wide;
} hero_odbc_work;

typedef struct hero_odbc_slot {
    /* The life the slot was last issued for, counted from 1. */
    uint64_t life;
    /* The next slot given back, its index plus one, while this one is. */
    uint32_t next;
    /* The calls in flight on it; a connection's count its statements' too. */
    uint32_t busy;
    /* A statement's connection, its index plus one. */
    uint32_t parent;
    uint8_t kind;
    bool held;
    bool closing;
    /* The driver manager's handle: a `SQLHDBC` or a `SQLHSTMT`. */
    hero_sql_handle handle;
    /* A statement's own memory; NULL for a connection. */
    hero_odbc_work *work;
} hero_odbc_slot;

static hero_odbc_slot hero_odbc_slots[HERO_ODBC_SLOTS];
/* The slots ever issued, from the first; and the first given back, its
 * index plus one, 0 for none. */
static uint32_t hero_odbc_fresh;
static uint32_t hero_odbc_given_back;

#if defined(_WIN32)
static SRWLOCK hero_odbc_lock = SRWLOCK_INIT;

static void hero_odbc_enter(void) {
    AcquireSRWLockExclusive(&hero_odbc_lock);
}

static void hero_odbc_leave(void) {
    ReleaseSRWLockExclusive(&hero_odbc_lock);
}
#else
static pthread_mutex_t hero_odbc_lock = PTHREAD_MUTEX_INITIALIZER;

static void hero_odbc_enter(void) {
    (void)pthread_mutex_lock(&hero_odbc_lock);
}

static void hero_odbc_leave(void) {
    (void)pthread_mutex_unlock(&hero_odbc_lock);
}
#endif

static uint64_t hero_odbc_value(uint32_t index, uint64_t life) {
    return HERO_HANDLE_TAG(HERO_HANDLE_TAG_ODBC) | (life << HERO_ODBC_INDEX_BITS) | ((uint64_t)index + 1);
}

/* The slot a handle's value names while it holds the life the value was
 * issued for, is of `kind`, and is not closing; NULL for any other value,
 * one of another table's tag before any slot is looked at. Under the lock. */
static hero_odbc_slot *hero_odbc_slot_of(uint64_t value, uint8_t kind) {
    uint64_t low = value & HERO_ODBC_INDEX_MASK;

    if ((value & HERO_HANDLE_TAG_MASK) != HERO_HANDLE_TAG(HERO_HANDLE_TAG_ODBC) || low == 0 || low > hero_odbc_fresh) {
        return NULL;
    }
    hero_odbc_slot *slot = &hero_odbc_slots[low - 1];

    if (!slot->held || slot->closing || slot->kind != kind || slot->life != (value & ~HERO_HANDLE_TAG_MASK) >> HERO_ODBC_INDEX_BITS) {
        return NULL;
    }
    return slot;
}

/* A new slot of `kind` for `handle`, its life begun; the handle's value, or
 * 0 where every slot is held. Under the lock. */
static uint64_t hero_odbc_issue(uint8_t kind, hero_sql_handle handle, uint32_t parent, hero_odbc_work *work) {
    uint32_t index;

    if (hero_odbc_given_back != 0) {
        index = hero_odbc_given_back - 1;
        hero_odbc_given_back = hero_odbc_slots[index].next;
    } else if (hero_odbc_fresh < HERO_ODBC_SLOTS) {
        index = hero_odbc_fresh;
        hero_odbc_fresh += 1;
    } else {
        return 0;
    }
    hero_odbc_slot *slot = &hero_odbc_slots[index];

    slot->life += 1;
    slot->next = 0;
    slot->busy = 0;
    slot->parent = parent;
    slot->kind = kind;
    slot->held = true;
    slot->closing = false;
    slot->handle = handle;
    slot->work = work;
    return hero_odbc_value(index, slot->life);
}

/* The slot given back for a later life. Under the lock, with no call in
 * flight on it. A slot at its last life is never issued again. */
static void hero_odbc_release(hero_odbc_slot *slot) {
    slot->held = false;
    slot->closing = false;
    slot->kind = 0;
    slot->handle = NULL;
    slot->work = NULL;
    slot->parent = 0;
    if (slot->life < HERO_ODBC_LIFE_MOST) {
        slot->next = hero_odbc_given_back;
        hero_odbc_given_back = (uint32_t)(slot - hero_odbc_slots) + 1;
    }
}

static void hero_odbc_work_free(hero_odbc_work *work) {
    if (work == NULL) {
        return;
    }
    for (size_t i = 0; i < work->param_count; i += 1) {
        hero_layer_free(work->params[i].bytes);
    }
    hero_layer_free(work->params);
    hero_odbc_bytes_free(&work->cell);
    hero_odbc_bytes_free(&work->wide);
    hero_layer_free(work);
}

/* What a close or the last call leaving must give back, gathered under the
 * lock and done outside it, since the driver may take its time: the
 * statements' handles first, then the connection's. */
typedef struct hero_odbc_ending {
    hero_odbc_slot *connection;
    hero_odbc_slot *statement;
} hero_odbc_ending;

/* Gives the driver's handles back for what `ending` names, outside the lock,
 * then the slots under it: a connection's statements first, its transaction
 * rolled back and its connection ended. Every slot named is closing and no
 * call is in flight on it, so no other call reads its handle meanwhile.
 * Answers the disconnect's result, or `HERO_SQL_SUCCESS`. */
static hero_sql_return hero_odbc_end(hero_odbc_ending ending) {
    hero_sql_return said = HERO_SQL_SUCCESS;

    if (ending.statement != NULL) {
        (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_STMT, ending.statement->handle);
        hero_odbc_work_free(ending.statement->work);
        hero_odbc_enter();
        hero_odbc_release(ending.statement);
        hero_odbc_leave();
    }
    if (ending.connection != NULL) {
        uint32_t own = (uint32_t)(ending.connection - hero_odbc_slots) + 1;
        hero_odbc_slot **mine = NULL;
        size_t count = 0;
        size_t kept = 0;

        /* The connection's statements, listed under the lock; where no
         * memory is left for the list they are given back under it. */
        hero_odbc_enter();
        for (uint32_t i = 0; i < hero_odbc_fresh; i += 1) {
            hero_odbc_slot *s = &hero_odbc_slots[i];

            count += s->held && s->kind == HERO_ODBC_KIND_STATEMENT && s->parent == own;
        }
        mine = count == 0 ? NULL : hero_layer_alloc(count * sizeof *mine);
        for (uint32_t i = 0; i < hero_odbc_fresh; i += 1) {
            hero_odbc_slot *s = &hero_odbc_slots[i];

            if (s->held && s->kind == HERO_ODBC_KIND_STATEMENT && s->parent == own) {
                if (mine != NULL) {
                    mine[kept] = s;
                    kept += 1;
                } else {
                    (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_STMT, s->handle);
                    hero_odbc_work_free(s->work);
                    hero_odbc_release(s);
                }
            }
        }
        hero_odbc_leave();
        for (size_t k = 0; k < kept; k += 1) {
            (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_STMT, mine[k]->handle);
            hero_odbc_work_free(mine[k]->work);
        }
        hero_sql_handle dbc = ending.connection->handle;

        (void)hero_odbc_call.EndTran(HERO_SQL_HANDLE_DBC, dbc, HERO_SQL_ROLLBACK);
        said = hero_odbc_call.Disconnect(dbc);
        if (!hero_odbc_succeeded(said)) {
            hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, dbc, said, "SQLDisconnect");
        }
        (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_DBC, dbc);
        hero_odbc_enter();
        for (size_t k = 0; k < kept; k += 1) {
            hero_odbc_release(mine[k]);
        }
        hero_odbc_release(ending.connection);
        hero_odbc_leave();
        hero_layer_free(mine);
    }
    return said;
}

/* The connection `c` names, marked busy; NULL with `HERO_ODBC_NOT_OPEN`. */
static hero_odbc_slot *hero_odbc_take_connection(hero_odbc_connection *c) {
    hero_odbc_enter();
    hero_odbc_slot *slot = hero_odbc_slot_of((uint64_t)(uintptr_t)c, HERO_ODBC_KIND_CONNECTION);

    if (slot != NULL) {
        slot->busy += 1;
    }
    hero_odbc_leave();
    if (slot == NULL) {
        hero_odbc_said(HERO_ODBC_NOT_OPEN, "the connection is not open: closed already, or never opened");
    }
    return slot;
}

/* The statement `s` names, marked busy with its connection; NULL with
 * `HERO_ODBC_NOT_OPEN` where it names none or its connection is closing,
 * and `HERO_ODBC_INVALID` where another call holds it. */
static hero_odbc_slot *hero_odbc_take_statement(hero_odbc_statement *s) {
    const char *refused = "the statement is not open: closed already, its connection closed, or never opened";
    int64_t kind = HERO_ODBC_NOT_OPEN;

    hero_odbc_enter();
    hero_odbc_slot *slot = hero_odbc_slot_of((uint64_t)(uintptr_t)s, HERO_ODBC_KIND_STATEMENT);

    if (slot != NULL) {
        hero_odbc_slot *parent = &hero_odbc_slots[slot->parent - 1];

        if (!parent->held || parent->closing) {
            slot = NULL;
        } else if (slot->busy > 0) {
            kind = HERO_ODBC_INVALID;
            refused = "the statement is in use by a call on another thread";
            slot = NULL;
        } else {
            slot->busy += 1;
            parent->busy += 1;
        }
    }
    hero_odbc_leave();
    if (slot == NULL) {
        hero_odbc_said(kind, refused);
    }
    return slot;
}

/* A call done with the slot it took; the last call to leave a slot closed
 * meanwhile gives its handles back. The call's own failure, if any, is kept:
 * what the ending would say is not the call's. */
static void hero_odbc_done(hero_odbc_slot *slot) {
    hero_odbc_ending ending = { 0 };

    hero_odbc_enter();
    slot->busy -= 1;
    if (slot->kind == HERO_ODBC_KIND_STATEMENT) {
        hero_odbc_slot *parent = &hero_odbc_slots[slot->parent - 1];

        parent->busy -= 1;
        if (parent->closing && parent->busy == 0) {
            ending.connection = parent;
        } else if (slot->closing && slot->busy == 0) {
            ending.statement = slot;
        }
    } else if (slot->closing && slot->busy == 0) {
        ending.connection = slot;
    }
    hero_odbc_leave();
    if (ending.connection != NULL || ending.statement != NULL) {
        int64_t kind = hero_odbc_kind_said;
        int64_t native = hero_odbc_native_said;
        char state[6];
        char words[HERO_ODBC_WORDS_MOST];

        memcpy(state, hero_odbc_state_said, sizeof state);
        memcpy(words, hero_odbc_words, sizeof words);
        (void)hero_odbc_end(ending);
        hero_odbc_kind_said = kind;
        hero_odbc_native_said = native;
        memcpy(hero_odbc_state_said, state, sizeof state);
        memcpy(hero_odbc_words, words, sizeof words);
    }
}

/* -- THE MANAGER: WHICH, ITS DRIVERS, ITS DATA SOURCES ---------------------------- */

int64_t hero_odbc_manager(void *buffer, uint64_t count) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_fine();
    return hero_odbc_fill(buffer, count, hero_odbc_loaded_name, strlen(hero_odbc_loaded_name));
}

/* A zero-ended run of `key=value` texts, the run ended by a second zero, in
 * at most `units` units: each appended to `into` after a tab. */
static bool hero_odbc_put_attributes(const hero_odbc_unit *list, size_t units, hero_odbc_bytes *into) {
    size_t at = 0;

    while (at < units && list[at] != 0) {
        size_t n = hero_odbc_units(list + at, units - at);

        if (!hero_odbc_put(into, "\t", 1) || !hero_odbc_from_native(list + at, n, into)) {
            return false;
        }
        at += n + 1;
    }
    return true;
}

/* Lines of the manager's drivers or data sources into `buffer` by the fill
 * form. */
static int64_t hero_odbc_list(bool drivers, hero_sql_usmall first, void *buffer, uint64_t count) {
    enum { NAME_UNITS = 1024, MORE_UNITS = 16384 };
    hero_odbc_unit *name = hero_layer_alloc(NAME_UNITS * sizeof(hero_odbc_unit));
    hero_odbc_unit *more = hero_layer_alloc(MORE_UNITS * sizeof(hero_odbc_unit));
    hero_odbc_bytes lines = { 0 };
    bool ok = name != NULL && more != NULL;
    hero_sql_usmall direction = first;

    if (!ok) {
        hero_odbc_said(HERO_ODBC_LIMIT, "no memory left for the list");
    }
    while (ok) {
        hero_sql_small name_length = 0;
        hero_sql_small more_length = 0;

        memset(more, 0, MORE_UNITS * sizeof(hero_odbc_unit));
        hero_sql_return rc = drivers
            ? hero_odbc_call.Drivers(hero_odbc_env, direction, name, NAME_UNITS, &name_length, more, MORE_UNITS, &more_length)
            : hero_odbc_call.DataSources(hero_odbc_env, direction, name, NAME_UNITS, &name_length, more, MORE_UNITS, &more_length);

        if (rc == HERO_SQL_NO_DATA) {
            break;
        }
        if (!hero_odbc_succeeded(rc)) {
            hero_odbc_driver_failed(HERO_SQL_HANDLE_ENV, hero_odbc_env, rc, drivers ? "SQLDrivers" : "SQLDataSources");
            ok = false;
            break;
        }
        if (name_length < 0 || name_length >= NAME_UNITS || more_length < 0 || more_length >= MORE_UNITS) {
            hero_odbc_said(HERO_ODBC_OTHER, drivers ? "a driver's name or attributes passed what the layer reads, 1024 and 16384 characters" : "a data source's name or description passed what the layer reads, 1024 and 16384 characters");
            ok = false;
            break;
        }
        ok = hero_odbc_from_native(name, (size_t)name_length, &lines);
        if (ok && drivers) {
            ok = hero_odbc_put_attributes(more, MORE_UNITS, &lines);
        } else if (ok) {
            ok = hero_odbc_put(&lines, "\t", 1) && hero_odbc_from_native(more, (size_t)more_length, &lines);
        }
        ok = ok && hero_odbc_put(&lines, "\n", 1);
        direction = HERO_SQL_FETCH_NEXT;
    }
    hero_layer_free(name);
    hero_layer_free(more);
    int64_t answer = -1;

    if (ok) {
        hero_odbc_fine();
        answer = hero_odbc_fill(buffer, count, lines.at, lines.count);
    }
    hero_odbc_bytes_free(&lines);
    return answer;
}

int64_t hero_odbc_drivers(void *buffer, uint64_t count) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    return hero_odbc_list(true, HERO_SQL_FETCH_FIRST, buffer, count);
}

int64_t hero_odbc_data_sources(int64_t which, void *buffer, uint64_t count) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_sql_usmall first;

    if (which == HERO_ODBC_SOURCES_ALL) {
        first = HERO_SQL_FETCH_FIRST;
    } else if (which == HERO_ODBC_SOURCES_USER) {
        first = HERO_SQL_FETCH_FIRST_USER;
    } else if (which == HERO_ODBC_SOURCES_SYSTEM) {
        first = HERO_SQL_FETCH_FIRST_SYSTEM;
    } else {
        hero_odbc_said(HERO_ODBC_INVALID, "no such list of data sources: ask for all, the user's or the system's");
        return -1;
    }
    return hero_odbc_list(false, first, buffer, count);
}

/* -- CONNECTIONS ----------------------------------------------------------------- */

/* A connection handle made from the environment, its login timeout set;
 * NULL with the failure. */
static hero_sql_handle hero_odbc_new_dbc(int64_t login_seconds) {
    hero_sql_handle dbc = NULL;

    if (login_seconds < 0 || login_seconds > UINT32_MAX) {
        hero_odbc_said(HERO_ODBC_INVALID, "a login timeout is from 0 to 4294967295 seconds");
        return NULL;
    }
    hero_sql_return rc = hero_odbc_call.AllocHandle(HERO_SQL_HANDLE_DBC, hero_odbc_env, &dbc);

    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_ENV, hero_odbc_env, rc, "SQLAllocHandle");
        return NULL;
    }
    if (login_seconds > 0) {
        rc = hero_odbc_call.SetConnectAttr(dbc, HERO_SQL_ATTR_LOGIN_TIMEOUT, (void *)(uintptr_t)login_seconds, HERO_SQL_IS_UINTEGER);
        if (!hero_odbc_succeeded(rc)) {
            hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, dbc, rc, "SQLSetConnectAttr");
            (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_DBC, dbc);
            return NULL;
        }
    }
    return dbc;
}

/* `dbc`, connected, given a slot; disconnected and NULL with
 * `HERO_ODBC_LIMIT` where every slot is held. */
static hero_odbc_connection *hero_odbc_keep_dbc(hero_sql_handle dbc) {
    hero_odbc_enter();
    uint64_t value = hero_odbc_issue(HERO_ODBC_KIND_CONNECTION, dbc, 0, NULL);
    hero_odbc_leave();

    if (value == 0) {
        (void)hero_odbc_call.Disconnect(dbc);
        (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_DBC, dbc);
        hero_odbc_said(HERO_ODBC_LIMIT, "every one of the 16384 handles this layer keeps is open");
        return NULL;
    }
    hero_odbc_fine();
    return (hero_odbc_connection *)(uintptr_t)value;
}

/* A text as an argument's length in units: `SQLSMALLINT`'s for a connection
 * string or a name. */
static bool hero_odbc_small_length(const hero_odbc_native *n, hero_sql_small *length) {
    if (n->units > INT16_MAX) {
        hero_odbc_said(HERO_ODBC_INVALID, "a connection string or a name passed 32767 characters, the most ODBC takes");
        return false;
    }
    *length = n->at == NULL ? 0 : (hero_sql_small)n->units;
    return true;
}

hero_odbc_connection *hero_odbc_connect(const char *text, int64_t login_seconds) {
    if (!hero_odbc_ready()) {
        return NULL;
    }
    if (text == NULL) {
        hero_odbc_said(HERO_ODBC_INVALID, "no connection string was given");
        return NULL;
    }
    hero_odbc_native in = { 0 };
    hero_sql_small length = 0;

    if (!hero_odbc_name_to_native(text, &in) || !hero_odbc_small_length(&in, &length)) {
        hero_odbc_native_free(&in);
        return NULL;
    }
    hero_sql_handle dbc = hero_odbc_new_dbc(login_seconds);

    if (dbc == NULL) {
        hero_odbc_native_free(&in);
        return NULL;
    }
    hero_sql_return rc = hero_odbc_call.DriverConnect(dbc, NULL, in.at, length, NULL, 0, NULL, HERO_SQL_DRIVER_NOPROMPT);

    hero_odbc_native_free(&in);
    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, dbc, rc, "SQLDriverConnect");
        (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_DBC, dbc);
        return NULL;
    }
    return hero_odbc_keep_dbc(dbc);
}

hero_odbc_connection *hero_odbc_connect_dsn(const char *dsn, const char *user, const char *password, int64_t login_seconds) {
    if (!hero_odbc_ready()) {
        return NULL;
    }
    if (dsn == NULL || user == NULL || password == NULL) {
        hero_odbc_said(HERO_ODBC_INVALID, "a data source, a user and a password are each a text, \"\" for none");
        return NULL;
    }
    hero_odbc_native n[3] = { { 0 }, { 0 }, { 0 } };
    hero_sql_small lengths[3] = { 0, 0, 0 };
    const char *texts[3] = { dsn, user, password };
    bool ok = true;

    for (size_t i = 0; i < 3 && ok; i += 1) {
        ok = hero_odbc_name_to_native(texts[i], &n[i]) && hero_odbc_small_length(&n[i], &lengths[i]);
    }
    hero_sql_handle dbc = ok ? hero_odbc_new_dbc(login_seconds) : NULL;
    hero_sql_return rc = HERO_SQL_ERROR;

    if (dbc != NULL) {
        /* A user or a password of "" is none: the data source's own. */
        rc = hero_odbc_call.Connect(dbc, n[0].at, lengths[0], lengths[1] == 0 ? NULL : n[1].at, lengths[1], lengths[2] == 0 ? NULL : n[2].at, lengths[2]);
    }
    for (size_t i = 0; i < 3; i += 1) {
        hero_odbc_native_free(&n[i]);
    }
    if (dbc == NULL) {
        return NULL;
    }
    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, dbc, rc, "SQLConnect");
        (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_DBC, dbc);
        return NULL;
    }
    return hero_odbc_keep_dbc(dbc);
}

int64_t hero_odbc_disconnect(hero_odbc_connection *c) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_ending ending = { 0 };

    hero_odbc_enter();
    hero_odbc_slot *slot = hero_odbc_slot_of((uint64_t)(uintptr_t)c, HERO_ODBC_KIND_CONNECTION);

    if (slot != NULL) {
        uint32_t own = (uint32_t)(slot - hero_odbc_slots) + 1;

        slot->closing = true;
        for (uint32_t i = 0; i < hero_odbc_fresh; i += 1) {
            hero_odbc_slot *s = &hero_odbc_slots[i];

            if (s->held && s->kind == HERO_ODBC_KIND_STATEMENT && s->parent == own) {
                s->closing = true;
            }
        }
        if (slot->busy == 0) {
            ending.connection = slot;
        }
    }
    hero_odbc_leave();
    if (slot == NULL) {
        hero_odbc_said(HERO_ODBC_NOT_OPEN, "the connection is not open: closed already, or never opened");
        return -1;
    }
    hero_odbc_fine();
    if (ending.connection != NULL && !hero_odbc_succeeded(hero_odbc_end(ending))) {
        return -1;
    }
    return 0;
}

/* `SQLGetInfo`'s item for `which`, and whether it is a text, a 16-bit or a
 * 32-bit number; false for a `which` of none. */
static bool hero_odbc_info_item(int64_t which, hero_sql_usmall *item, int *width) {
    static const struct {
        int64_t which;
        hero_sql_usmall item;
        int width;
    } items[] = {
        { HERO_ODBC_INFO_DBMS_NAME, 17, 0 },
        { HERO_ODBC_INFO_DBMS_VERSION, 18, 0 },
        { HERO_ODBC_INFO_DRIVER_NAME, 6, 0 },
        { HERO_ODBC_INFO_DRIVER_VERSION, 7, 0 },
        { HERO_ODBC_INFO_DRIVER_ODBC_VERSION, 77, 0 },
        { HERO_ODBC_INFO_MANAGER_VERSION, 171, 0 },
        { HERO_ODBC_INFO_DATA_SOURCE_NAME, 2, 0 },
        { HERO_ODBC_INFO_DATABASE_NAME, 16, 0 },
        { HERO_ODBC_INFO_SERVER_NAME, 13, 0 },
        { HERO_ODBC_INFO_USER_NAME, 47, 0 },
        { HERO_ODBC_INFO_IDENTIFIER_QUOTE, 29, 0 },
        { HERO_ODBC_INFO_SEARCH_ESCAPE, 14, 0 },
        { HERO_ODBC_INFO_KEYWORDS, 89, 0 },
        { HERO_ODBC_INFO_MULTIPLE_RESULTS, 36, 0 },
        { HERO_ODBC_INFO_READ_ONLY, 25, 0 },
        { HERO_ODBC_INFO_TRANSACTIONS, 46, 16 },
        { HERO_ODBC_INFO_DEFAULT_ISOLATION, 26, 32 },
        { HERO_ODBC_INFO_ISOLATIONS, 72, 32 },
    };
    for (size_t i = 0; i < sizeof items / sizeof items[0]; i += 1) {
        if (items[i].which == which) {
            *item = items[i].item;
            *width = items[i].width;
            return true;
        }
    }
    return false;
}

int64_t hero_odbc_info(hero_odbc_connection *c, int64_t which, void *buffer, uint64_t count) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_sql_usmall item = 0;
    int width = 0;

    if (!hero_odbc_info_item(which, &item, &width)) {
        hero_odbc_said(HERO_ODBC_INVALID, "no such item of SQLGetInfo's");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_connection(c);

    if (slot == NULL) {
        return -1;
    }
    hero_odbc_bytes answer = { 0 };
    bool ok = true;

    if (width != 0) {
        uint64_t number = 0;
        hero_sql_return rc = hero_odbc_call.GetInfo(slot->handle, item, &number, (hero_sql_small)(width / 8), NULL);

        if (!hero_odbc_succeeded(rc)) {
            hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, slot->handle, rc, "SQLGetInfo");
            ok = false;
        } else {
            char digits[24];
            size_t n = 0;
            uint64_t v = width == 16 ? (number & 0xFFFFu) : (number & 0xFFFFFFFFu);

            do {
                digits[sizeof digits - 1 - n] = (char)('0' + v % 10);
                v /= 10;
                n += 1;
            } while (v != 0 && n < sizeof digits);
            ok = hero_odbc_put(&answer, digits + sizeof digits - n, n);
        }
    } else {
        /* The text's length is asked in bytes, a wide manager's included. */
        size_t units = 512;

        for (int round = 0; round < 3 && ok; round += 1) {
            hero_odbc_unit *text = hero_layer_alloc_zero(units + 1, sizeof(hero_odbc_unit));
            hero_sql_small length = 0;

            if (text == NULL) {
                hero_odbc_said(HERO_ODBC_LIMIT, "no memory left for an answer of SQLGetInfo");
                ok = false;
                break;
            }
            size_t room_bytes = units * sizeof(hero_odbc_unit);
            hero_sql_return rc = hero_odbc_call.GetInfo(slot->handle, item, text, (hero_sql_small)(room_bytes > INT16_MAX ? INT16_MAX : room_bytes), &length);

            if (!hero_odbc_succeeded(rc)) {
                hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, slot->handle, rc, "SQLGetInfo");
                ok = false;
            } else if (length >= 0 && (size_t)length + sizeof(hero_odbc_unit) > room_bytes && round < 2) {
                units = (size_t)length / sizeof(hero_odbc_unit) + 2;
            } else {
                size_t got = length < 0 ? 0 : (size_t)length / sizeof(hero_odbc_unit);

                if (got > units) {
                    got = units;
                }
                got = hero_odbc_units(text, got);
                ok = hero_odbc_from_native(text, got, &answer);
                round = 3;
            }
            hero_layer_free(text);
        }
    }
    hero_odbc_done(slot);
    int64_t result = -1;

    if (ok) {
        hero_odbc_fine();
        result = hero_odbc_fill(buffer, count, answer.at, answer.count);
    }
    hero_odbc_bytes_free(&answer);
    return result;
}

/* `SQLSetConnectAttr`'s attribute for `which`; false for none. */
static bool hero_odbc_attribute_of(int64_t which, hero_sql_int *attribute) {
    switch (which) {
    case HERO_ODBC_ATTR_AUTOCOMMIT:
        *attribute = HERO_SQL_ATTR_AUTOCOMMIT;
        return true;
    case HERO_ODBC_ATTR_ISOLATION:
        *attribute = HERO_SQL_ATTR_TXN_ISOLATION;
        return true;
    case HERO_ODBC_ATTR_READ_ONLY:
        *attribute = HERO_SQL_ATTR_ACCESS_MODE;
        return true;
    case HERO_ODBC_ATTR_CONNECTION_TIMEOUT:
        *attribute = HERO_SQL_ATTR_CONNECTION_TIMEOUT;
        return true;
    case HERO_ODBC_ATTR_DEAD:
        *attribute = HERO_SQL_ATTR_CONNECTION_DEAD;
        return true;
    default:
        return false;
    }
}

int64_t hero_odbc_set_attribute(hero_odbc_connection *c, int64_t which, int64_t value) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_sql_int attribute = 0;

    if (!hero_odbc_attribute_of(which, &attribute) || which == HERO_ODBC_ATTR_DEAD) {
        hero_odbc_said(HERO_ODBC_INVALID, "no such attribute of a connection to set");
        return -1;
    }
    if (value < 0 || value > UINT32_MAX) {
        hero_odbc_said(HERO_ODBC_INVALID, "an attribute's value is from 0 to 4294967295");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_connection(c);

    if (slot == NULL) {
        return -1;
    }
    hero_sql_return rc = hero_odbc_call.SetConnectAttr(slot->handle, attribute, (void *)(uintptr_t)value, HERO_SQL_IS_UINTEGER);

    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, slot->handle, rc, "SQLSetConnectAttr");
    } else {
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return hero_odbc_succeeded(rc) ? 0 : -1;
}

int64_t hero_odbc_attribute(hero_odbc_connection *c, int64_t which, int64_t *value) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_sql_int attribute = 0;

    if (!hero_odbc_attribute_of(which, &attribute)) {
        hero_odbc_said(HERO_ODBC_INVALID, "no such attribute of a connection to read");
        return -1;
    }
    if (value == NULL) {
        hero_odbc_said(HERO_ODBC_INVALID, "no place was given for the attribute");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_connection(c);

    if (slot == NULL) {
        return -1;
    }
    uint64_t got = 0;
    hero_sql_return rc = hero_odbc_call.GetConnectAttr(slot->handle, attribute, &got, HERO_SQL_IS_UINTEGER, NULL);

    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, slot->handle, rc, "SQLGetConnectAttr");
    } else {
        /* Every attribute read here is a `SQLUINTEGER`; a driver that wrote
         * eight bytes wrote the same number. */
        *value = (int64_t)(got & 0xFFFFFFFFu);
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return hero_odbc_succeeded(rc) ? 0 : -1;
}

int64_t hero_odbc_end_transaction(hero_odbc_connection *c, int64_t commit) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    if (commit != 0 && commit != 1) {
        hero_odbc_said(HERO_ODBC_INVALID, "a transaction ends by a commit, 1, or a rollback, 0");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_connection(c);

    if (slot == NULL) {
        return -1;
    }
    hero_sql_return rc = hero_odbc_call.EndTran(HERO_SQL_HANDLE_DBC, slot->handle, commit == 1 ? HERO_SQL_COMMIT : HERO_SQL_ROLLBACK);

    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, slot->handle, rc, "SQLEndTran");
    } else {
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return hero_odbc_succeeded(rc) ? 0 : -1;
}

/* -- STATEMENTS ---------------------------------------------------------------- */

hero_odbc_statement *hero_odbc_statement_open(hero_odbc_connection *c) {
    if (!hero_odbc_ready()) {
        return NULL;
    }
    hero_odbc_slot *slot = hero_odbc_take_connection(c);

    if (slot == NULL) {
        return NULL;
    }
    hero_odbc_work *work = hero_layer_alloc_zero(1, sizeof *work);
    hero_sql_handle stmt = NULL;
    uint64_t value = 0;

    if (work == NULL) {
        hero_odbc_said(HERO_ODBC_LIMIT, "no memory left for a statement");
    } else {
        hero_sql_return rc = hero_odbc_call.AllocHandle(HERO_SQL_HANDLE_STMT, slot->handle, &stmt);

        if (!hero_odbc_succeeded(rc)) {
            hero_odbc_driver_failed(HERO_SQL_HANDLE_DBC, slot->handle, rc, "SQLAllocHandle");
            stmt = NULL;
        } else {
            hero_odbc_enter();
            value = hero_odbc_issue(HERO_ODBC_KIND_STATEMENT, stmt, (uint32_t)(slot - hero_odbc_slots) + 1, work);
            hero_odbc_leave();
            if (value == 0) {
                (void)hero_odbc_call.FreeHandle(HERO_SQL_HANDLE_STMT, stmt);
                hero_odbc_said(HERO_ODBC_LIMIT, "every one of the 16384 handles this layer keeps is open");
            } else {
                hero_odbc_fine();
            }
        }
        if (value == 0) {
            hero_odbc_work_free(work);
        }
    }
    hero_odbc_done(slot);
    return (hero_odbc_statement *)(uintptr_t)value;
}

int64_t hero_odbc_statement_close(hero_odbc_statement *s) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_ending ending = { 0 };

    hero_odbc_enter();
    hero_odbc_slot *slot = hero_odbc_slot_of((uint64_t)(uintptr_t)s, HERO_ODBC_KIND_STATEMENT);

    if (slot != NULL) {
        slot->closing = true;
        if (slot->busy == 0) {
            ending.statement = slot;
        }
    }
    hero_odbc_leave();
    if (slot == NULL) {
        hero_odbc_said(HERO_ODBC_NOT_OPEN, "the statement is not open: closed already, its connection closed, or never opened");
        return -1;
    }
    hero_odbc_fine();
    if (ending.statement != NULL) {
        (void)hero_odbc_end(ending);
    }
    return 0;
}

int64_t hero_odbc_belongs(hero_odbc_statement *s, hero_odbc_connection *c) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    int64_t answer = -1;

    hero_odbc_enter();
    hero_odbc_slot *statement = hero_odbc_slot_of((uint64_t)(uintptr_t)s, HERO_ODBC_KIND_STATEMENT);
    hero_odbc_slot *connection = hero_odbc_slot_of((uint64_t)(uintptr_t)c, HERO_ODBC_KIND_CONNECTION);

    if (statement != NULL && connection != NULL) {
        answer = statement->parent == (uint32_t)(connection - hero_odbc_slots) + 1;
    }
    hero_odbc_leave();
    if (answer < 0) {
        hero_odbc_said(HERO_ODBC_NOT_OPEN, "the statement or the connection is not open");
    } else {
        hero_odbc_fine();
    }
    return answer;
}

int64_t hero_odbc_set_timeout(hero_odbc_statement *s, int64_t seconds) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    if (seconds < 0 || seconds > UINT32_MAX) {
        hero_odbc_said(HERO_ODBC_INVALID, "a statement's timeout is from 0 to 4294967295 seconds");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    hero_sql_return rc = hero_odbc_call.SetStmtAttr(slot->handle, HERO_SQL_ATTR_QUERY_TIMEOUT, (void *)(uintptr_t)seconds, HERO_SQL_IS_UINTEGER);

    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLSetStmtAttr");
    } else {
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return hero_odbc_succeeded(rc) ? 0 : -1;
}

/* A statement's text in the manager's units, its length an `SQLINTEGER`. */
static bool hero_odbc_statement_text(const char *text, hero_odbc_native *n) {
    if (text == NULL) {
        hero_odbc_said(HERO_ODBC_INVALID, "no statement was given");
        return false;
    }
    return hero_odbc_to_native(text, strlen(text), n, "a statement passed 1073741824 bytes, the most the layer takes");
}

int64_t hero_odbc_prepare(hero_odbc_statement *s, const char *text) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_native n = { 0 };

    if (!hero_odbc_statement_text(text, &n)) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        hero_odbc_native_free(&n);
        return -1;
    }
    (void)hero_odbc_call.FreeStmt(slot->handle, HERO_SQL_CLOSE);
    hero_sql_return rc = hero_odbc_call.Prepare(slot->handle, n.at, (hero_sql_int)n.units);

    hero_odbc_native_free(&n);
    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLPrepare");
    } else {
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return hero_odbc_succeeded(rc) ? 0 : -1;
}

int64_t hero_odbc_parameter_count(hero_odbc_statement *s) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    hero_sql_small count = 0;
    hero_sql_return rc = hero_odbc_call.NumParams(slot->handle, &count);

    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLNumParams");
    } else {
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return hero_odbc_succeeded(rc) ? (int64_t)(count < 0 ? 0 : count) : -1;
}

/* The parameter at `index` of the statement `s` names, its old value
 * forgotten, with the statement taken; NULL with the failure. The caller
 * fills it and calls `hero_odbc_done`. */
static hero_odbc_param *hero_odbc_param_at(hero_odbc_statement *s, uint64_t index, hero_odbc_slot **taken) {
    *taken = NULL;
    if (!hero_odbc_ready()) {
        return NULL;
    }
    if (index < 1 || index > UINT16_MAX) {
        hero_odbc_said(HERO_ODBC_INVALID, "a parameter's index is from 1 to 65535");
        return NULL;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return NULL;
    }
    hero_odbc_work *work = slot->work;

    if (index > work->param_count) {
        hero_odbc_param *grown = hero_layer_grow(work->params, (size_t)index * sizeof *grown);

        if (grown == NULL) {
            hero_odbc_said(HERO_ODBC_LIMIT, "no memory left for a parameter");
            hero_odbc_done(slot);
            return NULL;
        }
        memset(grown + work->param_count, 0, ((size_t)index - work->param_count) * sizeof *grown);
        work->params = grown;
        work->param_count = (size_t)index;
    }
    hero_odbc_param *p = &work->params[index - 1];

    hero_layer_free(p->bytes);
    memset(p, 0, sizeof *p);
    *taken = slot;
    return p;
}

int64_t hero_odbc_bind_null(hero_odbc_statement *s, uint64_t index) {
    hero_odbc_slot *slot;
    hero_odbc_param *p = hero_odbc_param_at(s, index, &slot);

    if (p == NULL) {
        return -1;
    }
    p->set = true;
    p->c_type = HERO_SQL_C_CHAR;
    p->sql_type = HERO_SQL_VARCHAR;
    p->size = 1;
    p->indicator = HERO_SQL_NULL_DATA;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

int64_t hero_odbc_bind_boolean(hero_odbc_statement *s, uint64_t index, int64_t value) {
    if (value != 0 && value != 1) {
        hero_odbc_said(HERO_ODBC_INVALID, "a truth value is 1 or 0");
        return -1;
    }
    hero_odbc_slot *slot;
    hero_odbc_param *p = hero_odbc_param_at(s, index, &slot);

    if (p == NULL) {
        return -1;
    }
    p->set = true;
    p->c_type = HERO_SQL_C_BIT;
    p->sql_type = HERO_SQL_BIT;
    p->size = 1;
    p->bit = (unsigned char)value;
    p->indicator = 1;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

int64_t hero_odbc_bind_integer(hero_odbc_statement *s, uint64_t index, int64_t value) {
    hero_odbc_slot *slot;
    hero_odbc_param *p = hero_odbc_param_at(s, index, &slot);

    if (p == NULL) {
        return -1;
    }
    p->set = true;
    p->c_type = HERO_SQL_C_SBIGINT;
    p->sql_type = HERO_SQL_BIGINT;
    p->size = 19;
    p->integer = value;
    p->indicator = (hero_sql_len)sizeof p->integer;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

int64_t hero_odbc_bind_float(hero_odbc_statement *s, uint64_t index, double value) {
    hero_odbc_slot *slot;
    hero_odbc_param *p = hero_odbc_param_at(s, index, &slot);

    if (p == NULL) {
        return -1;
    }
    p->set = true;
    p->c_type = HERO_SQL_C_DOUBLE;
    p->sql_type = HERO_SQL_DOUBLE;
    p->size = 15;
    p->number = value;
    p->indicator = (hero_sql_len)sizeof p->number;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

int64_t hero_odbc_bind_text(hero_odbc_statement *s, uint64_t index, const void *bytes, uint64_t count) {
    if (bytes == NULL && count > 0) {
        hero_odbc_said(HERO_ODBC_INVALID, "a text's bytes were not given");
        return -1;
    }
    if (count > HERO_ODBC_TEXT_MOST) {
        hero_odbc_said(HERO_ODBC_INVALID, "a parameter's text passed 1073741824 bytes, the most the layer takes");
        return -1;
    }
    hero_odbc_native n = { 0 };

    if (!hero_odbc_to_native((const char *)bytes, (size_t)count, &n, "a parameter's text passed the most the layer takes")) {
        return -1;
    }
    hero_odbc_slot *slot;
    hero_odbc_param *p = hero_odbc_param_at(s, index, &slot);

    if (p == NULL) {
        hero_odbc_native_free(&n);
        return -1;
    }
    p->set = true;
#if defined(_WIN32)
    p->c_type = HERO_SQL_C_WCHAR;
    p->sql_type = n.units > HERO_ODBC_SHORT_TEXT_MOST ? HERO_SQL_WLONGVARCHAR : HERO_SQL_WVARCHAR;
#else
    p->c_type = HERO_SQL_C_CHAR;
    p->sql_type = n.units > HERO_ODBC_SHORT_TEXT_MOST ? HERO_SQL_LONGVARCHAR : HERO_SQL_VARCHAR;
#endif
    p->size = n.units == 0 ? 1 : (hero_sql_ulen)n.units;
    p->bytes = n.at;
    p->count = n.units * sizeof(hero_odbc_unit);
    p->indicator = (hero_sql_len)p->count;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

int64_t hero_odbc_bind_bytes(hero_odbc_statement *s, uint64_t index, const void *bytes, uint64_t count) {
    if (bytes == NULL && count > 0) {
        hero_odbc_said(HERO_ODBC_INVALID, "a parameter's bytes were not given");
        return -1;
    }
    if (count > HERO_ODBC_TEXT_MOST) {
        hero_odbc_said(HERO_ODBC_INVALID, "a parameter's bytes passed 1073741824, the most the layer takes");
        return -1;
    }
    unsigned char *copy = hero_layer_alloc(count == 0 ? 1 : (size_t)count);

    if (copy == NULL) {
        hero_odbc_said(HERO_ODBC_LIMIT, "no memory left for a parameter's bytes");
        return -1;
    }
    if (count > 0) {
        memcpy(copy, bytes, (size_t)count);
    }
    hero_odbc_slot *slot;
    hero_odbc_param *p = hero_odbc_param_at(s, index, &slot);

    if (p == NULL) {
        hero_layer_free(copy);
        return -1;
    }
    p->set = true;
    p->c_type = HERO_SQL_C_BINARY;
    p->sql_type = count > HERO_ODBC_SHORT_BYTES_MOST ? HERO_SQL_LONGVARBINARY : HERO_SQL_VARBINARY;
    p->size = count == 0 ? 1 : (hero_sql_ulen)count;
    p->bytes = copy;
    p->count = (size_t)count;
    p->indicator = (hero_sql_len)count;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

int64_t hero_odbc_bind_timestamp(hero_odbc_statement *s, uint64_t index, const int64_t *parts, uint64_t count) {
    if (parts == NULL || count < HERO_ODBC_TIMESTAMP_PARTS) {
        hero_odbc_said(HERO_ODBC_INVALID, "a timestamp is seven parts: year, month, day, hour, minute, second, nanosecond");
        return -1;
    }
    static const int64_t least[HERO_ODBC_TIMESTAMP_PARTS] = { 1, 1, 1, 0, 0, 0, 0 };
    static const int64_t most[HERO_ODBC_TIMESTAMP_PARTS] = { 9999, 12, 31, 23, 59, 59, 999999999 };

    for (size_t i = 0; i < HERO_ODBC_TIMESTAMP_PARTS; i += 1) {
        if (parts[i] < least[i] || parts[i] > most[i]) {
            hero_odbc_said(HERO_ODBC_INVALID, "a timestamp's part is outside its range: years 1 to 9999, months 1 to 12, days 1 to 31, hours 0 to 23, minutes and seconds 0 to 59, nanoseconds 0 to 999999999");
            return -1;
        }
    }
    hero_odbc_slot *slot;
    hero_odbc_param *p = hero_odbc_param_at(s, index, &slot);

    if (p == NULL) {
        return -1;
    }
    int64_t ns = parts[6];
    hero_sql_small digits = ns == 0 ? 0 : ns % 1000000 == 0 ? 3 : ns % 1000 == 0 ? 6 : 9;

    p->set = true;
    p->c_type = HERO_SQL_C_TYPE_TIMESTAMP;
    p->sql_type = HERO_SQL_TYPE_TIMESTAMP;
    p->digits = digits;
    p->size = (hero_sql_ulen)(digits == 0 ? 19 : 20 + digits);
    p->stamp.year = (hero_sql_small)parts[0];
    p->stamp.month = (hero_sql_usmall)parts[1];
    p->stamp.day = (hero_sql_usmall)parts[2];
    p->stamp.hour = (hero_sql_usmall)parts[3];
    p->stamp.minute = (hero_sql_usmall)parts[4];
    p->stamp.second = (hero_sql_usmall)parts[5];
    p->stamp.fraction = (hero_sql_uint)ns;
    p->indicator = (hero_sql_len)sizeof p->stamp;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

int64_t hero_odbc_unbind(hero_odbc_statement *s) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    hero_odbc_work *work = slot->work;

    for (size_t i = 0; i < work->param_count; i += 1) {
        hero_layer_free(work->params[i].bytes);
    }
    hero_layer_free(work->params);
    work->params = NULL;
    work->param_count = 0;
    (void)hero_odbc_call.FreeStmt(slot->handle, HERO_SQL_RESET_PARAMS);
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

/* Every value of `work` handed to the driver as its parameter, the cursor
 * of a run before closed; false with the failure, a gap among them
 * included. Called by a call that holds the statement. */
static bool hero_odbc_bind_all(hero_odbc_slot *slot) {
    hero_odbc_work *work = slot->work;

    (void)hero_odbc_call.FreeStmt(slot->handle, HERO_SQL_CLOSE);
    (void)hero_odbc_call.FreeStmt(slot->handle, HERO_SQL_RESET_PARAMS);
    for (size_t i = 0; i < work->param_count; i += 1) {
        hero_odbc_param *p = &work->params[i];
        void *value;
        hero_sql_len room = 0;

        if (!p->set) {
            hero_odbc_said(HERO_ODBC_INVALID, "a parameter below the last one bound was given no value");
            return false;
        }
        switch (p->c_type) {
        case HERO_SQL_C_BIT:
            value = &p->bit;
            room = 1;
            break;
        case HERO_SQL_C_SBIGINT:
            value = &p->integer;
            room = (hero_sql_len)sizeof p->integer;
            break;
        case HERO_SQL_C_DOUBLE:
            value = &p->number;
            room = (hero_sql_len)sizeof p->number;
            break;
        case HERO_SQL_C_TYPE_TIMESTAMP:
            value = &p->stamp;
            room = (hero_sql_len)sizeof p->stamp;
            break;
        default:
            /* An empty text or run of bytes points at a byte of its own,
             * which the driver reads none of. */
            value = p->bytes != NULL ? p->bytes : (void *)&p->bit;
            room = (hero_sql_len)p->count;
            break;
        }
        hero_sql_return rc = hero_odbc_call.BindParameter(slot->handle, (hero_sql_usmall)(i + 1), HERO_SQL_PARAM_INPUT, p->c_type, p->sql_type, p->size, p->digits, value, room, &p->indicator);

        if (!hero_odbc_succeeded(rc)) {
            hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLBindParameter");
            (void)hero_odbc_call.FreeStmt(slot->handle, HERO_SQL_RESET_PARAMS);
            return false;
        }
    }
    return true;
}

/* A run's result: a searched change of no row (`SQL_NO_DATA`) is none's
 * failure. The driver forgets the parameters' addresses at once. */
static int64_t hero_odbc_ran(hero_odbc_slot *slot, hero_sql_return rc, const char *what) {
    int64_t result = 0;

    /* The diagnostics first: every call on the handle but `SQLGetDiagRec`
     * clears them, `SQLFreeStmt` included. */
    if (hero_odbc_succeeded(rc) || rc == HERO_SQL_NO_DATA) {
        hero_odbc_fine();
    } else if (rc == HERO_SQL_ERROR || rc == HERO_SQL_INVALID_HANDLE) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, what);
        result = -1;
    } else {
        /* `SQL_NEED_DATA` or `SQL_STILL_EXECUTING`: neither is asked for. */
        hero_odbc_said(HERO_ODBC_OTHER, "the driver asked for data at run time or ran apart, which no call here asks for");
        (void)hero_odbc_call.FreeStmt(slot->handle, HERO_SQL_CLOSE);
        result = -1;
    }
    (void)hero_odbc_call.FreeStmt(slot->handle, HERO_SQL_RESET_PARAMS);
    return result;
}

int64_t hero_odbc_execute(hero_odbc_statement *s) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    int64_t result = -1;

    if (hero_odbc_bind_all(slot)) {
        result = hero_odbc_ran(slot, hero_odbc_call.Execute(slot->handle), "SQLExecute");
    }
    slot->work->cell_kind = 0;
    hero_odbc_done(slot);
    return result;
}

int64_t hero_odbc_execute_direct(hero_odbc_statement *s, const char *text) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_native n = { 0 };

    if (!hero_odbc_statement_text(text, &n)) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        hero_odbc_native_free(&n);
        return -1;
    }
    int64_t result = -1;

    if (hero_odbc_bind_all(slot)) {
        result = hero_odbc_ran(slot, hero_odbc_call.ExecDirect(slot->handle, n.at, (hero_sql_int)n.units), "SQLExecDirect");
    }
    hero_odbc_native_free(&n);
    slot->work->cell_kind = 0;
    hero_odbc_done(slot);
    return result;
}

int64_t hero_odbc_catalog(hero_odbc_statement *s, int64_t which, const char *a, const char *b, const char *c, const char *d, const char *e, const char *f, int64_t given) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    if (which < HERO_ODBC_CATALOG_TABLES || which > HERO_ODBC_CATALOG_COLUMN_PRIVILEGES) {
        hero_odbc_said(HERO_ODBC_INVALID, "no such question of the catalog's");
        return -1;
    }
    if (given < 0 || given > 63) {
        hero_odbc_said(HERO_ODBC_INVALID, "the names given are six bits, from 0 to 63");
        return -1;
    }
    const char *texts[6] = { a, b, c, d, e, f };
    hero_odbc_native n[6] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
    hero_sql_small lengths[6] = { 0, 0, 0, 0, 0, 0 };
    bool ok = true;

    for (size_t i = 0; i < 6 && ok; i += 1) {
        if ((given >> i) & 1) {
            if (texts[i] == NULL) {
                hero_odbc_said(HERO_ODBC_INVALID, "a name said to be given was not");
                ok = false;
            } else {
                ok = hero_odbc_name_to_native(texts[i], &n[i]) && hero_odbc_small_length(&n[i], &lengths[i]);
            }
        }
    }
    hero_odbc_slot *slot = ok ? hero_odbc_take_statement(s) : NULL;
    hero_sql_return rc = HERO_SQL_ERROR;

    if (slot != NULL) {
        hero_sql_handle h = slot->handle;

        (void)hero_odbc_call.FreeStmt(h, HERO_SQL_CLOSE);
        (void)hero_odbc_call.FreeStmt(h, HERO_SQL_RESET_PARAMS);
        switch (which) {
        case HERO_ODBC_CATALOG_TABLES:
            rc = hero_odbc_call.Tables(h, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2], n[3].at, lengths[3]);
            break;
        case HERO_ODBC_CATALOG_COLUMNS:
            rc = hero_odbc_call.Columns(h, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2], n[3].at, lengths[3]);
            break;
        case HERO_ODBC_CATALOG_PRIMARY_KEYS:
            rc = hero_odbc_call.PrimaryKeys(h, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2]);
            break;
        case HERO_ODBC_CATALOG_FOREIGN_KEYS:
            rc = hero_odbc_call.ForeignKeys(h, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2], n[3].at, lengths[3], n[4].at, lengths[4], n[5].at, lengths[5]);
            break;
        case HERO_ODBC_CATALOG_INDEXES:
        case HERO_ODBC_CATALOG_UNIQUE_INDEXES:
            rc = hero_odbc_call.Statistics(h, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2], which == HERO_ODBC_CATALOG_UNIQUE_INDEXES ? HERO_SQL_INDEX_UNIQUE : HERO_SQL_INDEX_ALL, HERO_SQL_QUICK);
            break;
        case HERO_ODBC_CATALOG_PROCEDURES:
            rc = hero_odbc_call.Procedures(h, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2]);
            break;
        case HERO_ODBC_CATALOG_PROCEDURE_COLUMNS:
            rc = hero_odbc_call.ProcedureColumns(h, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2], n[3].at, lengths[3]);
            break;
        case HERO_ODBC_CATALOG_TYPES:
            rc = hero_odbc_call.GetTypeInfo(h, HERO_SQL_ALL_TYPES);
            break;
        case HERO_ODBC_CATALOG_ROW_ID_COLUMNS:
        case HERO_ODBC_CATALOG_ROW_VERSION_COLUMNS:
            rc = hero_odbc_call.SpecialColumns(h, which == HERO_ODBC_CATALOG_ROW_ID_COLUMNS ? HERO_SQL_BEST_ROWID : HERO_SQL_ROWVER, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2], HERO_SQL_SCOPE_CURROW, HERO_SQL_NULLABLE);
            break;
        case HERO_ODBC_CATALOG_TABLE_PRIVILEGES:
            rc = hero_odbc_call.TablePrivileges(h, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2]);
            break;
        default:
            rc = hero_odbc_call.ColumnPrivileges(h, n[0].at, lengths[0], n[1].at, lengths[1], n[2].at, lengths[2], n[3].at, lengths[3]);
            break;
        }
        if (!hero_odbc_succeeded(rc)) {
            hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, h, rc, "a catalog function");
        } else {
            hero_odbc_fine();
        }
        slot->work->cell_kind = 0;
        hero_odbc_done(slot);
    }
    for (size_t i = 0; i < 6; i += 1) {
        hero_odbc_native_free(&n[i]);
    }
    return hero_odbc_succeeded(rc) ? 0 : -1;
}

int64_t hero_odbc_column_count(hero_odbc_statement *s) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    hero_sql_small count = 0;
    hero_sql_return rc = hero_odbc_call.NumResultCols(slot->handle, &count);

    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLNumResultCols");
    } else {
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return hero_odbc_succeeded(rc) ? (int64_t)(count < 0 ? 0 : count) : -1;
}

/* Whether `column` is an index of a column; else the failure. */
static bool hero_odbc_column_index(uint64_t column) {
    if (column < 1 || column > UINT16_MAX) {
        hero_odbc_said(HERO_ODBC_INVALID, "a column's index is from 1 to 65535");
        return false;
    }
    return true;
}

/* The column's name, its type, or both, as `SQLDescribeCol` gives them. */
static bool hero_odbc_describe(hero_odbc_slot *slot, uint64_t column, hero_odbc_bytes *name, int64_t *type) {
    size_t units = 256;

    for (int round = 0; round < 2; round += 1) {
        hero_odbc_unit *text = hero_layer_alloc_zero(units + 1, sizeof(hero_odbc_unit));
        hero_sql_small length = 0;
        hero_sql_small sql_type = 0;
        hero_sql_ulen size = 0;
        hero_sql_small digits = 0;
        hero_sql_small nullable = 0;

        if (text == NULL) {
            hero_odbc_said(HERO_ODBC_LIMIT, "no memory left for a column's name");
            return false;
        }
        hero_sql_return rc = hero_odbc_call.DescribeCol(slot->handle, (hero_sql_usmall)column, text, (hero_sql_small)units, &length, &sql_type, &size, &digits, &nullable);

        if (!hero_odbc_succeeded(rc)) {
            hero_layer_free(text);
            hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLDescribeCol");
            return false;
        }
        if (length >= 0 && (size_t)length >= units && round == 0 && length < INT16_MAX) {
            units = (size_t)length + 1;
            hero_layer_free(text);
            continue;
        }
        bool ok = true;

        if (name != NULL) {
            size_t got = length < 0 ? 0 : (size_t)length;

            ok = hero_odbc_from_native(text, hero_odbc_units(text, got < units ? got : units), name);
        }
        if (type != NULL) {
            *type = sql_type;
        }
        hero_layer_free(text);
        return ok;
    }
    return false;
}

int64_t hero_odbc_column_name(hero_odbc_statement *s, uint64_t column, void *buffer, uint64_t count) {
    if (!hero_odbc_ready() || !hero_odbc_column_index(column)) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    hero_odbc_bytes name = { 0 };
    bool ok = hero_odbc_describe(slot, column, &name, NULL);

    hero_odbc_done(slot);
    int64_t result = -1;

    if (ok) {
        hero_odbc_fine();
        result = hero_odbc_fill(buffer, count, name.at, name.count);
    }
    hero_odbc_bytes_free(&name);
    return result;
}

int64_t hero_odbc_column_type(hero_odbc_statement *s, uint64_t column, int64_t *type) {
    if (!hero_odbc_ready() || !hero_odbc_column_index(column)) {
        return -1;
    }
    if (type == NULL) {
        hero_odbc_said(HERO_ODBC_INVALID, "no place was given for the type");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    bool ok = hero_odbc_describe(slot, column, NULL, type);

    hero_odbc_done(slot);
    if (ok) {
        hero_odbc_fine();
    }
    return ok ? 0 : -1;
}

int64_t hero_odbc_column_type_name(hero_odbc_statement *s, uint64_t column, void *buffer, uint64_t count) {
    if (!hero_odbc_ready() || !hero_odbc_column_index(column)) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    enum { UNITS = 512 };
    hero_odbc_unit text[UNITS + 1];
    hero_sql_small length = 0;
    hero_odbc_bytes name = { 0 };

    memset(text, 0, sizeof text);
    /* The room and the length are in bytes, a wide manager's included. */
    hero_sql_return rc = hero_odbc_call.ColAttribute(slot->handle, (hero_sql_usmall)column, HERO_SQL_DESC_TYPE_NAME, text, (hero_sql_small)(UNITS * sizeof(hero_odbc_unit)), &length, NULL);
    bool ok = hero_odbc_succeeded(rc);

    if (!ok) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLColAttribute");
    } else {
        size_t got = length < 0 ? 0 : (size_t)length / sizeof(hero_odbc_unit);

        ok = hero_odbc_from_native(text, hero_odbc_units(text, got < UNITS ? got : UNITS), &name);
    }
    hero_odbc_done(slot);
    int64_t result = -1;

    if (ok) {
        hero_odbc_fine();
        result = hero_odbc_fill(buffer, count, name.at, name.count);
    }
    hero_odbc_bytes_free(&name);
    return result;
}

int64_t hero_odbc_row_count(hero_odbc_statement *s, int64_t *rows) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    if (rows == NULL) {
        hero_odbc_said(HERO_ODBC_INVALID, "no place was given for the count");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    hero_sql_len n = 0;
    hero_sql_return rc = hero_odbc_call.RowCount(slot->handle, &n);

    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLRowCount");
    } else {
        *rows = n < 0 ? -1 : n;
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return hero_odbc_succeeded(rc) ? 0 : -1;
}

/* 1, 0 or -1 for a call that moves to a row or a result. */
static int64_t hero_odbc_moved(hero_odbc_statement *s, bool row) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    hero_sql_return rc = row ? hero_odbc_call.Fetch(slot->handle) : hero_odbc_call.MoreResults(slot->handle);
    int64_t result;

    slot->work->cell_kind = 0;
    if (hero_odbc_succeeded(rc)) {
        hero_odbc_fine();
        result = 1;
    } else if (rc == HERO_SQL_NO_DATA) {
        hero_odbc_fine();
        result = 0;
    } else {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, row ? "SQLFetch" : "SQLMoreResults");
        result = -1;
    }
    hero_odbc_done(slot);
    return result;
}

int64_t hero_odbc_fetch(hero_odbc_statement *s) {
    return hero_odbc_moved(s, true);
}

int64_t hero_odbc_more_results(hero_odbc_statement *s) {
    return hero_odbc_moved(s, false);
}

int64_t hero_odbc_close_cursor(hero_odbc_statement *s) {
    if (!hero_odbc_ready()) {
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    hero_sql_return rc = hero_odbc_call.FreeStmt(slot->handle, HERO_SQL_CLOSE);

    slot->work->cell_kind = 0;
    if (!hero_odbc_succeeded(rc)) {
        hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLFreeStmt");
    } else {
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return hero_odbc_succeeded(rc) ? 0 : -1;
}

/* A value of variable length read whole into `into`, piece by piece:
 * `terminator` bytes the driver writes after each piece of text. 1 a value,
 * 0 SQL's NULL, -1. */
static int64_t hero_odbc_read_long(hero_odbc_slot *slot, uint64_t column, hero_sql_small c_type, size_t terminator, hero_odbc_bytes *into) {
    into->count = 0;
    for (int piece = 0;; piece += 1) {
        if (!hero_odbc_room(into, 4096 + terminator)) {
            return -1;
        }
        size_t room = into->room - into->count;

        if (terminator == 2 && room % 2 != 0) {
            room -= 1;
        }
        if (room > (size_t)INT64_MAX) {
            room = (size_t)INT64_MAX;
        }
        hero_sql_len indicator = 0;
        hero_sql_return rc = hero_odbc_call.GetData(slot->handle, (hero_sql_usmall)column, c_type, into->at + into->count, (hero_sql_len)room, &indicator);

        if (rc == HERO_SQL_NO_DATA) {
            /* Every piece read: a value whose first call says so is empty. */
            return 1;
        }
        if (!hero_odbc_succeeded(rc)) {
            hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLGetData");
            return -1;
        }
        if (indicator == HERO_SQL_NULL_DATA) {
            return piece == 0 ? 0 : 1;
        }
        size_t usable = room - terminator;

        if (indicator != HERO_SQL_NO_TOTAL && indicator >= 0 && (size_t)indicator <= usable) {
            into->count += (size_t)indicator;
            return 1;
        }
        /* A piece of a longer value: the room filled but its terminator. */
        into->count += usable;
        if (indicator > 0 && (size_t)indicator > usable) {
            size_t rest = (size_t)indicator - usable;

            if (!hero_odbc_room(into, rest + terminator)) {
                return -1;
            }
        }
        if (rc == HERO_SQL_SUCCESS) {
            return 1;
        }
    }
}

int64_t hero_odbc_read(hero_odbc_statement *s, uint64_t column, int64_t as) {
    if (!hero_odbc_ready() || !hero_odbc_column_index(column)) {
        return -1;
    }
    if (as < HERO_ODBC_AS_BOOLEAN || as > HERO_ODBC_AS_TIMESTAMP) {
        hero_odbc_said(HERO_ODBC_INVALID, "no such kind to read a cell as");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot == NULL) {
        return -1;
    }
    hero_odbc_work *work = slot->work;
    hero_sql_len indicator = 0;
    hero_sql_return rc = HERO_SQL_SUCCESS;
    int64_t result = -1;

    work->cell_kind = 0;
    switch (as) {
    case HERO_ODBC_AS_BOOLEAN: {
        unsigned char bit = 0;

        rc = hero_odbc_call.GetData(slot->handle, (hero_sql_usmall)column, HERO_SQL_C_BIT, &bit, 1, &indicator);
        work->cell_integer = bit != 0;
        break;
    }
    case HERO_ODBC_AS_INTEGER:
        rc = hero_odbc_call.GetData(slot->handle, (hero_sql_usmall)column, HERO_SQL_C_SBIGINT, &work->cell_integer, (hero_sql_len)sizeof work->cell_integer, &indicator);
        break;
    case HERO_ODBC_AS_FLOAT:
        rc = hero_odbc_call.GetData(slot->handle, (hero_sql_usmall)column, HERO_SQL_C_DOUBLE, &work->cell_float, (hero_sql_len)sizeof work->cell_float, &indicator);
        break;
    case HERO_ODBC_AS_TIMESTAMP:
        memset(&work->cell_stamp, 0, sizeof work->cell_stamp);
        rc = hero_odbc_call.GetData(slot->handle, (hero_sql_usmall)column, HERO_SQL_C_TYPE_TIMESTAMP, &work->cell_stamp, (hero_sql_len)sizeof work->cell_stamp, &indicator);
        break;
    case HERO_ODBC_AS_BYTES:
        result = hero_odbc_read_long(slot, column, HERO_SQL_C_BINARY, 0, &work->cell);
        break;
    default:
#if defined(_WIN32)
        result = hero_odbc_read_long(slot, column, HERO_SQL_C_WCHAR, 2, &work->wide);
        if (result == 1) {
            work->cell.count = 0;
            if (!hero_odbc_from_native((const hero_odbc_unit *)work->wide.at, work->wide.count / 2, &work->cell)) {
                result = -1;
            }
        }
#else
        result = hero_odbc_read_long(slot, column, HERO_SQL_C_CHAR, 1, &work->cell);
#endif
        break;
    }
    if (as == HERO_ODBC_AS_BOOLEAN || as == HERO_ODBC_AS_INTEGER || as == HERO_ODBC_AS_FLOAT || as == HERO_ODBC_AS_TIMESTAMP) {
        if (!hero_odbc_succeeded(rc)) {
            hero_odbc_driver_failed(HERO_SQL_HANDLE_STMT, slot->handle, rc, "SQLGetData");
            result = -1;
        } else {
            result = indicator == HERO_SQL_NULL_DATA ? 0 : 1;
        }
    }
    if (result == 1) {
        work->cell_kind = as;
        hero_odbc_fine();
    } else if (result == 0) {
        work->cell_kind = 0;
        hero_odbc_fine();
    }
    hero_odbc_done(slot);
    return result;
}

/* The statement `s` names, taken, where its cell holds a value of one of
 * the kinds `a` and `b`; NULL with the failure. */
static hero_odbc_slot *hero_odbc_cell_of(hero_odbc_statement *s, int64_t a, int64_t b) {
    if (!hero_odbc_ready()) {
        return NULL;
    }
    hero_odbc_slot *slot = hero_odbc_take_statement(s);

    if (slot != NULL && slot->work->cell_kind != a && slot->work->cell_kind != b) {
        hero_odbc_said(HERO_ODBC_INVALID, "the cell holds no value of this kind: read a column first");
        hero_odbc_done(slot);
        return NULL;
    }
    return slot;
}

int64_t hero_odbc_cell_integer(hero_odbc_statement *s, int64_t *value) {
    if (value == NULL) {
        hero_odbc_said(HERO_ODBC_INVALID, "no place was given for the integer");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_cell_of(s, HERO_ODBC_AS_INTEGER, HERO_ODBC_AS_BOOLEAN);

    if (slot == NULL) {
        return -1;
    }
    *value = slot->work->cell_integer;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

int64_t hero_odbc_cell_float(hero_odbc_statement *s, double *value) {
    if (value == NULL) {
        hero_odbc_said(HERO_ODBC_INVALID, "no place was given for the float");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_cell_of(s, HERO_ODBC_AS_FLOAT, HERO_ODBC_AS_FLOAT);

    if (slot == NULL) {
        return -1;
    }
    *value = slot->work->cell_float;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}

int64_t hero_odbc_cell_bytes(hero_odbc_statement *s, void *buffer, uint64_t count) {
    hero_odbc_slot *slot = hero_odbc_cell_of(s, HERO_ODBC_AS_TEXT, HERO_ODBC_AS_BYTES);

    if (slot == NULL) {
        return -1;
    }
    int64_t length = hero_odbc_fill(buffer, count, slot->work->cell.at, slot->work->cell.count);

    hero_odbc_fine();
    hero_odbc_done(slot);
    return length;
}

int64_t hero_odbc_cell_timestamp(hero_odbc_statement *s, int64_t *parts, uint64_t count) {
    if (parts == NULL || count < HERO_ODBC_TIMESTAMP_PARTS) {
        hero_odbc_said(HERO_ODBC_INVALID, "a timestamp is seven parts: room for seven was not given");
        return -1;
    }
    hero_odbc_slot *slot = hero_odbc_cell_of(s, HERO_ODBC_AS_TIMESTAMP, HERO_ODBC_AS_TIMESTAMP);

    if (slot == NULL) {
        return -1;
    }
    const hero_sql_timestamp *t = &slot->work->cell_stamp;

    parts[0] = t->year;
    parts[1] = t->month;
    parts[2] = t->day;
    parts[3] = t->hour;
    parts[4] = t->minute;
    parts[5] = t->second;
    parts[6] = t->fraction;
    hero_odbc_fine();
    hero_odbc_done(slot);
    return 0;
}
