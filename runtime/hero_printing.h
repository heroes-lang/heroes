/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_printing.h — the platform layer's printers: the printers this machine
 * knows listed with their state, the papers each takes and its default, a
 * PDF document submitted to one as a job with the settings a print dialog
 * asks (copies, paper, orientation, sides, colour, pages, collation), the
 * job's state and pages told while it runs and when it ends, and the job
 * cancelled (M-rebel-library, 2026-11-22; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h` is:
 * its functions are defined in `platform/hero_printing.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_printing.h"`, with no `link` word and no flag.
 *
 * THE SYSTEMS. On macOS and Linux, CUPS, the print system both ship: its
 * library, `libcups.2.dylib` or `libcups.so.2`, opened at the first call, and
 * the scheduler it speaks to, so a printer is a CUPS queue and its name the
 * queue's. On Windows, the print spooler: a printer is one `EnumPrinters`
 * lists, local or a connection, and its name the spooler's. Where the
 * library is not installed or no scheduler answers (a container, a server
 * with no print service), every call answers -1 with
 * `HERO_PRINTING_UNSUPPORTED`, and `hero_printing_last_message` says which.
 *
 * ONE DOCUMENT FORM, PDF, on every system: CUPS takes it as it is and
 * converts it for the printer; on Windows the system's own PDF renderer
 * (`Windows.Data.Pdf`) draws each page through Direct2D's print control into
 * an XPS print job, which the spooler converts for any driver. No shell verb
 * and no other program is started.
 *
 * WHAT CROSSES. A list is written into the caller's buffer as UTF-8 fields,
 * each followed by a zero byte, a fixed number of fields to each item
 * (`HERO_PRINTING_PRINTER_FIELDS`, `HERO_PRINTING_MEDIA_FIELDS`,
 * `HERO_PRINTING_JOB_FIELDS`), and the call answers how many bytes the whole
 * list takes: where that is more than `count`, nothing is written, and the
 * caller asks again with that many, the list being read anew. A number in a
 * field is decimal; a yes or no is `1` or `0`. A length is in hundredths of a
 * millimetre, IPP's unit, which Windows' tenths of a millimetre reach
 * exactly. A paper is named by its PWG 5101.1 self-describing name,
 * `iso_a4_210x297mm`, `na_letter_8.5x11in`, on every system.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a call answering -1, and
 * `hero_printing_last_error()` then names the kind,
 * `hero_printing_last_os_error()` the system's number (IPP's status on CUPS,
 * the Win32 error or the HRESULT on Windows), `hero_printing_last_message`
 * the system's own words; each held per thread. No call hands out a handle:
 * a job is its printer's name and its number, and every call is whole.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 */

#ifndef HERO_PRINTING_H
#define HERO_PRINTING_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform. */
#define HERO_PRINTING_OK 0
/* An argument the layer refuses: a name NULL, empty, longer than its bound
 * or no UTF-8; a document that is not a PDF; a setting outside its values;
 * page ranges out of order; a count with no bytes behind it. */
#define HERO_PRINTING_INVALID 1
/* No printer of that name, no paper of that name on the printer, or no job
 * of that number on the printer. */
#define HERO_PRINTING_NOT_FOUND 2
/* The system refuses this to the program: the job belongs to another user,
 * or the print service asks for credentials the program has not given. */
#define HERO_PRINTING_DENIED 3
/* The system has no memory, or the printer no room for another job. */
#define HERO_PRINTING_LIMIT 4
/* This machine has no print system this layer reaches (THE SYSTEMS, above),
 * or the printer takes no such setting. */
#define HERO_PRINTING_UNSUPPORTED 5
/* The system refused for a reason none of the above names, a printer that
 * takes no job now among them; the message carries its words. */
#define HERO_PRINTING_OTHER 6

/* The most bytes a printer's name, a paper's name or a job's title holds,
 * its zero byte not counted: IPP's bound on a name, `name(MAX)`. */
#define HERO_PRINTING_NAME_MOST 255
/* The most copies a job asks: IPP's common `copies-supported` bound. */
#define HERO_PRINTING_COPIES_MOST 999
/* The most page ranges a job names. */
#define HERO_PRINTING_RANGES_MOST 100

/* The fields of one printer, in this order: its name, which every other call
 * takes; the words a person reads for it (CUPS' `printer-info`, the name
 * again on Windows); where it stands; its make and model (the driver's name
 * on Windows); its state, `idle`, `processing` or `stopped`; why, as IPP's
 * `printer-state-reasons` keywords joined by commas (`media-empty`,
 * `offline-report`), empty where nothing is wrong; the system's own words
 * about its state, empty where it has none; whether it takes jobs now; and
 * whether it is the default printer. */
#define HERO_PRINTING_PRINTER_FIELDS 9
/* The fields of one paper: its PWG name; the words a person reads for it,
 * the system's own (`A4`, `Letter`); its width and its length; its unprinted
 * margins at the left, the right, the top and the bottom; and whether it is
 * the printer's default. */
#define HERO_PRINTING_MEDIA_FIELDS 9
/* The fields of one job's state: its state, `pending`, `held`,
 * `processing`, `stopped`, `cancelled`, `aborted` or `completed`; why, as
 * IPP's `job-state-reasons` keywords joined by commas, empty where the
 * system names none; the system's own words about it, empty where it has
 * none; and the pages printed so far, IPP's `job-impressions-completed`. */
#define HERO_PRINTING_JOB_FIELDS 4
/* The fields of what a printer can do: whether it prints on both sides,
 * whether it prints colour, and the most copies a job may ask of it, at most
 * `HERO_PRINTING_COPIES_MOST`. */
#define HERO_PRINTING_CAPABILITY_FIELDS 3

