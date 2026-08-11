//! The file table: which of the concatenated files an offset belongs to.
//!
//! One `Source` holds every file of a compilation in one text (see `mod.rs` for
//! why), and this is the index that makes that reversible. Each entry knows
//! where its file starts, how many lines came before it, and what module it is
//! — so a diagnostic, a `#line` directive and the mangler can all ask the same
//! question of the same table and get the same answer.

/// One file inside a `Source`.
///
/// Owned strings, no references, and the table is a plain `Vec` — the Cyclone
/// rule (CLAUDE.md §5), and the shape the port reads as `files: [File]`.
pub struct FileEntry {
    /// The path as given, which is what a diagnostic prints. Two invocations
    /// naming the same file differently print differently and that is correct:
    /// the reader is told the path they typed.
    pub name: String,
    /// The module this file is: what a qualified name writes before the dot.
    /// The **raw** name, `_` and all, because that is what the author typed.
    pub module: String,
    /// The same module as a C identifier component — `module_of` of the above,
    /// so `print_inst` is `printinst`.
    ///
    /// **Two namespaces, and they are not the same namespace.** What the author
    /// types before the dot and what reaches the linker have different collision
    /// sets, and until panel 032 the emitter used the first for the second:
    /// module `print` with `inst_value_name` and module `print_inst` with
    /// `value_name` both emitted `h_print_inst_value_name`, and clang answered
    /// `redefinition` — exit 2, *the compiler is wrong*, on a legal program.
    /// That is the exact pair `emit/mangle.rs`'s own doc names as the reason the
    /// component is sanitised, and the sanitising was being skipped.
    pub component: String,
    /// Byte offset of this file's first byte in the concatenated text.
    pub start: u32,
    /// How many lines of the concatenated text come before it, so that
    /// `line_in_file = line_in_source - lines_before`.
    pub lines_before: u32,
    /// The Heroes library (§1.11 Tier 2). It is an ordinary module in every
    /// respect but one: a diagnostic pointing into it is a compiler bug, not a
    /// user error, and nothing that hands a program back may print it.
    pub is_library: bool,
}

/// The module a source path stands for.
///
/// The file's stem, alphanumerics only. Deterministic from the path *as given* —
/// `./x.hero`, `x.hero` and `/tmp/x.hero` all yield `x`, which is what keeps
/// `--emit-c` byte-identical across invocations that name the same file
/// differently.
///
/// **The module component is sanitised to `[A-Za-z0-9]`**, and that one line is
/// what makes `h_<module>_<name>` injective: without it the scheme is ambiguous
/// wherever both halves may contain `_` — module `print` with name
/// `inst_value_name` and module `print_inst` with name `value_name` are the same
/// symbol, and those are this compiler's own file names. With the module
/// alphanumeric, the first `_` after `h_` ends it.
///
/// **Residual, and now reachable**: `print_inst` and `printinst` are one
/// component, so two such modules in one program collide. Panel 031 R10 makes
/// that a Heroes diagnostic naming both files rather than a linker error —
/// checked where the module table is built, not here, because the collision is a
/// property of a *set* of files and this function knows about one.
///
/// It lives in `source` rather than in the emitter because at M8a a module is a
/// language-level thing — it is what a qualified name names — and the C symbol
/// is downstream of it. `emit::mangle` calls this.
pub fn module_of(path: &str) -> String {
    let file = path.rsplit(['/', '\\']).next().unwrap_or(path);
    let stem = file.split('.').next().unwrap_or(file);
    let kept: String = stem.chars().filter(|c| c.is_ascii_alphanumeric()).collect();
    if kept.is_empty() {
        // A file called `.hero` or `---.hero`. It has to become *something*, and
        // one fixed word is better than an empty component, which would make
        // `h__main` collide with a module named `_`.
        return "anon".to_string();
    }
    kept
}

/// The module component the library's own functions get, whatever is being
/// compiled. `range` is `h_library_range` in every program.
///
/// Decided at panel 028 R3b with the evidence in hand: every Heroes function is
/// emitted with external linkage, and two modules that both call `range` are
/// `ld: duplicate symbol` at every optimisation level. The rule is one
/// definition and many prototypes — which only works if the definition has the
/// *same* name everywhere, so the component cannot be a user file's stem.
pub const LIBRARY_MODULE: &str = "library";

/// What the library calls itself in a diagnostic and in a `#line`.
///
/// Not a path, and deliberately unopenable: a clang error against one of its
/// lines must not look like an error in a file the author can edit. It lives
/// here because this is where the name is *set* — the emitter used to carry its
/// own copy for a `#line` case that no longer exists (panel 032 D2).
pub const LIBRARY_FILE: &str = "<heroes library>";

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_module_is_the_stem_however_the_path_was_written() {
        assert_eq!(module_of("geom.hero"), "geom");
        assert_eq!(module_of("./geom.hero"), "geom");
        assert_eq!(module_of("/tmp/sub/geom.hero"), "geom");
        assert_eq!(module_of("geom"), "geom");
    }

    /// The sanitising rule, and the residual it leaves — asserted rather than
    /// described, because the comment above claimed the opposite until M8a.
    #[test]
    fn the_component_is_alphanumeric_and_that_is_not_injective() {
        assert_eq!(module_of("print_inst.hero"), "printinst");
        assert_eq!(module_of("printinst.hero"), "printinst");
        assert_eq!(
            module_of("print_inst.hero"),
            module_of("printinst.hero"),
            "the collision is real, which is why a program containing both is refused"
        );
    }

    #[test]
    fn a_path_with_no_alphanumerics_still_becomes_a_name() {
        assert_eq!(module_of("---.hero"), "anon");
        assert_eq!(module_of(".hero"), "anon");
    }
}
