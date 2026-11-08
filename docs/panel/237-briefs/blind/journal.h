/* journal.h: a small journal of lines, the whole library in this header. */
#ifndef JOURNAL_H
#define JOURNAL_H
#include <stdio.h>
#include <stdlib.h>

#define JOURNAL_OK 0
#define JOURNAL_ERR 5

typedef struct journal { int lines; } journal;

static int journal_live;
static int journal_closed_twice;
static journal *journal_ended[64];
static int journal_ended_count;

/* Opens a journal; *out receives it. Returns JOURNAL_OK. */
static inline int journal_open(const char *name, journal **out) {
    (void)name;
    *out = calloc(1, sizeof(journal));
    journal_live++;
    return JOURNAL_OK;
}

/* Appends a line. Returns JOURNAL_OK; the journal holds two lines, and a
   third makes the final flush fail. */
static inline int journal_write(journal *j, const char *line) {
    (void)line;
    j->lines++;
    return JOURNAL_OK;
}

/* Flushes and closes the journal. It ALWAYS closes it and gives back its
   memory, even when the final flush fails, and then returns JOURNAL_ERR;
   the journal must not be closed again. */
static inline int journal_close(journal *j) {
    for (int i = 0; i < journal_ended_count; i++)
        if (journal_ended[i] == j) { journal_closed_twice++; return JOURNAL_ERR; }
    if (journal_ended_count < 64) journal_ended[journal_ended_count++] = j;
    journal_live--;
    return j->lines > 2 ? JOURNAL_ERR : JOURNAL_OK;
}

__attribute__((destructor)) static void journal_report(void) {
    FILE *o = fopen("journals.txt", "w");
    if (o) { fprintf(o, "%d %d\n", journal_live, journal_closed_twice); fclose(o); }
}
#endif