/* The orientations a job asks, IPP's `orientation-requested` numbers. */
#define HERO_PRINTING_PORTRAIT 3
#define HERO_PRINTING_LANDSCAPE 4
#define HERO_PRINTING_REVERSE_LANDSCAPE 5
#define HERO_PRINTING_REVERSE_PORTRAIT 6
/* The sides a job asks, IPP's `sides` keywords. */
#define HERO_PRINTING_ONE_SIDED 1
#define HERO_PRINTING_TWO_SIDED_LONG_EDGE 2
#define HERO_PRINTING_TWO_SIDED_SHORT_EDGE 3
/* The colour a job asks, IPP's `print-color-mode`: the printer's own
 * default, colour, or shades of grey. */
#define HERO_PRINTING_COLOR_DEFAULT 0
#define HERO_PRINTING_COLOR 1
#define HERO_PRINTING_MONOCHROME 2

/* The printers this machine knows, `HERO_PRINTING_PRINTER_FIELDS` fields
 * each, in the system's order, written into `buffer` (WHAT CROSSES, above),
 * and answers the bytes the list takes; 0 where it knows none. -1:
 * `INVALID`, `UNSUPPORTED`, `LIMIT`, `OTHER`. */
int64_t hero_printing_printers(void *buffer, uint64_t count);

/* The papers `printer` takes, `HERO_PRINTING_MEDIA_FIELDS` fields each, in
 * the system's order, written into `buffer`, and answers the bytes the list
 * takes; 0 where the printer names none. Asked of the printer's driver or of
 * the printer itself, so it may take a second. -1: `INVALID`, `NOT_FOUND`,
 * `UNSUPPORTED`, `LIMIT`, `OTHER`. */
int64_t hero_printing_media(const char *printer, void *buffer, uint64_t count);

/* What `printer` can do, `HERO_PRINTING_CAPABILITY_FIELDS` fields written
 * into `buffer`, and answers the bytes they take: what `hero_printing_submit`
 * holds a job's sides, colour and copies to. Asked of the printer's driver or
 * of the printer itself. -1: `INVALID`, `NOT_FOUND`, `UNSUPPORTED`,
 * `OTHER`. */
int64_t hero_printing_capabilities(const char *printer, void *buffer, uint64_t count);

/* Submits the `size` bytes of the PDF document `pdf` to `printer` as one
 * job titled `title`, and answers the job's number, more than 0, once the
 * system holds the whole document: what is printed is the system's from
 * then on, and `hero_printing_job` tells how it goes.
 *
 * - `copies`: 1 to `HERO_PRINTING_COPIES_MOST`.
 * - `media`: a paper's PWG name, which the printer must take; empty for the
 *   printer's default.
 * - `orientation`, `sides`, `color`: one of the numbers above.
 * - `collate`: 1 for each copy whole before the next, 0 for each page's
 *   copies together.
 * - `ranges`: `range_count` numbers, pairs of a first and a last page,
 *   counted from 1, each pair after the one before it and not touching it;
 *   none for every page.
 *
 * -1: `INVALID`, `NOT_FOUND`, `DENIED`, `LIMIT`, `UNSUPPORTED`, `OTHER`; no
 * job stands then, the layer cancelling one the system had begun. */
int64_t hero_printing_submit(const char *printer, const char *title, const void *pdf, uint64_t size, int64_t copies, const char *media, int64_t orientation, int64_t sides, int64_t color, int64_t collate, const int64_t *ranges, uint64_t range_count);

/* The state of job `job` on `printer`, `HERO_PRINTING_JOB_FIELDS` fields,
 * written into `buffer`, and answers the bytes they take. CUPS keeps an
 * ended job's state; the Windows spooler forgets a job once it ends, so the
 * layer answers for a job this process submitted the end it saw last, and
 * `completed` for one that left the queue printed or with no error.
 * -1: `INVALID`, `NOT_FOUND`, `DENIED`, `UNSUPPORTED`, `OTHER`. */
int64_t hero_printing_job(const char *printer, int64_t job, void *buffer, uint64_t count);

/* The numbers of the jobs on `printer` that have not ended, the user's own
 * and the others' the system shows, each a decimal field, oldest first,
 * written into `buffer`, and answers the bytes they take; 0 where none
 * waits. -1: `INVALID`, `NOT_FOUND`, `UNSUPPORTED`, `OTHER`. */
int64_t hero_printing_jobs(const char *printer, void *buffer, uint64_t count);

/* Cancels job `job` on `printer` and answers 0; a job that had already
 * ended stays as it ended, and answers 0 all the same. -1: `INVALID`,
 * `NOT_FOUND`, `DENIED`, `UNSUPPORTED`, `OTHER`. */
int64_t hero_printing_cancel(const char *printer, int64_t job);

/* Waits at most `within_ms` milliseconds, 0 to an hour, for job `job` on
 * `printer` to end, asking its state every quarter of a second, and answers
 * 1 once it has ended, 0 where it has not when the time is up. -1:
 * `INVALID`, `NOT_FOUND`, `DENIED`, `UNSUPPORTED`, `OTHER`. */
int64_t hero_printing_wait(const char *printer, int64_t job, int64_t within_ms);

/* The kind of the last failure on this thread, `HERO_PRINTING_OK` where the
 * last call did not fail. */
int64_t hero_printing_last_error(void);

/* The system's number for the last failure on this thread: IPP's status
 * code on CUPS, the Win32 error or the HRESULT on Windows; 0 where the layer
 * refused the call itself. */
int64_t hero_printing_last_os_error(void);

/* The words of the last failure on this thread, UTF-8 with no zero byte,
 * written into `buffer` where they fit in `count`, and answers their length,
 * so a caller asks again with room. */
int64_t hero_printing_last_message(void *buffer, uint64_t count);

#endif
