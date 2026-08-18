//! The eight integer widths, and what a width implies (spec § Types; design.md
//! §4.3; panel 042, author ratification 2026-08-12).
//!
//! Split out of `table.rs` by the §11 sweep. The seam is real rather than
//! dimensional: everything here is a fact about **one width** — how many bits, what
//! it is called, what values it holds, what C calls it — and none of it knows the
//! type table exists. `table.rs` is about how types are stored and compared.
//!
//! **This enum exists so that a width cannot be forgotten**, and that was measured
//! rather than argued. The alternative was one `Ty` variant per width: eight unit
//! variants produced **2** rustc errors and left **18** silent sites, one of them
//! `emit/operator.rs`'s `matches!(operands, Ty::Int(_))`, which under that shape is
//! simply `false` for a `u8` — so the overflow guard would be dropped and *"overflow
//! aborts at every width"* would be silently untrue at exit 0. With the width inside
//! the variant, every site that dispatches on it is an exhaustive `match` and a new
//! width is a compile error there (CLAUDE.md §11's loud-fallback rule).
//!
//! A site that does **not** care about the width writes `Ty::Int(_)`, and that is a
//! claim: it says this question has the same answer at every width. Where that is
//! false the arm must be exhaustive instead.

/// How wide an integer is, and whether it carries a sign. The argument for this
/// shape is the module doc's, and it is the file's reason to exist.
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub enum IntKind {
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
}

/// Every width, in declaration order, for the places that must enumerate them —
/// the prelude, the resolver's name table, and the did-you-mean list. One array
/// so those three cannot disagree about what exists.
pub const INT_KINDS: [IntKind; 8] = [
    IntKind::I8,
    IntKind::I16,
    IntKind::I32,
    IntKind::I64,
    IntKind::U8,
    IntKind::U16,
    IntKind::U32,
    IntKind::U64,
];

impl IntKind {
    /// The surface spelling, which is also what a diagnostic prints.
    ///
    /// **There is no `int`.** The author deleted it 2026-08-12 rather than
    /// aliasing it (panel 042's verdict section): one spelling per width, and
    /// `int` is a foreign word with a `certain` fix to `i64`. The reason is the
    /// thesis rather than taste — `int` is a word carrying forty years of
    /// conflicting widths, and a reader has to know the platform to know what it
    /// means, while `i64` is ambiguous to nobody. (This paragraph once said
    /// "there is no `i64`": a search-and-replace over the rename ate its own
    /// subject. Repaired 2026-08-17 against panel 042's option table.)
    pub fn name(self) -> &'static str {
        match self {
            IntKind::I8 => "i8",
            IntKind::I16 => "i16",
            IntKind::I32 => "i32",
            IntKind::I64 => "i64",
            IntKind::U8 => "u8",
            IntKind::U16 => "u16",
            IntKind::U32 => "u32",
            IntKind::U64 => "u64",
        }
    }

    /// `an i64`, `a u8`. The article follows how the name is *said* — the signed
    /// ones begin with a vowel sound and the unsigned ones do not — and it is
    /// derived from `signed()` rather than tabulated, so a ninth width gets it
    /// right without a second edit.
    pub fn a_name(self) -> String {
        format!("{} `{}`", if self.signed() { "an" } else { "a" }, self.name())
    }

    /// How many bits, and whether the top one is a sign. Together they are the
    /// whole of what a width *is*, and every question below is derived from them
    /// rather than tabulated again — a table per question is a table per chance
    /// to disagree.
    pub fn bits(self) -> u32 {
        match self {
            IntKind::I8 | IntKind::U8 => 8,
            IntKind::I16 | IntKind::U16 => 16,
            IntKind::I32 | IntKind::U32 => 32,
            IntKind::I64 | IntKind::U64 => 64,
        }
    }

    pub fn signed(self) -> bool {
        match self {
            IntKind::I8 | IntKind::I16 | IntKind::I32 | IntKind::I64 => true,
            IntKind::U8 | IntKind::U16 | IntKind::U32 | IntKind::U64 => false,
        }
    }

    /// Whether **every** value of `other` is a value of `self` — i.e. whether a
    /// conversion from `other` into `self` can fail.
    ///
    /// The four cases are not symmetric and the asymmetry is the point: an
    /// unsigned source needs a *strictly* wider signed target, because the target
    /// spends a bit on a sign it will never use; and a signed source never fits
    /// an unsigned target at all, however wide, because -1 is a value.
    pub fn contains(self, other: IntKind) -> bool {
        match (self.signed(), other.signed()) {
            (true, true) | (false, false) => self.bits() >= other.bits(),
            (true, false) => self.bits() > other.bits(),
            (false, true) => false,
        }
    }

    /// The closed range a literal must fall in. `u64`'s top is above `i64::MAX`,
    /// so the high end is a `u64` and the low end an `i64` — the one pair of
    /// numbers in this language that does not fit a single Rust integer either.
    pub fn range(self) -> (i64, u64) {
        if self.signed() {
            let top = (1u64 << (self.bits() - 1)) - 1;
            (-(top as i64) - 1, top)
        } else if self.bits() == 64 {
            (0, u64::MAX)
        } else {
            (0, (1u64 << self.bits()) - 1)
        }
    }

    /// The C type the emitter writes. Fixed-width by name, so the generated C
    /// says what the Heroes source says and clang checks the rest.
    pub fn c_type(self) -> &'static str {
        match self {
            IntKind::I8 => "int8_t",
            IntKind::I16 => "int16_t",
            IntKind::I32 => "int32_t",
            IntKind::I64 => "int64_t",
            IntKind::U8 => "uint8_t",
            IntKind::U16 => "uint16_t",
            IntKind::U32 => "uint32_t",
            IntKind::U64 => "uint64_t",
        }
    }
}
