# Panel 192, the historian's brief

Read `00-shared.md`, `00-facts.md` and the critic's first pass
(`docs/panel/192-reports/completeness-critic-briefs.md`) first; they bind
you. You are advisory and hold no veto (your charter,
`.claude/agents/historian.md`). **Every claim you make is sourced by a page
you fetched in this sitting**, with its URL and the date you read it; a
recollection is inadmissible and is written as one where you cannot fetch it.
*Repaired after the critic's first pass: the text it read is
`historian-before-the-critic.md`.*

## What to find, for each question

1. **Q1 and Q2, invisible and reordering characters in source.**
   - Trojan Source, CVE-2021-42574: what each major toolchain did, and
     when:
     - Rust's lints on bidirectional code points in literals and comments,
       their names, their default level, the release, and **exactly which
       code points they cover** (whether ZWJ, ZWNJ or the variation
       selectors are among them);
     - GCC's `-Wbidi-chars`, its default and whether it distinguishes an
       unbalanced run;
     - Clang;
     - Go;
     - Python;
     - Java;
     - Swift;
     - Zig.
   - **Showing rather than refusing**: what editors and code hosts did (an
     editor drawing the control visibly, a host's warning banner), and
     whether any compiler writes such a character visibly in its own output
     instead of refusing it.
   - Raw control characters in a string literal: which languages refuse
     them and which accept them, the tab above all (Go, Rust, Zig, Swift,
     Python, C, JavaScript).
   - Unicode's own guidance: UTS #55 (source code handling) and UAX #9's
     controls. Say what it recommends for a comment and for a string, and
     what it says of ZWJ, ZWNJ and the variation selectors, which real text
     needs.
2. **Q3, how a refused character is written.**
   - Which escape each language offers for a code point and for a byte
     (`\u{...}`, `\uXXXX`, `\xNN`, octal), whether it refuses `\u{0}` or a
     surrogate there, and whether a byte escape may write half a character
     (Rust refuses `\x80` and above in a string, by recollection: verify).
   - **One spelling**: whether any language refuses an escape that writes a
     character the literal could hold raw, or a raw character that has an
     escape, and what its formatter does.
   - A language with no byte escape: what it does.
   - **The shared brief's first version named Rust, Swift and JavaScript
     beside `\u{...}` and C, Python and Go beside `\xNN` from the
     coordinator's recollection: verify each.**
3. **Q4, a NUL lent to C.** What each language does when a string holding a
   NUL is handed to C or to the operating system:
   - Rust's `CString::new` and the standard library's path functions;
   - Go's `syscall.ByteSliceFromString` and the `os` package;
   - Python's *embedded null byte*;
   - Java's file APIs;
   - .NET;
   - Zig.

   Say whether each refuses at the lend, at the door, at construction, or
   not at all, and at what cost. Name the defects or CVEs a truncated path
   caused.
4. **Q5, an argument that is not Unicode on Windows.** What Rust's
   `std::env::args` and `args_os`, Go's `os.Args`, and Python's `sys.argv`
   do with a lone surrogate on Windows, and what WTF-8 is and who uses it.

## Your report

`docs/panel/192-reports/historian.md` in the trunk, written as you go. For
each question:
- the precedents, with their sources;
- what each says about the routes in the shared brief;
- a route the precedents suggest that the brief did not list;
- the condition that would change your reading.
