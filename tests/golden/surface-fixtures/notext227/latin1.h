/* Defect 227, panel 189's R6: a C header holding one byte that is not
 * UTF-8, 0xE9 in café as a Latin-1 editor writes it, committed as bytes.
 * Its cache key has a form no file of text can have, whatever that file
 * spells: selfhost/module/reading.hero and selfhost/cli/deps.hero read it. */
static inline int answer(void) { return 4; }
