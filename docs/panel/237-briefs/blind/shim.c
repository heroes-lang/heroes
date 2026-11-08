#include <sqlite3.h>
#include <stdio.h>
static int live;
static int my_open(const char *f, sqlite3 **db) { int rc = sqlite3_open(f, db); if (rc == SQLITE_OK) live++; return rc; }
static int my_close(sqlite3 *db) { int rc = sqlite3_close(db); if (rc == SQLITE_OK && db) live--; return rc; }
static int my_close_v2(sqlite3 *db) { int rc = sqlite3_close_v2(db); if (rc == SQLITE_OK && db) live--; return rc; }
__attribute__((destructor)) static void report(void) { FILE *o = fopen("connections.txt", "w"); if (o) { fprintf(o, "%d\n", live); fclose(o); } }
__attribute__((used)) static struct { const void *n, *o; } interposers[] __attribute__((section("__DATA,__interpose"))) = {
  { (const void *)my_open, (const void *)sqlite3_open },
  { (const void *)my_close, (const void *)sqlite3_close },
  { (const void *)my_close_v2, (const void *)sqlite3_close_v2 },
};
