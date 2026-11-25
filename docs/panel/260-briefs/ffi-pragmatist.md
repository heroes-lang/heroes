# Panel 260, the ffi-pragmatist's brief

Read `docs/panel/260-briefs/00-shared.md` first, whole, then, if the coordinator's prompt says it exists, the critic's
first reading (`docs/panel/260-reports/critic-briefs.md`). Your seat judges **the C boundary** (design.md §1.11, §4.19,
reached by `grep -n`; `.claude/rules/c-boundary.md`): what a binding written in real C does, whether the author's
principle (`00-shared.md` § 1) holds against it, and what the archive and the link line become. You have a veto on ABI
breakage and on anything that makes a binding unsafe or refuses a correct program. You write and compile C.

**Your copy** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-ffi-pragmatist`, a detached worktree the
coordinator made at the base `00-shared.md` names, **`c3885b81b`** (re-frozen after the critic's first reading: step
182c's include walk, `00-shared.md` § 4.5, and step 185a's `runtime/parts/` moved since `80e2e39ad`); build its
compiler from the seed and then stage 2 there (§ 10 of the shared brief). **You hold this Mac, Linux arm64 and Linux x86-64**: `heroes-linux-arm64:latest` and `heroes-linux:latest`
(the second with `docker run --pull never` and no `--platform`), one container at a time, `--rm`, named
`p260-ffi-pragmatist`, removed when idle. Windows is a question unless the coordinator's prompt hands you the box.

## What you are asked to settle, in this order

1. **What *binds the layer* means in C, every door.** `00-shared.md` § 6.2 reads the candidate readings from the files
   and runs none: the group's header name, what each unit's opening reaches, a bound name a member's header declares,
   the names a group's header bodies call (today read only after a link fails, `cli/header_needs.hero`'s doc), the
   link's missing names, **step 182c's walk** (built and run before every build's first clang, every `#if` but `#if 0`
   read as true, `00-shared.md` § 4.5), and the program's own objects' undefined names by `nm` before the link. **The
   ten** tracked programs there bind only their own header's `static` functions, whose bodies call the layer
   (`ground/own-bound.txt` for nine; the tenth, `the-platform-layer-ends-a-wait-at-its-sockets-deadline.hero`, binds
   seven `deadline_*` functions through `the-platform-layer-deadline.h`, two includes from `hero_net.h`);
   `owndecl` declares `hero_time_monotonic_ns` by hand and includes nothing of the layer; the critic's `platformhdr`
   names `platform/hero_audio_arm.h`, no member's header, and binds a function `hero_audio_coreaudio.c` defines, and
   nothing in `selfhost/check` or `selfhost/cli` refuses it (`grep -rln 'platform/' selfhost` names only `layer*.hero`
   and `runtime_key.hero`); `constonly` names `hero_net.h` and binds only a constant, so the link takes no member.
   **Write the doors beside them and run each under the base's compiler** (where R6 builds them all): a macro of a
   header of the program's own that names a layer function; a pointer to a layer function taken in such a header and
   called from Heroes through a function value; a C library the program names with `link` whose own object calls a
   layer function; a layer function reached by `dlsym` at run time; a layer header included under a condition (`#if`)
   the unit's opening never takes (which the walk reads as taken, so a check resting on it would demand a declaration
   there); a member's header reached through an arm's header (`hero_audio_coreaudio.h`). For each: is it a correct
   program by §4.19 and by panel 234's R6 (*"a group binds a layer function only by naming the layer's header"*, R6's
   own words, `docs/panel/234-*.md:400-428`), which reading sees it, what an undeclared program built that way must be
   told under the principle (no fallback), and on which line. Keep them in
   `docs/panel/260-reports/programs/ffi-pragmatist/` of your copy, sources only.
   **And your seat's own earlier condition, which the principle reverses**: panel 234's R6 says every condition this
   seat set on P2 holds, among them *"found and linked by the compiler with no `link` word or flag in a package"*
   (`docs/panel/234-*.md:418-419`, `sed -n` at the base). The author's declaration is a word in a package. Say whether
   that condition still binds, what replaced its reason, and whether the `link` word the group already has
   (`extern "hero_time.h" link "<the layer>"`; spec § 13, *"`link` a library when the symbols need one"*, line 401)
   is itself the home, with no new grammar (`00-shared.md` § 7.1).
