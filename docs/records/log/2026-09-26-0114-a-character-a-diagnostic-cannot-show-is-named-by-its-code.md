# A character a diagnostic cannot show is named by its code

2026-09-26 | the lexer's refusal of a character names it so the reader can find
it: a visible ASCII character as itself, a control character and a character a
terminal shows as nothing or that reorders the line by its code alone, any
other character above ASCII as itself and its code; and every JSON the compiler
writes goes through one escape | a stray 0x01 was printed raw between two
backquotes and the reader had nothing to look for, a right-to-left override
reversed the rest of the diagnostic's line, and a tab inside a correct
program's string literal made `lex --dump-tokens --json` write JSON no reader
accepts | design.md §4.17, CLAUDE.md §8 | no panel: the message of an existing
diagnostic, not a class, and no program means anything else; defect 102
