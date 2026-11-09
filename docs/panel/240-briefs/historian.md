# Panel 240: historian

Read `00-shared.md` beside this file first. You have no worktree; your tools are
the web and the repository's files to read.

**Your question is precedent, verified on the page.** For each, the URL and the
day you read it, `verified` only where the page itself says it:

1. **How do other build systems and languages spell "find this C library through
   pkg-config"** in the file or the source that asks for it? At least: Zig's
   `build.zig` (`linkSystemLibrary`, and whether it calls pkg-config), Cargo
   (`pkg-config` crate in `build.rs`, `system-deps` and its `[package.metadata.system-deps]`),
   Meson (`dependency()`), CMake (`pkg_check_modules`, `FindPkgConfig`), Nim
   (`{.passL: gorge("pkg-config ...").}`), Go's cgo (`#cgo pkg-config:`), Swift
   PM (`.systemLibrary(pkgConfig:)`), Haskell Cabal (`pkgconfig-depends:`),
   Odin, V, Crystal (`@[Link("...")]`, which calls pkg-config), and pkgconf's own
   naming (the tool Homebrew and vcpkg ship as `pkg-config`). Which word does
   each put in the program's text, and does any use the bare word *package* for
   it?
2. **Which ecosystems call a file a module and the fetched, versioned unit a
   package** (Python, npm/ES modules, Dart, Swift, Cargo's package and crate,
   Hex), and which call the versioned unit a module (Go). The coordinator's
   reading is that the first is the common one; verify or refute it.
3. **Has any language renamed a mark like this before 1.0**, and what did it do
   with old programs (an error naming the new spelling, an alias, a migration
   tool)?

No paid run. A precedent the page does not state is not written. Write your
report to `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-scratch-round/p240/reports/historian.md` as you go.
