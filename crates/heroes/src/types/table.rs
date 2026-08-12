//! The type table: what a type *is* once the checker has it, as opposed to how
//! it was written (design.md §4.3 the type table, §4.6 `T?`, §4.12 generics).
//!
//! **Types are interned, and interning is the whole design.** Every distinct
//! type gets exactly one `TyId`, so `a == b` on two `TyId`s *is* type equality —
//! no recursive comparison anywhere in the compiler, one place that decides what
//! "the same type" means, and a number that later passes can use as a key: the
//! descriptor pass (M-value-aggregates) needs one `copy`/`drop`/`eq`/`hash` per reachable type,
//! and monomorphisation (M-generics-library) needs to know it has already seen `(int, int)`.
//!
//! `Ty` is `Copy`, which is why a function type keeps its parameters in a
//! side arena (`Types::params`) as a run rather than a `Vec`: the Cyclone rule
//! (CLAUDE.md §5) allows references only as parameters, so a `get` that returned
//! `&Ty` could not exist, and a `Ty` that owned a `Vec` could not be copied out.
//! The port reads it as `[Ty]` and `[TyId]` — two arrays, which is Heroes' only
//! indirection (§4.10).

use std::collections::BTreeMap;

/// Index into `Types::nodes`. Equality of `TyId`s is equality of types.
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub struct TyId(pub u32);

/// A run of parameter types inside `Types::params`.
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub struct Params {
    pub start: u32,
    pub len: u32,
}

/// How wide an integer is, and whether it carries a sign (§4.3; panel 042,
/// author ratification 2026-08-12).
///
/// **This enum exists so that a width cannot be forgotten.** The alternative was
/// one `Ty` variant per width, and it was measured rather than argued: eight unit
/// variants produced **2** rustc errors and left **18** silent sites, one of them
/// `emit/ops.rs`'s `operands == Ty::Int`, which for a `u8` is simply `false` — so
/// the overflow check would be omitted and *"overflow aborts at every width"*
/// would be silently untrue at exit 0. With the width inside the variant, every
/// site that dispatches on it is an exhaustive `match` and a new width is a
/// compile error there (panel 042; CLAUDE.md §11's loud-fallback rule).
///
/// A site that does **not** care about the width writes `Ty::Int(_)`, and that is
/// a claim: it says this question has the same answer at every width. Where that
/// is false the arm must be exhaustive instead.
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub enum IntKind {
    I64,
}

impl IntKind {
    /// The surface spelling, which is also what a diagnostic prints.
    ///
    /// **There is no `i64`.** The author deleted it 2026-08-12 rather than
    /// aliasing it (panel 042's verdict section): one spelling per width, and
    /// `i64` is a foreign word with a `certain` fix to `i64`. The reason is the
    /// thesis rather than taste — `i64` is a word carrying forty years of
    /// conflicting widths, and a reader has to know the platform to know what it
    /// means, while `i64` is ambiguous to nobody.
    pub fn name(self) -> &'static str {
        match self {
            IntKind::I64 => "i64",
        }
    }

    /// The C type the emitter writes for it.
    pub fn c_type(self) -> &'static str {
        match self {
            IntKind::I64 => "int64_t",
        }
    }
}

#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub enum Ty {
    Int(IntKind),
    F64,
    Bool,
    Str,
    /// `ptr` — an opaque C pointer (§4.19).
    Ptr,
    /// `cstr` — a C string (§4.19).
    Cstr,
    /// `()` — no value. A function written without `->` returns it, and a
    /// statement's type must be it (§4.14, panel 003).
    Unit,
    Array(TyId),
    Map(TyId, TyId),
    /// `T?` — a `T` or an error (§4.6). Never nested: `T??` is rejected by the
    /// parser, so this node's argument is never itself a `Fallible`.
    Fallible(TyId),
    Func {
        params: Params,
        result: TyId,
    },
    /// A `record` or `variant`: index into `Ast::decls`. Two records with
    /// identical fields are **different types** — Heroes is nominal here, which
    /// is what makes `Point` and `Size` unconfusable.
    Named(u32),
    /// One *case* of a variant: `(declaration, case index)`. A case with fields
    /// is a small record (§4.2), and this is the type a pattern's payload
    /// binding has — `.num n` gives `n` this type. It cannot be written in
    /// surface syntax, and messages print it `Token.num`.
    Case(u32, u32),
    /// The error side of a `T?` (§4.6): two `str` fields, `code` and `msg`,
    /// built in. `.err e` binds `e` to this.
    Failure,
    /// A type parameter of the enclosing function, by position (§4.12).
    Generic(u32),
    /// A type the checker could not work out. Its diagnostic has already been
    /// reported, and every rule involving it stays silent — one mistake, one
    /// message.
    Error,
}

