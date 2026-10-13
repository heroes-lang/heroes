/* Defect 2045, 2026-10-13: an enumerator named after a function the Heroes
 * runtime declares in every unit, written after a tab, which clang expands
 * alike in the line it quotes and under its caret. */
enum {	hero_str_len = 3 };
