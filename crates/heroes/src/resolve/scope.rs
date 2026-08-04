//! The scope stack: what is visible, what may not be redeclared, and what was
//! never read (§4.4 — "Shadowing is a compile error", "An unused variable is a
//! compile error").
//!
//! One flat vector of `(name, local)` plus a vector of marks saying where each
//! open scope begins. Closing a scope truncates. That is the whole structure —
//! no tree, no parent pointers, nothing the Cyclone rule forbids, and the port
//! to Heroes reads it as `[(str, int)]` and `[int]`.
//!
//! Four rules are decided here rather than in the walk, because all four are
//! about the *stack* and not about any one statement:
//!
//! - **`_` never binds.** `for _ in xs`, `.num _`, `_ = e`, and a parameter
//!   written `_`. It declares nothing, so it can neither shadow nor be unused —
//!   which makes it the escape valve for the unused rule, and the reason the two
//!   certain fixes here both rename to `_`. Every language that enforces an
//!   unused rule ships this valve (panel 015, historian: Zig, TypeScript,
//!   OCaml, Rust, Erlang, Elixir — six for six).
//! - **Shadowing looks at every open scope**, including the parameters, the top
//!   level and the built-ins, because "already in scope" is what §4.4 says and
//!   all three are. Two *sibling* blocks may each bind `x`: neither is in scope
//!   inside the other.
//! - **The check is sequential.** Only a binding already in scope conflicts, so
//!   a line below can never change whether the line above compiles. This is a
//!   deliberate departure from Java (JLS §6.4) and C# (CS0136), which make the
//!   same error order-*insensitive*: §1.3 says locality is the currency, and a
//!   legality rule that reads downwards spends it.
//! - **A write through an `@` parameter is a use.** §4.8's copy-out always
//!   happens, so `function reset(@counts: {str: int})` whose whole body is
//!   `counts @ {}` is a complete function, not an unused binding (panel 015,
//!   llm-ergonomist). Every other write is not a use: only a read is.

use crate::source::{Source, Span};
use crate::syntax::{Ast, ExprId, TypeId};

use super::builtins::index_of;
use super::errors;
use super::{Local, LocalKind, Resolver};

pub(super) struct Scopes {
    entries: Vec<(String, u32)>,
    /// Where each open scope begins in `entries`.
    marks: Vec<usize>,
}

/// What is being bound, as one value: its kind, whether it may be written, and
/// the syntax M3b will want back (see `Local`).
pub(super) struct Binding {
    kind: LocalKind,
    mutable: bool,
    ty: Option<TypeId>,
    value: Option<ExprId>,
}

impl Binding {
    /// A parameter. Its type is always written (§4.4), and `@` makes it in-out.
    pub(super) fn param(ty: TypeId, mutable: bool) -> Binding {
        Binding { kind: LocalKind::Param, mutable, ty: Some(ty), value: None }
    }

    /// `x = e`, or `xs: [int] = []` where the empty literal needs the
    /// annotation (§4.5).
    pub(super) fn bind(ty: Option<TypeId>, value: ExprId) -> Binding {
        Binding { kind: LocalKind::Bind, mutable: false, ty, value: Some(value) }
    }

    /// `v: int @ 0` — the type is mandatory, which is what makes the third line
    /// shape distinguishable from a mutation (§4.4).
    pub(super) fn cell(ty: TypeId, value: ExprId) -> Binding {
        Binding { kind: LocalKind::Cell, mutable: true, ty: Some(ty), value: Some(value) }
    }

    /// `for x in xs` — the element type comes from the iterable.
    pub(super) fn loop_var(iterable: ExprId) -> Binding {
        Binding { kind: LocalKind::Loop, mutable: false, ty: None, value: Some(iterable) }
    }

    /// `.num n` — the payload's type comes from the case the pattern names, so
    /// neither half is known until M3c.
    pub(super) fn payload() -> Binding {
        Binding { kind: LocalKind::Payload, mutable: false, ty: None, value: None }
    }
}

impl Scopes {
    pub(super) fn new() -> Scopes {
        Scopes { entries: Vec::new(), marks: Vec::new() }
    }

    pub(super) fn open(&mut self) {
        self.marks.push(self.entries.len());
    }

    pub(super) fn close(&mut self) {
        let mark = self.marks.pop().unwrap_or(0);
        self.entries.truncate(mark);
    }

