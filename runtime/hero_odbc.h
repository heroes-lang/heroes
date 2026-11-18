/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_odbc.h — the platform layer's ODBC: a connection to any database the
 * system's ODBC driver manager reaches, statements run with their values
 * bound as parameters, and their rows read cell by cell (M-rebel-library;
 * spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h`
 * is: its functions are defined in `platform/hero_odbc.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_odbc.h"`, with no `link` word and no flag.
 *
 * THE SYSTEM'S DRIVER MANAGER, LOADED WHEN A PROGRAM FIRST ASKS FOR IT, and
 * never named on the link line, so the layer compiles and links on a machine
 * with no ODBC at all, and there every call answers `HERO_ODBC_UNSUPPORTED`:
 *
 *   - Windows: `odbc32.dll`, which every Windows carries, loaded from the
 *     system's own folder alone. Text crosses the wide calls (`...W`) as
 *     UTF-16, converted from and to UTF-8 here.
 *   - Linux: unixODBC's `libodbc.so.2` (or `.so.1`), else iODBC's
 *     `libiodbc.so.2`.
 *   - macOS: unixODBC's `libodbc.2.dylib`, else iODBC's `libiodbc.2.dylib`,
 *     by the loader's own search and then in Homebrew's two folders,
 *     `/opt/homebrew/lib` and `/usr/local/lib`. macOS carries neither.
 *
 *   On Linux and macOS text crosses the narrow calls as UTF-8 bytes, which
 *   both managers hand a driver unchanged; the width of a wide character
 *   differs between them (two bytes in unixODBC, four in iODBC), and the
 *   narrow calls do not depend on it.
 *
 * TWO HANDLES, ISSUED AND CHECKED: a connection and a statement, both kept
 * in one table of slots under one lock and one tag of
 * `platform/hero_handle_tag.h`, so a statement's handle is never a
 * connection's at any slot and life, and one closed already, NULL, or one
 * this layer never issued is `HERO_ODBC_NOT_OPEN`, never a read of freed
 * memory. A statement belongs to the connection it was opened on: closing
 * the connection ends its statements too, and each of them answers
 * `HERO_ODBC_NOT_OPEN` after it. A call in flight keeps its handle: a close
 * from another thread meanwhile ends the handle when the last call on it
 * returns, never under it.
 *
 * A VALUE NEVER ENTERS A STATEMENT'S TEXT. Each `hero_odbc_bind_*` copies its
 * value into this layer's own memory, kept with the statement until
 * `hero_odbc_unbind` or its close; `hero_odbc_execute` and
 * `hero_odbc_execute_direct` hand every value to the driver as a parameter,
 * numbered from 1 as the statement's `?` stand.
 *
 * A ROW IS READ A CELL AT A TIME: `hero_odbc_fetch` moves to the next row,
 * `hero_odbc_read` reads one column of it, in ascending order, as the kind
 * the caller names, into the statement's cell, and the `hero_odbc_cell_*`
 * calls hand the cell over. A column is read once per row.
 *
 * THE FILL FORM, as `hero_disk.h` says it: a function that answers bytes
 * takes the caller's `buffer` and its `count`, writes nothing past them, and
 * answers how many bytes the WHOLE answer has. Where that is above `count`
 * the buffer was short, nothing in it is to be read, and the caller asks
 * again with at least that many. A `count` of 0 only measures.
 *
 * NO PLATFORM TYPE AND NO CONDITIONAL BUT THE GUARD: a group sees
 * `int64_t`, `uint64_t`, `double`, `void *`, `const char *` and the two
 * handles below; no header of ODBC's is read by a program.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a handle
 * answers NULL, one answering a count -1, and `hero_odbc_last_error()` then
 * names the kind. Where the driver manager or the driver refused,
 * `HERO_ODBC_DRIVER`, `hero_odbc_last_state` gives the SQLSTATE of its first
 * diagnostic record, `hero_odbc_last_os_error` that record's native error,
 * and `hero_odbc_last_message` the records' words. The last error is per
 * thread.
 */

#ifndef HERO_ODBC_H
#define HERO_ODBC_H

#include <stdint.h>

/* A connection to a data source. A group declares it as a handle,
 * `record Connection tag hero_odbc_connection`, which
 * `hero_odbc_disconnect` gives back. */
typedef struct hero_odbc_connection hero_odbc_connection;

/* A statement of a connection. A group declares it as a handle,
 * `record Statement tag hero_odbc_statement`, which
 * `hero_odbc_statement_close` gives back. */
typedef struct hero_odbc_statement hero_odbc_statement;

/* The kinds of failure, the same numbers on every platform. */
#define HERO_ODBC_OK 0
/* An argument the layer refuses: an index of no column or parameter, a
 * number outside its range, a text that is not UTF-8 where one is asked. */
#define HERO_ODBC_INVALID 1
/* A handle closed already, NULL, or one this layer never issued. */
#define HERO_ODBC_NOT_OPEN 2
/* No memory, or every slot of the table holds a handle. */
#define HERO_ODBC_LIMIT 3
/* No driver manager on this machine, or one without the function. */
#define HERO_ODBC_UNSUPPORTED 4
/* The driver manager or the driver refused: the SQLSTATE, the native error
 * and the words are the last diagnostic's. */
#define HERO_ODBC_DRIVER 5
#define HERO_ODBC_OTHER 6

/* The kinds a cell is read as, `hero_odbc_read`'s `as`. */
#define HERO_ODBC_AS_BOOLEAN 1
#define HERO_ODBC_AS_INTEGER 2
#define HERO_ODBC_AS_FLOAT 3
#define HERO_ODBC_AS_TEXT 4
#define HERO_ODBC_AS_BYTES 5
#define HERO_ODBC_AS_TIMESTAMP 6

/* The parts of a timestamp, in the order `hero_odbc_bind_timestamp` takes
 * them and `hero_odbc_cell_timestamp` gives them: year, month, day, hour,
 * minute, second, nanosecond. */
#define HERO_ODBC_TIMESTAMP_PARTS UINT64_C(7)

/* ODBC's numbers for the SQL types a column is described with, as the ODBC
 * specification gives them (`sql.h`, `sqlext.h`), which
 * `hero_odbc_column_type` answers. A driver may answer a number of its own
 * outside this list. */
#define HERO_ODBC_TYPE_CHAR 1
#define HERO_ODBC_TYPE_NUMERIC 2
#define HERO_ODBC_TYPE_DECIMAL 3
#define HERO_ODBC_TYPE_INTEGER 4
#define HERO_ODBC_TYPE_SMALLINT 5
#define HERO_ODBC_TYPE_FLOAT 6
#define HERO_ODBC_TYPE_REAL 7
#define HERO_ODBC_TYPE_DOUBLE 8
#define HERO_ODBC_TYPE_DATE 9
#define HERO_ODBC_TYPE_TIME 10
#define HERO_ODBC_TYPE_TIMESTAMP 11
#define HERO_ODBC_TYPE_VARCHAR 12
#define HERO_ODBC_TYPE_TYPE_DATE 91
#define HERO_ODBC_TYPE_TYPE_TIME 92
#define HERO_ODBC_TYPE_TYPE_TIMESTAMP 93
#define HERO_ODBC_TYPE_LONGVARCHAR (-1)
#define HERO_ODBC_TYPE_BINARY (-2)
#define HERO_ODBC_TYPE_VARBINARY (-3)
#define HERO_ODBC_TYPE_LONGVARBINARY (-4)
#define HERO_ODBC_TYPE_BIGINT (-5)
#define HERO_ODBC_TYPE_TINYINT (-6)
#define HERO_ODBC_TYPE_BIT (-7)
#define HERO_ODBC_TYPE_WCHAR (-8)
#define HERO_ODBC_TYPE_WVARCHAR (-9)
#define HERO_ODBC_TYPE_WLONGVARCHAR (-10)
#define HERO_ODBC_TYPE_GUID (-11)

/* What `hero_odbc_info` answers, each one of `SQLGetInfo`'s items: the
 * texts first, then the numbers, which it writes as decimal digits. */
#define HERO_ODBC_INFO_DBMS_NAME 1
#define HERO_ODBC_INFO_DBMS_VERSION 2
#define HERO_ODBC_INFO_DRIVER_NAME 3
#define HERO_ODBC_INFO_DRIVER_VERSION 4
#define HERO_ODBC_INFO_DRIVER_ODBC_VERSION 5
#define HERO_ODBC_INFO_MANAGER_VERSION 6
#define HERO_ODBC_INFO_DATA_SOURCE_NAME 7
#define HERO_ODBC_INFO_DATABASE_NAME 8
#define HERO_ODBC_INFO_SERVER_NAME 9
#define HERO_ODBC_INFO_USER_NAME 10
#define HERO_ODBC_INFO_IDENTIFIER_QUOTE 11
#define HERO_ODBC_INFO_SEARCH_ESCAPE 12
#define HERO_ODBC_INFO_KEYWORDS 13
#define HERO_ODBC_INFO_MULTIPLE_RESULTS 14
#define HERO_ODBC_INFO_READ_ONLY 15
#define HERO_ODBC_INFO_TRANSACTIONS 16
#define HERO_ODBC_INFO_DEFAULT_ISOLATION 17
#define HERO_ODBC_INFO_ISOLATIONS 18

/* The attributes of a connection `hero_odbc_set_attribute` sets and
 * `hero_odbc_attribute` reads, each a number. */
#define HERO_ODBC_ATTR_AUTOCOMMIT 1
#define HERO_ODBC_ATTR_ISOLATION 2
#define HERO_ODBC_ATTR_READ_ONLY 3
#define HERO_ODBC_ATTR_CONNECTION_TIMEOUT 4
#define HERO_ODBC_ATTR_DEAD 5

/* An isolation level, as ODBC numbers it: `HERO_ODBC_ATTR_ISOLATION`'s
 * values and the bits of `HERO_ODBC_INFO_ISOLATIONS`. */
#define HERO_ODBC_READ_UNCOMMITTED 1
#define HERO_ODBC_READ_COMMITTED 2
#define HERO_ODBC_REPEATABLE_READ 4
#define HERO_ODBC_SERIALIZABLE 8

/* The catalog's questions, `hero_odbc_catalog`'s `which`, each one of
 * ODBC's catalog functions; the names it takes are in its comment. */
#define HERO_ODBC_CATALOG_TABLES 1
#define HERO_ODBC_CATALOG_COLUMNS 2
#define HERO_ODBC_CATALOG_PRIMARY_KEYS 3
#define HERO_ODBC_CATALOG_FOREIGN_KEYS 4
#define HERO_ODBC_CATALOG_INDEXES 5
#define HERO_ODBC_CATALOG_UNIQUE_INDEXES 6
#define HERO_ODBC_CATALOG_PROCEDURES 7
#define HERO_ODBC_CATALOG_PROCEDURE_COLUMNS 8
#define HERO_ODBC_CATALOG_TYPES 9
#define HERO_ODBC_CATALOG_ROW_ID_COLUMNS 10
#define HERO_ODBC_CATALOG_ROW_VERSION_COLUMNS 11
#define HERO_ODBC_CATALOG_TABLE_PRIVILEGES 12
#define HERO_ODBC_CATALOG_COLUMN_PRIVILEGES 13

/* Which data sources `hero_odbc_data_sources` lists. */
#define HERO_ODBC_SOURCES_ALL 0
#define HERO_ODBC_SOURCES_USER 1
#define HERO_ODBC_SOURCES_SYSTEM 2

/* The most bytes a text handed to this layer may hold: a statement, a
 * connection string, a name, a parameter's text or bytes. */
#define HERO_ODBC_TEXT_MOST 1073741824

/* The kind of the last call's failure on this thread, `HERO_ODBC_OK` after a
 * call that did not fail. */
int64_t hero_odbc_last_error(void);

/* The native error of the last failure's first diagnostic record, the
 * driver's own number; 0 where there was none. */
int64_t hero_odbc_last_os_error(void);

/* The SQLSTATE of the last failure's first diagnostic record, five
 * characters, into `buffer` by the fill form; 0 where there was none. */
int64_t hero_odbc_last_state(void *buffer, uint64_t count);

/* The words of the last failure, into `buffer` by the fill form: the first
 * diagnostic record's, then each other's after `; ` with its SQLSTATE in
 * brackets, or the layer's own where no driver spoke. */
int64_t hero_odbc_last_message(void *buffer, uint64_t count);

/* The name of the driver manager's library this process loaded, into
 * `buffer` by the fill form; -1 with `HERO_ODBC_UNSUPPORTED` where none
 * would load, the message naming each library tried. */
int64_t hero_odbc_manager(void *buffer, uint64_t count);

/* The drivers the manager knows, into `buffer` by the fill form, one per
 * line ending `\n`: the driver's name, then each of its attributes as
 * `key=value`, every field after a tab. */
int64_t hero_odbc_drivers(void *buffer, uint64_t count);

/* The data sources the manager knows (`HERO_ODBC_SOURCES_*`), into `buffer`
 * by the fill form, one per line ending `\n`: the source's name, a tab, and
 * its driver's description. */
int64_t hero_odbc_data_sources(int64_t which, void *buffer, uint64_t count);

/* A connection made from a connection string (`SQLDriverConnect`, no
 * prompt), waiting at most `login_seconds` to log in, 0 for as long as the
 * driver waits; NULL on failure. */
hero_odbc_connection *hero_odbc_connect(const char *text, int64_t login_seconds);

/* A connection made to the data source `dsn` as `user` with `password`
 * (`SQLConnect`), "" leaving each to the data source; NULL on failure. */
hero_odbc_connection *hero_odbc_connect_dsn(const char *dsn, const char *user, const char *password, int64_t login_seconds);

/* Ends `c`: its statements closed, a transaction still open rolled back,
 * the connection ended and given back. 0, or -1 where `c` was not open or
 * the driver refused to disconnect; `c` is given back either way. */
int64_t hero_odbc_disconnect(hero_odbc_connection *c);

/* `SQLGetInfo`'s answer for `which` (`HERO_ODBC_INFO_*`), into `buffer` by
 * the fill form, a number written as its decimal digits. */
int64_t hero_odbc_info(hero_odbc_connection *c, int64_t which, void *buffer, uint64_t count);

/* Sets the attribute `which` (`HERO_ODBC_ATTR_*`) of `c` to `value`: 1 or 0
 * for autocommit and reading only, an isolation level, seconds for the
 * connection's timeout. 0, or -1. */
int64_t hero_odbc_set_attribute(hero_odbc_connection *c, int64_t which, int64_t value);

/* The attribute `which` of `c` into `value`. 0, or -1. */
int64_t hero_odbc_attribute(hero_odbc_connection *c, int64_t which, int64_t *value);

/* Ends `c`'s transaction (`SQLEndTran`): committed where `commit` is 1,
 * rolled back where it is 0. 0, or -1. */
int64_t hero_odbc_end_transaction(hero_odbc_connection *c, int64_t commit);

/* A new statement of `c`, or NULL. */
hero_odbc_statement *hero_odbc_statement_open(hero_odbc_connection *c);

/* Ends `s` and gives it back. 0, or -1 where it was not open. */
int64_t hero_odbc_statement_close(hero_odbc_statement *s);

/* 1 where `s` is a statement of `c`, 0 where it is another connection's,
 * -1 where either is not open. */
int64_t hero_odbc_belongs(hero_odbc_statement *s, hero_odbc_connection *c);

/* How long `s` runs before the driver stops it, in seconds, 0 for no
 * limit (`SQL_ATTR_QUERY_TIMEOUT`). 0, or -1. */
int64_t hero_odbc_set_timeout(hero_odbc_statement *s, int64_t seconds);

/* `text` read by the driver once, to be run by `hero_odbc_execute`. 0, or
 * -1. */
int64_t hero_odbc_prepare(hero_odbc_statement *s, const char *text);

/* How many parameters the prepared statement takes (`SQLNumParams`), or
 * -1. */
int64_t hero_odbc_parameter_count(hero_odbc_statement *s);

/* The value of the parameter at `index`, from 1, kept until
 * `hero_odbc_unbind`: SQL's NULL, a truth value 1 or 0, an integer, a
 * float, the first `count` bytes of `bytes` as UTF-8 text or as bytes, and
 * a timestamp's `HERO_ODBC_TIMESTAMP_PARTS` parts. Each 0, or -1. */
int64_t hero_odbc_bind_null(hero_odbc_statement *s, uint64_t index);
int64_t hero_odbc_bind_boolean(hero_odbc_statement *s, uint64_t index, int64_t value);
int64_t hero_odbc_bind_integer(hero_odbc_statement *s, uint64_t index, int64_t value);
int64_t hero_odbc_bind_float(hero_odbc_statement *s, uint64_t index, double value);
int64_t hero_odbc_bind_text(hero_odbc_statement *s, uint64_t index, const void *bytes, uint64_t count);
int64_t hero_odbc_bind_bytes(hero_odbc_statement *s, uint64_t index, const void *bytes, uint64_t count);
int64_t hero_odbc_bind_timestamp(hero_odbc_statement *s, uint64_t index, const int64_t *parts, uint64_t count);

/* Forgets every parameter's value. 0, or -1. */
int64_t hero_odbc_unbind(hero_odbc_statement *s);

/* Runs the prepared statement with the values bound, every parameter from
 * 1 to the highest bound given one; a gap below it is `HERO_ODBC_INVALID`.
 * A searched change that touches no row is no failure. 0, or -1. */
int64_t hero_odbc_execute(hero_odbc_statement *s);

/* Runs `text` once with the values bound (`SQLExecDirect`). 0, or -1. */
int64_t hero_odbc_execute_direct(hero_odbc_statement *s, const char *text);

/* Runs the catalog's question `which` (`HERO_ODBC_CATALOG_*`), whose rows
 * are then read as a query's. `a` to `f` are its names in ODBC's order: a
 * catalog, a schema and a table (or a procedure), then a column, a
 * procedure's column or the table's kinds in `d`; for foreign keys the
 * primary table's catalog, schema and table, then the foreign table's in
 * `d` to `f`. Bit `k` of `given`, from 0, says the `k`-th is given: one not
 * given is no name, which ODBC reads as every one, and a question reads
 * none of the names past its own. 0, or -1. */
int64_t hero_odbc_catalog(hero_odbc_statement *s, int64_t which, const char *a, const char *b, const char *c, const char *d, const char *e, const char *f, int64_t given);

/* How many columns the statement's current result has, 0 for one that
 * answers no rows, or -1. */
int64_t hero_odbc_column_count(hero_odbc_statement *s);

/* The name of the column at `column`, from 1, into `buffer` by the fill
 * form, or -1. */
int64_t hero_odbc_column_name(hero_odbc_statement *s, uint64_t column, void *buffer, uint64_t count);

/* The SQL type of the column at `column` into `type`, an
 * `HERO_ODBC_TYPE_*` or a number of the driver's own. 0, or -1. */
int64_t hero_odbc_column_type(hero_odbc_statement *s, uint64_t column, int64_t *type);

/* The database's own name for the column's type, into `buffer` by the fill
 * form, or -1. */
int64_t hero_odbc_column_type_name(hero_odbc_statement *s, uint64_t column, void *buffer, uint64_t count);

/* How many rows the statement inserted, changed or deleted into `rows`, -1
 * where the driver does not say. 0, or -1. */
int64_t hero_odbc_row_count(hero_odbc_statement *s, int64_t *rows);

/* Moves to the next row: 1 a row, 0 none left, -1. */
int64_t hero_odbc_fetch(hero_odbc_statement *s);

/* Moves to the statement's next result: 1 another, 0 none left, -1. */
int64_t hero_odbc_more_results(hero_odbc_statement *s);

/* Drops what is left of the statement's results. 0, or -1. */
int64_t hero_odbc_close_cursor(hero_odbc_statement *s);

/* Reads the column at `column`, from 1, of the current row as `as`
 * (`HERO_ODBC_AS_*`) into the statement's cell: 1 a value, 0 SQL's NULL, -1.
 * A row's columns are read in ascending order, each once. */
int64_t hero_odbc_read(hero_odbc_statement *s, uint64_t column, int64_t as);

/* The cell's integer, or its truth value as 1 or 0, into `value`. 0, or -1
 * where the cell holds none. */
int64_t hero_odbc_cell_integer(hero_odbc_statement *s, int64_t *value);

/* The cell's float into `value`. 0, or -1 where the cell holds none. */
int64_t hero_odbc_cell_float(hero_odbc_statement *s, double *value);

/* The cell's text, UTF-8 as the driver gave it, or its bytes, into
 * `buffer` by the fill form, or -1 where the cell holds neither. */
int64_t hero_odbc_cell_bytes(hero_odbc_statement *s, void *buffer, uint64_t count);

/* The cell's timestamp, its `HERO_ODBC_TIMESTAMP_PARTS` parts into
 * `parts`. 0, or -1 where the cell holds none or `count` is short. */
int64_t hero_odbc_cell_timestamp(hero_odbc_statement *s, int64_t *parts, uint64_t count);

#endif