pub struct Types {
    nodes: Vec<Ty>,
    params: Vec<TyId>,
    /// Structure → id. This is what makes the ids canonical.
    index: BTreeMap<Ty, TyId>,
    /// A parameter run → where it starts, so `(function(int) -> i64)` written
    /// twice interns to one id rather than two runs of equal content.
    runs: BTreeMap<Vec<TyId>, u32>,
}

impl Default for Types {
    fn default() -> Types {
        Types::new()
    }
}

impl Types {
    /// The types this one holds, one level down — an array's element, a map's key
    /// and value, a fallible's payload, a function type's parameters and result.
    ///
    /// Exists for the emitter's reachability walk: a slot of type `[int?]` names
    /// two declarations, and a walk that stops at the outer one leaves the inner
    /// `T?` unnamed (`emit/ctype.rs` asserts that never happens).
    pub fn contained(&self, id: TyId) -> Vec<TyId> {
        match self.get(id) {
            Ty::Array(element) => vec![element],
            Ty::Fallible(payload) => vec![payload],
            Ty::Map(key, value) => vec![key, value],
            Ty::Func { params, result } => {
                let mut out: Vec<TyId> = self.params_of(params).to_vec();
                out.push(result);
                out
            }
            _ => Vec::new(),
        }
    }

    /// The scalars are interned first, in a fixed order, so their ids are
    /// constants the whole pass can name without a lookup — and `Error` is
    /// **id 0**, which is what lets `expr_types` be a dense array whose default
    /// entry already means "not typed, and something said why".
    pub fn new() -> Types {
        let mut types = Types {
            nodes: Vec::new(),
            params: Vec::new(),
            index: BTreeMap::new(),
            runs: BTreeMap::new(),
        };
        for ty in [
            Ty::Error,
            Ty::Unit,
            Ty::Int(IntKind::I64),
            Ty::F64,
            Ty::Bool,
            Ty::Str,
            Ty::Ptr,
            Ty::Cstr,
            Ty::Failure,
        ] {
            types.intern(ty);
        }
        types
    }

    pub fn error(&self) -> TyId {
        TyId(0)
    }

    pub fn unit(&self) -> TyId {
        TyId(1)
    }

    /// `i64`. The prelude's fixed order makes this a constant rather than a
    /// lookup, and `the_prelude_ids_match_their_accessors` is what keeps the
    /// constant honest — the numbers below are hardcoded against `new()`'s list,
    /// so a width inserted into the middle of it would silently re-point every
    /// one of them.
    pub fn int(&self) -> TyId {
        TyId(2)
    }

    pub fn f64(&self) -> TyId {
        TyId(3)
    }

    pub fn bool(&self) -> TyId {
        TyId(4)
    }

    pub fn str(&self) -> TyId {
        TyId(5)
    }

    pub fn failure(&self) -> TyId {
        TyId(8)
    }

    pub fn intern(&mut self, ty: Ty) -> TyId {
        if let Some(id) = self.index.get(&ty) {
            return *id;
        }
        let id = TyId(self.nodes.len() as u32);
        self.nodes.push(ty);
        self.index.insert(ty, id);
        id
    }

    /// Interns a function type, canonicalising its parameter run first.
    pub fn func(&mut self, params: &[TyId], result: TyId) -> TyId {
        let key = params.to_vec();
        let start = match self.runs.get(&key) {
            Some(start) => *start,
            None => {
                let start = self.params.len() as u32;
                self.params.extend_from_slice(params);
                self.runs.insert(key, start);
                start
            }
        };
        self.intern(Ty::Func {
            params: Params { start, len: params.len() as u32 },
            result,
        })
    }

    /// Whether a structure is already interned, without interning it. One
    /// caller: a rule that needs to *compare* against `[str]` and must not
    /// create the type as a side effect of asking.
    pub fn lookup(&self, ty: Ty) -> Option<TyId> {
        self.index.get(&ty).copied()
    }

    pub fn get(&self, id: TyId) -> Ty {
        self.nodes[id.0 as usize]
    }

    pub fn params_of(&self, params: Params) -> Vec<TyId> {
        let start = params.start as usize;
        self.params[start..start + params.len as usize].to_vec()
    }

    pub fn len(&self) -> usize {
        self.nodes.len()
    }

    pub fn is_empty(&self) -> bool {
        self.nodes.is_empty()
    }

    /// True where a type is, or contains, `Error`. The checker uses it to stay
    /// quiet: a mismatch between `Error` and anything is a mistake already
    /// reported somewhere else.
    pub fn poisoned(&self, id: TyId) -> bool {
        match self.get(id) {
            Ty::Error => true,
            Ty::Array(inner) | Ty::Fallible(inner) => self.poisoned(inner),
            Ty::Map(key, value) => self.poisoned(key) || self.poisoned(value),
            Ty::Func { params, result } => {
                self.poisoned(result)
                    || self.params_of(params).iter().any(|p| self.poisoned(*p))
            }
            _ => false,
        }
    }
}
