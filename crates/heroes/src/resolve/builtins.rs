//! The names no file may declare — design.md §4.20's consolidated inventory,
//! which is also Principle 0's library closure list (§1.0), split by §1.11's
//! two tiers.
//!
//! **The tier says where the implementation comes from, not whether the name is
//! taken.** Every name here is reserved everywhere: at the top level, as a
//! parameter, as a local. Panel 015 put a second tier to the judges — the six
//! functions the spec says are "written in Heroes" would have been declarable,
//! with a file's own definition winning silently — and two judges rejected it
//! from opposite directions: the llm-ergonomist vetoed it on locality (the
//! meaning of `xs.map(f)` would depend on a distant line the reader cannot
//! see), the spec-warden on Principle 0 (it is an *exception* to a rule the
//! spec already states, and exceptions are what cost tokens). One tier, one
//! rule, zero spec tokens.
//!
//! Two names in the inventory are deliberately **absent**, and their absence is
//! a rejection that can be relaxed later rather than an acceptance that cannot
//! be withdrawn:
//!
//! - **`panic`** — Part 5 desugars `assert` into `if` plus `panic`, so the
//!   runtime has it; the spec offers the user `assert`. A user-callable
//!   `panic` is a language addition and waits for a panel.
//! - **`Builder`** — a runtime type behind `join` (§4.20), not surface.
//!
//! `fail` is here because `fail("code", "msg")` has to resolve to something,
//! but its *reservation* is enforced one stage earlier: it is a lexer keyword
//! (`lexer/keywords.rs`), so a declaration named `fail` never reaches the
//! resolver. Rule in one place — the diagnostic a user sees says "keyword"
//! because the lexer is the one that says it (ffi-pragmatist, panel 015).

/// Where a built-in's implementation lives (§1.11). Not a permission level:
/// both tiers are equally reserved.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Tier {
    /// Tier 1 — the C runtime. The language cannot express these: `print`
    /// takes any number of arguments of four types and renders them
    /// canonically (panel 006), and the emitter recognises it by name.
    Runtime,
    /// Tier 2 — written in Heroes itself, and the payoff from generics
    /// (§4.12). They arrive as real source at M-generics-library, in one prelude; a *user*
    /// file still may not redeclare them.
    Heroes,
}

pub struct Builtin {
    pub name: &'static str,
    pub tier: Tier,
}

/// Sorted by name, searched linearly. Thirty-six entries need no index, and a
/// sorted table prints in a deterministic order — §4.16's rule for hole
/// suggestions, applied to every list the compiler shows.
pub const BUILTINS: [Builtin; 36] = [
    Builtin { name: "all", tier: Tier::Heroes },
    Builtin { name: "args", tier: Tier::Heroes },
    Builtin { name: "any", tier: Tier::Heroes },
    Builtin { name: "chars", tier: Tier::Runtime },
    Builtin { name: "cstr", tier: Tier::Runtime },
    Builtin { name: "default", tier: Tier::Heroes },
    Builtin { name: "exit", tier: Tier::Heroes },
    Builtin { name: "fail", tier: Tier::Runtime },
    Builtin { name: "filter", tier: Tier::Heroes },
    // **One `fit_<width>` per width, and they are not `to_<width>`** (panel 042
    // Q2, author ruling 3). `to_i64` and `to_f64` are the number-KIND pair and
    // they abort; these are the width pair and they return `T?`, because a value
    // may simply not fit. Two names rather than one overloaded name is what every
    // language that has both does — Swift added a label (`exactly:`), Rust added
    // a name (`try_into`), and neither changed the incumbent's return type.
    //
    // The name says the question: `fit_u8(x)` asks whether `x` fits in a `u8`.
    //
    // **A widening returns `T`, not `T?`** (step 7, pending author ratification;
    // `docs/panel/043`). This comment asserted the opposite for one step, having
    // been written at step 6 and left behind when step 7 changed the rule — which
    // is CLAUDE.md §11's named failure mode, committed inside the milestone that
    // found three instances of it elsewhere. It also credited the ground to panel
    // 042 Q2, and panel 043's compiler-engineer — who cast that veto — records
    // that 042 never ruled on a widening at all: its Q2 chose among *names*. The
    // ruling being bent is the author's, and only the author's.
    Builtin { name: "fit_i16", tier: Tier::Runtime },
    Builtin { name: "fit_i32", tier: Tier::Runtime },
    Builtin { name: "fit_i64", tier: Tier::Runtime },
    Builtin { name: "fit_i8", tier: Tier::Runtime },
    Builtin { name: "fit_u16", tier: Tier::Runtime },
    Builtin { name: "fit_u32", tier: Tier::Runtime },
    Builtin { name: "fit_u64", tier: Tier::Runtime },
    Builtin { name: "fit_u8", tier: Tier::Runtime },
    Builtin { name: "find", tier: Tier::Heroes },
    Builtin { name: "fold", tier: Tier::Heroes },
    Builtin { name: "keys", tier: Tier::Runtime },
    Builtin { name: "is_err", tier: Tier::Heroes },
    Builtin { name: "join", tier: Tier::Runtime },
    Builtin { name: "len", tier: Tier::Runtime },
    Builtin { name: "map", tier: Tier::Heroes },
    Builtin { name: "must", tier: Tier::Heroes },
    Builtin { name: "ok", tier: Tier::Runtime },
    Builtin { name: "print", tier: Tier::Runtime },
    Builtin { name: "push", tier: Tier::Runtime },
    Builtin { name: "range", tier: Tier::Heroes },
    Builtin { name: "read_file", tier: Tier::Heroes },
    Builtin { name: "slice", tier: Tier::Runtime },
    Builtin { name: "sort", tier: Tier::Runtime },
    Builtin { name: "to_f64", tier: Tier::Runtime },
    Builtin { name: "to_i64", tier: Tier::Runtime },
    Builtin { name: "to_str", tier: Tier::Runtime },
    Builtin { name: "write_file", tier: Tier::Heroes },
];

pub fn index_of(name: &str) -> Option<u32> {
    BUILTINS.iter().position(|b| b.name == name).map(|i| i as u32)
}
