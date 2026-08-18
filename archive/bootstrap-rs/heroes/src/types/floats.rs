//! The two float widths, and what a width implies (spec § Types; design.md §4.3;
//! panel 060, **author instruction 2026-08-15** overturning that sitting's own
//! deferral: *"e aggiungi il tipo f32 basta storie"*).
//!
//! The sibling of `widths.rs`, deliberately and structurally. That file's module
//! doc records the experiment this one inherits rather than repeats: eight unit
//! variants for the integer widths produced **2** rustc errors and left **18**
//! silent sites, one of them an `emit/operator.rs` `matches!` that would have
//! dropped the overflow guard for a `u8` at exit 0. Panel 060's compiler-engineer
//! measured the identical number for a bare `Ty::F32` — **2 errors, 19 silent
//! sites across 15 files** — which is the same trap arriving from the other
//! direction, and named the same answer: **the variant carries the width**.
//!
//! So there is no `Ty::F32`. There is `Ty::Float(FloatKind)`, every site that
//! dispatches on the width is an exhaustive `match`, and a site writing
//! `Ty::Float(_)` is making a claim — *this question has the same answer at both
//! widths* — rather than forgetting one.
//!
//! **Why two widths at all, when §4.3 wanted one.** `f32` was struck by panel 052
//! and struck again by panel 060, both times correctly on their own evidence: a C
//! `float` *parameter* bound `f64` is bit-exact, because clang converts against the
//! real prototype. What neither sitting could fund was the case with no prototype
//! in it — a `float` **field** of a struct crossing by value, where 060's warden
//! measured a `record Vector2 { x: f64, y: f64 }` over the header's `Vector2`
//! compiling at exit 0 with every `sizeof` assertion green. The author's
//! instruction is what decided it; the shape is what the two sittings had already
//! measured.

/// How wide a float is. Two variants and no sign question — IEEE-754 binary32 and
/// binary64 — which is why this enum is smaller than `IntKind` and still exists for
/// exactly the same reason.
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub enum FloatKind {
    F32,
    F64,
}

/// Both widths, in declaration order, for the places that must enumerate them —
/// the prelude, the resolver's name table, and the did-you-mean list. One array so
/// those three cannot disagree about what exists, exactly as `INT_KINDS` does.
pub const FLOAT_KINDS: [FloatKind; 2] = [FloatKind::F32, FloatKind::F64];

impl FloatKind {
    /// The surface spelling, which is also what a diagnostic prints.
    pub fn name(self) -> &'static str {
        match self {
            FloatKind::F32 => "f32",
            FloatKind::F64 => "f64",
        }
    }

    /// `an f32`, `an f64` — both begin with a vowel sound, but the article is
    /// still derived rather than written twice, so a third width cannot get it
    /// wrong by being added in one place only.
    pub fn a_name(self) -> String {
        format!("an `{}`", self.name())
    }

    pub fn bits(self) -> u32 {
        match self {
            FloatKind::F32 => 32,
            FloatKind::F64 => 64,
        }
    }

    /// Whether **every** value of `other` is a value of `self` — i.e. whether a
    /// conversion from `other` into `self` can lose something.
    ///
    /// `f64` contains `f32` exactly: binary32's 24-bit significand and 8-bit
    /// exponent both fit binary64's 53 and 11, so every `f32` — including every
    /// subnormal, both infinities and every NaN payload — round-trips. The
    /// converse does not, which is why `to_f32` is the direction that rounds.
    pub fn contains(self, other: FloatKind) -> bool {
        self.bits() >= other.bits()
    }

    /// The C type the emitter writes.
    ///
    /// **`float` and `double`, never `_Float32`/`_Float64`.** The FFI's whole
    /// guarantee is that clang checks a declaration against the header the author
    /// named (§4.19), and a header says `float`. The C23 interchange names are a
    /// different type to `_Generic`, so a probe written in them would refuse every
    /// correct binding in existence.
    pub fn c_type(self) -> &'static str {
        match self {
            FloatKind::F32 => "float",
            FloatKind::F64 => "double",
        }
    }

    /// The suffix a C literal of this width needs.
    ///
    /// **This is not cosmetic and it is not about precision.** An unsuffixed C
    /// floating constant has type `double`, so `float x = 0.1;` computes in
    /// `double` and narrows — which `-Wshorten-64-to-32` does not catch (it is an
    /// integer warning) but `-Wconversion` would, and which changes the value the
    /// program stores. Writing `0.1f` makes the constant the width the slot is.
    pub fn literal_suffix(self) -> &'static str {
        match self {
            FloatKind::F32 => "f",
            FloatKind::F64 => "",
        }
    }
}
