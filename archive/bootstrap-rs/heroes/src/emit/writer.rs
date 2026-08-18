//! The output, which has to count its own lines (design.md §3.1, panel 020).
//!
//! **`#line N` anchors the *next* line, and C then auto-increments.** That one
//! sentence is why this is a type and not a `String`. "Emit `#line` on source-line
//! change" is not implementable by comparing an instruction's span with the
//! previous instruction's: a Heroes line that lowers to K lines of C drifts by
//! K−1, silently, and the further into a function you read the further off the
//! mapping is. The comparison has to be against **what clang currently believes** —
//! the effective line — which this writer tracks.
//!
//! **And tracking the belief is half the job — the other half went missing for the
//! backend's whole life, repaired 2026-08-16.** The belief was tracked and then
//! honoured only at an `at_file` call, so the drift the paragraph above describes
//! was diagnosed and not actually prevented: an emitter that writes K lines under
//! one request positions once, and lines 2..K walk forward a source line each.
//! Measured over `tests/golden/run/`: **550 C lines across 81 of 81 programs**
//! landed on a blank line, on the next declaration, or past the end of the file.
//! `print` on the last line of a 6-line program put `hero_print_end()` on line 7
//! and the `return` on line 8, and lldb showed the author both — `two.hero:7`,
//! `two.hero:8`, in a file with six lines — so design.md §2's "steps through
//! `.hero` source lines" was false on the stepping half. The repair is `intent`
//! beside `claim`: **what was asked for**, as against **what clang believes**, with
//! `line` restoring the first whenever the second has aged out from under it.
//! Cost, measured on the same corpus: **+1.03%** generated C lines, no content line
//! changed, and `emit/` goldens differ only by the restored directives and the
//! shifted restore counts they push along.
//!
//! **The asymmetry that makes this cheap**: auto-increment is *correct* while the
//! claim names the generated file, because that file really does advance one line
//! per output line, and *wrong* while it names a source file, because K lines of C
//! belong to one Heroes line. So a restore is emitted only against a `.hero` claim,
//! which is why the fix costs a percent rather than a doubling.
//!
//! The frozen target had the bug. `tools/spike/01-first.c` advertises
//! `00-first.hero:2` in its own header comment; clang, given a deliberate type
//! error, reports `00-first.hero:6` — a blank line — and prints the *C* text under
//! the `.hero` file name. Measured by the panel's ffi-pragmatist against the file
//! CLAUDE.md calls the shape the emitter must produce.
//!
//! The **restore** to the generated file has the same requirement from the other
//! side: `#line <n> "<stem>.c"` must name the line the *next output line* will
//! actually be, so lldb and clang land on the housekeeping code itself rather than
//! blaming a user line for it. bison has the concept (`b4_sync_start` /
//! `b4_sync_end`) and points it at its skeletons; pointing it at the emitter's own
//! output appears to be original, and is flagged as such in the DESIGN-LOG rather
//! than claimed as precedent.
//!
//! The generated name is derived from the **source** stem, never from `-o`: the
//! emitted C must not mention the output path, or the double-emit determinism diff
//! would be a property of the invocation instead of the emitter.

/// What clang believes the next output line is: which file, and which line in it.
#[derive(Clone, PartialEq, Eq, Debug)]
enum Claim {
    /// Nothing has been claimed yet — the file is still its own truth.
    None,
    At { file: String, line: u32 },
}

pub(super) struct Writer {
    out: String,
    /// Lines pushed so far. The next line's number in the generated file is
    /// `pushed + 1`.
    pushed: u32,
    claim: Claim,
    /// What the emitter last **asked** for, as opposed to what clang currently
    /// believes. The two are the same only until a second line is written under
    /// one request, and keeping just the belief is what let every multi-line
    /// lowering walk off the end of the author's file.
    intent: Claim,
    generated: String,
}

impl Writer {
    pub(super) fn new(module: &str) -> Writer {
        Writer {
            out: String::new(),
            pushed: 0,
            claim: Claim::None,
            intent: Claim::None,
            generated: format!("{module}.c"),
        }
    }

    /// One line of C. Everything goes through here, which is what keeps the count
    /// honest.
    ///
    /// **Auto-increment is right for the generated file and wrong for a source
    /// one**, and that asymmetry is the whole of this method's second half. C
    /// advances the line after every output line; the generated file really does
    /// advance with it, so a claim on `<stem>.c` stays true for free. A claim on a
    /// `.hero` file does not: **one Heroes line lowers to K lines of C, and all K
    /// belong to the same source line**, so the claim has to be re-asserted rather
    /// than allowed to drift. Before the repair, the K−1 extra lines walked forward
    /// one source line each — `print` on the last line of a 6-line file put
    /// `hero_print_end()` on line 7 and the `return` on line 8, and lldb showed both
    /// (`two.hero:7`, `two.hero:8`, measured 2026-08-16). Over `tests/golden/run/`
    /// that was **550 C lines across 81 of 81 programs**, every one of them landing
    /// on a blank line, on the next declaration, or past the end of the file.
    pub(super) fn line(&mut self, text: &str) {
        if let Claim::At { file, .. } = &self.intent {
            if !self.points_at_generated() && self.claim != self.intent {
                let (file, line) = (file.clone(), self.intent_line());
                self.restore(&file, line);
            }
        }
        self.push(text);
    }

