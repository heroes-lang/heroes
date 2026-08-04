//! The type table: what a type *is* once the checker has it, as opposed to how
//! it was written (design.md §4.3 the type table, §4.6 `T?`, §4.12 generics).
//!
//! **Types are interned, and interning is the whole design.** Every distinct
//! type gets exactly one `TyId`, so `a == b` on two `TyId`s *is* type equality —
//! no recursive comparison anywhere in the compiler, one place that decides what
//! "the same type" means, and a number that later passes can use as a key: the
//! descriptor pass (M5c) needs one `copy`/`drop`/`eq`/`hash` per reachable type,
//! and monomorphisation (M6) needs to know it has already seen `(int, int)`.
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

#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub enum Ty {
    Int,
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
    /// A parameter run → where it starts, so `(function(int) -> int)` written
    /// twice interns to one id rather than two runs of equal content.
    runs: BTreeMap<Vec<TyId>, u32>,
}

impl Default for Types {
    fn default() -> Types {
        Types::new()
    }
}

impl Types {
    pub fn new() -> Types {
        Types {
            nodes: Vec::new(),
            params: Vec::new(),
            index: BTreeMap::new(),
            runs: BTreeMap::new(),
        }
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