    fn depth(&self) -> u32 {
        self.marks.len().saturating_sub(1) as u32
    }

    /// Innermost first — the answer to "which binding does this name mean", and
    /// the reason a sibling scope's `x` is invisible: it was truncated when its
    /// block closed.
    fn lookup(&self, name: &str) -> Option<u32> {
        self.entries.iter().rev().find(|(n, _)| n == name).map(|(_, index)| *index)
    }

    /// Whether the *innermost* scope already binds this name. One caller: two
    /// patterns of one arm may not bind the same payload name.
    fn in_current(&self, name: &str) -> bool {
        let mark = self.marks.last().copied().unwrap_or(0);
        self.entries[mark..].iter().any(|(n, _)| n == name)
    }
}

impl Resolver {
    pub(super) fn open_scope(&mut self) {
        self.scopes.open();
    }

    pub(super) fn close_scope(&mut self) {
        self.scopes.close();
    }

    pub(super) fn lookup_local(&self, name: &str) -> Option<u32> {
        self.scopes.lookup(name)
    }

    pub(super) fn bound_in_current_scope(&self, name: &str) -> bool {
        self.scopes.in_current(name)
    }

    /// Every name currently visible, innermost first — the did-you-mean's
    /// first and best candidates.
    pub(super) fn visible_locals(&self) -> Vec<String> {
        self.scopes.entries.iter().rev().map(|(name, _)| name.clone()).collect()
    }

    /// Binds `name` in the innermost scope and hands back its local index.
    /// `None` means nothing was bound — either the wildcard, or a name that was
    /// refused, and in both cases the walk carries on: a refused binding is one
    /// diagnostic, not a cascade.
    pub(super) fn declare(
        &mut self,
        ast: &Ast,
        src: &Source,
        name: Span,
        binding: Binding,
    ) -> Option<u32> {
        let Binding { kind, mutable, ty, value } = binding;
        let text = src.slice(name);
        if text == "_" {
            return None;
        }
        if let Some(previous) = self.scopes.lookup(text) {
            let at = self.out.locals[previous as usize].name;
            let (line, _) = src.line_col(at.start);
            let diagnostic = errors::shadowed(text, line, name);
            self.push_diagnostic(diagnostic);
            return None;
        }
        if let Some(&decl) = self.out.top.get(text) {
            let (line, _) = src.line_col(ast.decls[decl as usize].name.start);
            let diagnostic = errors::shadows_top_level(text, line, name);
            self.push_diagnostic(diagnostic);
            return None;
        }
        // A built-in's name is taken everywhere, not only at the top level: a
        // local `ok` makes `ok(x)` on the next line mean the local, which is
        // precisely the bug the shadow ban exists to kill.
        if index_of(text).is_some() {
            let diagnostic = errors::builtin_name_taken(text, name);
            self.push_diagnostic(diagnostic);
            return None;
        }
        let index = self.out.locals.len() as u32;
        self.out.locals.push(Local {
            name,
            kind,
            mutable,
            owner: self.owner,
            depth: self.scopes.depth(),
            ty,
            value,
            reads: 0,
            writes: 0,
        });
        self.scopes.entries.push((text.to_string(), index));
        Some(index)
    }

    /// §4.16's exemption is checked here, once, after the whole file has been
    /// walked: a hole at the bottom of the file suspends the rule at the top of
    /// it, so this cannot be decided while walking.
    pub(super) fn report_unused(&mut self, src: &Source) {
        if self.out.has_hole {
            return;
        }
        let mut diagnostics = Vec::new();
        for local in &self.out.locals {
            if local.reads > 0 {
                continue;
            }
            // The copy-out of an `@` parameter is observable by the caller
            // (§4.8), so writing one is using it.
            if local.kind == LocalKind::Param && local.mutable && local.writes > 0 {
                continue;
            }
            let name = src.slice(local.name);
            // Already spoken for: the compiler offered this name as the repair
            // for a name that resolved to nothing, and applying that repair
            // reads it. One typo, one diagnostic.
            if self.suggested.contains(name) {
                continue;
            }
            diagnostics.push(errors::unused(name, local.kind, local.writes, local.name));
        }
        for diagnostic in diagnostics {
            self.push_diagnostic(diagnostic);
        }
    }
}