    /// The unconditional half of `line`, and the one `directive` uses: writing the
    /// `#line` itself must not ask whether a `#line` is needed.
    fn push(&mut self, text: &str) {
        self.out.push_str(text);
        self.out.push('\n');
        self.pushed += 1;
        if let Claim::At { line, .. } = &mut self.claim {
            *line += 1;
        }
    }

    fn points_at_generated(&self) -> bool {
        matches!(&self.intent, Claim::At { file, .. } if *file == self.generated)
    }

    fn intent_line(&self) -> u32 {
        match &self.intent {
            Claim::At { line, .. } => *line,
            Claim::None => 1,
        }
    }

    /// Re-state a claim the auto-increment has aged out from under. Same bytes as
    /// `directive`, without touching `intent` — the intent is what is being honoured.
    fn restore(&mut self, file: &str, line: u32) {
        self.claim = Claim::None;
        let text = format!("#line {line} \"{}\"", escape(file));
        self.push(&text);
        self.claim = Claim::At { file: file.to_string(), line };
    }

    pub(super) fn blank(&mut self) {
        self.line("");
    }

    /// Point the next line at the author's source, if it is not already pointing
    /// there.
    /// Point the next line at a file of the compilation, at that file's own
    /// line — which since M-module-namespace is not always the root's.
    ///
    /// **One entry point, and it replaced two.** There used to be `at_source`,
    /// which claimed the file the reader named, and `at_library`, which claimed
    /// the library. With N modules that split is wrong in the ordinary case: a
    /// `#line` for `geom.hero` claiming `main.hero` at a line past its end is
    /// the compiler blaming the author for a file they did not write in, and at
    /// M-ffi-ladder it hands §4.19's guarantee — clang checking an `extern` against the
    /// real header — to a file that does not contain the declaration.
    ///
    /// The library keeps its own name for the reason it always had: its lines
    /// are in a file the author cannot open, so a clang error against one must
    /// not look like theirs. That is now the same rule as every other module's,
    /// not an exception to it.
    /// **The test is the intent, not the claim.** A claim that has aged out under
    /// its own auto-increment is restored by `line`, at the moment a line is
    /// actually written — so asking again for what is already intended emits
    /// nothing here, and emits nothing at all if no line follows.
    pub(super) fn at_file(&mut self, file: &str, line: u32) {
        let line = line.max(1);
        if self.intent == (Claim::At { file: file.to_string(), line }) {
            return;
        }
        self.directive(file, line);
    }

    /// Point the next line at the generated file — around a prologue, a copy-out,
    /// the shim: code no user line is responsible for.
    pub(super) fn at_generated(&mut self) {
        if matches!(&self.claim, Claim::At { file, .. } if *file == self.generated) {
            return;
        }
        let next = self.pushed + 2; // this directive occupies one line first
        let generated = self.generated.clone();
        self.directive(&generated, next);
    }

    fn directive(&mut self, file: &str, line: u32) {
        // Cleared first, so the push does not increment a claim this directive is
        // about to replace. `push`, not `line`: writing a `#line` must not ask
        // whether one is needed.
        self.claim = Claim::None;
        let text = format!("#line {line} \"{}\"", escape(file));
        self.push(&text);
        self.claim = Claim::At { file: file.to_string(), line };
        self.intent = self.claim.clone();
    }

    pub(super) fn finish(self) -> String {
        self.out
    }
}

/// A path inside a C string literal. A `"` or a `\` in a file name would otherwise
/// end the directive early — and on Windows-style paths the backslash is the
/// separator, so this is not a hypothetical.
fn escape(path: &str) -> String {
    let mut out = String::with_capacity(path.len());
    for c in path.chars() {
        match c {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            _ => out.push(c),
        }
    }
    out
}

/// Point the next line at whichever source a span actually came from.
///
/// Every caller that has a span and a `Source` goes through here rather than
/// choosing for itself, so "is this the author's code or the library's" is
/// answered in one place (CLAUDE.md §11's rule about where a rule lives).
pub(super) fn at_span(w: &mut Writer, src: &crate::source::Source, offset: u32) {
    // `locate`, and nothing else: the file that holds the offset and the line
    // within it. The third caller in one milestone to need exactly this.
    let (file, line, _) = src.locate(offset);
    w.at_file(file, line);
}