2. **What the linkers say when a layer name is missing**, the words the new diagnostic would read where only the link
   can tell: Apple's ld, GNU ld, lld and lld-link (or link.exe, unrun without the box), on each platform you hold, for an
   undeclared program whose link carries no archive. `selfhost/cli/link_report.hero` (359 lines) already reads their
   undefined-name lines (`references`, 77): say what it reads of these.
3. **The archive and the link line a declared build gets**, written out: R6 whole as today (the archive after every
   object, `selfhost/cli/assemble.hero:263`), and whichever route of `00-shared.md` § 7.5 a seat would adopt for a
   declared build. Measure, on each platform you hold, which members a declared link takes (Apple's `-map` and
   `-why_load`, GNU ld's and lld's `-t` and `-Map`, lld's `--why-extract`); `selfhost/cli/supplied_probe.hero:239` and
   `supplied_read.hero:184` and `213` already read these answers for a library the program names.
4. **The member-to-member calls, by `nm`**, which matter only to a declared build that pays less than R6 whole
   (`00-shared.md` § 6.1: 9 here, 107 in the arm64 image, each C member compiled alone at `-O0`). Re-run them from the
   archive a real build writes (`build/layer-*.a`), at `-O0`, `-O2` and `--sanitize`, and in the x86-64 image; group
   every call by the header it goes through; and look for a call `nm` cannot see (a function pointer, a weak symbol, a
   constructor, a library a member opens at run time, design.md §4.19's task 2593, a C member and a Swift member's C
   entry).
5. **A program per member family, declared and undeclared.** One small program per family of the layer's headers (41
   members have a header at `runtime/<member>.h`, `00-shared.md` § 3.1), binding one function of it and printing
   something of it; build each under the base's compiler and under the seats' built resolution, declared and not, plain,
   `-O2` and `--sanitize`, on this Mac and in the arm64 image: exit, the first line of stderr, and the members the binary
   took. A program R6 builds that the resolution refuses with no declaration missing is a refusal the sitting must see.
6. **The two defects found beside, under the principle** (`ground/d-lane-layer-speed-1.md`, a header one member reads
   stops every program's build; `d-lane-layer-speed-2.md`, a program binding `hero_ui.h` does not link on Linux arm64 at
   `-O0` or `-O2`, `nextafter` with no `-lm`): run each in the arm64 image under the base and under what the seats build.
   Which programs does the planted `cups.h` stop when only declared programs compile the layer, and who puts libm on a
   declared link for a member that needs it (`supplied_kept.alone_needs`, `selfhost/cli/supplied_kept.hero:225`)?
   **And two filed defects of the same class** (`00-shared.md` § 8): defect 2788 (closed 2026-11-24, blocking, *"On
   Windows every program fails to build: `hero_ui_parts_gtk.c` has no platform guard"*), R6's *one member that does
   not compile fails every program on that platform*, which the principle bounds to declared programs; and defect 2687
   (open, improvement, *"A macOS program that only digests with `crypto` links Security, CryptoKit, Foundation and the
   Swift runtime, about 3 ms a start, because the layer's crypto member is one object"*), what a declared build pays
   at run time, not only at build time.
7. **ABI and what a binding sees.** Does the declaration move `HERO_RUNTIME_ABI`, a `hero_*.h` a package compiles against
   (a program binding `hero_os.h` is the language's door and its functions' stability is promised, decision 2228,
   `docs/design.md:3794-3798`), or the archive's name or suffix (`selfhost/cli/archiver.hero:193`, `suffix_of`)? On
   Windows (MSVC), where the archive is a `.lib` and the CI's leg links with lld-link or link.exe, say what the principle
   needs and what is unrun. And from the C world's side, a question for the historian's sources rather than yours to
   assert: which C toolchains make a library declare what it links (pkg-config's `Libs.private`, libtool's `.la`
   `dependency_libs`), and what did that cost them? **The historian's brief asks it** (`historian.md`, question 10);
   read its report rather than asserting either.

## The report

`docs/panel/260-reports/ffi-pragmatist.md` of your copy, written as you go and copied to the trunk's same path when you
finish or stop: your verdict (approve, approve with a condition, object, veto), the section it rests on, the table of
doors (door, platform, correct or not, which reading sees it, what an undeclared program is told), the linkers' words,
the per-family table (family, platform, declared or not, plain, `-O2`, `--sanitize`, members taken), the member calls,
**what a veto would compel**, what the conservative resolution would have been, every place the shared brief was false
or silent, and one falsifiable prediction with the measurement at which it is checkable. Say what you ran and what you
read, apart.
