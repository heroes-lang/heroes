# Panel 250, historian

Written 2026-11-14 and 2026-11-15 by the historian seat, advisory, no veto, under
`docs/panel/250-briefs/historian.md` and `00-shared.md` (the plain `.md` files; the
`.before-critic.md` copies not read), in the copy at `.claude/worktrees/p250`, base `48f53b343`.

**The instrument, said once.** Every precedent below is a page fetched in this session, its URL beside
it, and says only what that page states. The session crossed midnight: the pages of § 1 to § 3, and
dep's documentation pages of § 5, were fetched on **2026-11-14**; dep's v0.5.0 announcement and
CHANGELOG were fetched at about midnight and are dated **2026-11-14/15**, since the tool gives no clock
per fetch; every page after them (Go's `cmd/vendor/modules.txt`, Zig, Python, npm, Bazel, Rust's
bootstrap, Deno's std post and migration guide, and the Go and Cargo issues of § 1, § 8 and § 10 not
dated 2026-11-14) was fetched on **2026-11-15**, each line saying which. The fetching tool renders a
page through a model and returns the passages asked for: a passage in quotation marks is what that
fetch returned as the page's own words, a sentence outside quotation marks is this seat's, and where
the fetch paraphrased the line says *as the reader rendered it*. A page that could not be read is
`unverified` and nothing rests on it. No `go`, `cargo`, `deno`, `npm`, `pip`, `uv` or `zig` was run:
every behaviour is as documented, not as measured. This seat has no shell, so no `grep` of the tree was
run by it; the tree was read with the file reader alone. No build, no paid call, no GitHub write.
*Heroes of code* was not consulted, neither as route nor as source. On a GitHub issue, a commenter is
named as the person the page names; *Member* is written only where the reader returned that badge.

Status: **complete**, 2026-11-15.

---

## Where the briefs say more than their pages, or less, first

1. **The route this seat's reading of precedent favours changes what a program writes, and a message a
   reader acts on**, in 00-shared.md's words: placing the reached Go packages (G, or M where they
   coincide) changes what lands under `heroes.packages/`, and the two messages 00-shared.md quotes
   (*"a file of it was changed, added or taken out since it was placed"*, and `fetch`'s *"`net.hero`
   taken out and 17 more"*) give advice 00-shared.md already says such a route makes false; which words
   replace them is the seats', not a precedent's. On the precedents read, placement needs no catalogue
   field, no `use` line and no sentence of a language's reference (§ 8); whether Heroes' own route does
   is the spec-warden's.
2. **Go's vendor copy is not checked at build, and Go's reference says so in its own words.** Panel
   242's historian could only infer it (*"that it does not is an inference from that silence, unrun"*,
   `docs/panel/242-reports/historian.md:135-136`), and 00-shared.md's F4 carries that inference. The
   `go mod vendor` section of https://go.dev/ref/mod (offset 100000, read 2026-11-14), which 242's
   reading did not quote: *"Note that `go mod vendor` removes the `vendor` directory if it exists before
   re-constructing it. Local changes should not be made to vendored packages. The `go` command does not
   check that packages in the `vendor` directory have not been modified, but one can verify the
   integrity of the `vendor` directory by running `go mod vendor` and checking that no changes were
   made."* F4's reading of Go is now a page. **verified**.
3. **Go's `go mod vendor` places the reached grain, by Go package, not whole modules** (§ 1). The
   brief's question is answered: packages. 00-shared.md § Before whole placement asks whether the
   project would return to the pre-235 grain; that grain is the one Go's vendor folder has kept.
   **verified**.
4. **Deno's vendor copy is checked only when copied in**, by Deno's own pull request (§ 3), which the
   brief did not ask. **verified**.
5. **The one system read that places a subset by reach, hashes the placed subset and checks it on a
   command is dep**, Go's official experiment before modules, absent from the brief's list (§ 5).
   **verified**.
6. **No JSR page read says a client fetches only what it imports.** JSR's API page has the version
   metadata's *"list of files in the package version"* and a module graph *"to allow for less waterfall
   when downloading modules"*, and *"no sentence saying clients download only the modules they need"*
   (the reader, https://jsr.io/docs/api, read 2026-11-14). The sentence that says it is Deno's, in a
   blog post by its author (§ 3).
7. **Cargo's `.cargo-checksum.json` does not stand beside the manifest** as route 4's parenthesis puts
   it (*"a listing or per-file hashes kept beside the manifest rather than in the tree (route 4:
   Cargo's `.cargo-checksum.json`, ...)"*): it is written inside each vendored crate's folder, beside
   that crate's `Cargo.toml` (§ 2), so it is the in-tree shape, F1's. The listing kept beside the
   program's own manifest is dep's, in `Gopkg.lock` (§ 5).
8. **The coordinator's list asks whether `--require-hashes` and `uv.lock` hash "the archive?"**: pip's
   page does not say *archive* in the sentences read, and what `uv.lock` hashes was not reached on a
   primary page (§ 4): both `unverified` as stated.

---

## verdict

**approve** (advisory; no veto), **of a program placing the reached grain, by Go package**, with four
readings of precedent the synthesis should weigh:

- **Grain.** Placing only the reached packages is Go's vendor model and was dep's; Go's cost of record
  is C code in a directory that is not a package, dropped by the copy (golang/go#26366), answered by a
  layout rule, not a computed C closure. That reads for **G or H over M where they part**.
- **Fingerprint.** The closest precedent to **F1** is JSR with Deno's lock: one hash per version over a
  listing of every file's hash, each fetched file compared to its line. Go's maintainer said `go.sum`
  would not take per-package sums (**F2**'s shape); dep's digest over the pruned tree is **F3/route
  7**'s shape and needed an escape hatch (`noverify`).
- **When checked.** **R7's check at every build has no precedent among the subset-placing systems
  read**: Go and Deno say in their own words they do not re-check the placed copy, dep checked on a
  command. Keeping R7 under M is a departure; no page records it failing either.
- **The reader.** Go's vendor mode, by its source, *"cannot use the network or module cache"* and
  answers an import it cannot resolve with *"import lookup disabled by -mod=%s"*; that such an import
  fails until `go mod vendor` runs again is this seat's reading, as is the parallel with 00-shared.md's
  refused `use <core>/strconv`. Deno writes its vendor folder when the program runs, which R8 forbids.

---

## precedents

### 1. Go: `go mod vendor` places the reached Go packages; the sum covers the whole module; nothing checks the copy at build

- *verified*, **the grain**, https://go.dev/ref/mod, § Vendoring (offset 0, read 2026-11-14): *"The go
  mod vendor command constructs a directory named `vendor` in the main module's root directory
  containing copies of all packages needed to build and test packages in the main module. Packages that
  are only imported by tests of packages outside the main module are not included. As with go mod tidy
  and other module commands, build constraints except for `ignore` are not considered when constructing
  the `vendor` directory."* And: *"Vendoring may be used to allow interoperation with older versions of
  Go, or to ensure that all files used for a build are stored in a single file tree."*
- *verified*, **the listing**, same section: *"`go mod vendor` also creates the file
  `vendor/modules.txt` that contains a list of vendored packages and the module versions they were
  copied from."* *"When the `go` command reads `vendor/modules.txt`, it checks that the module versions
  are consistent with `go.mod`. If `go.mod` has changed since `vendor/modules.txt` was generated, the
  `go` command will report an error."* *"If the `vendor` directory is present in the main module's root
  directory, it will be used automatically if the go version in the main module's go.mod file is `1.14`
  or higher."* What `modules.txt` holds, from the command's source
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/modcmd/vendor.go, read
  2026-11-14): a line `"# "` with the module path and version, annotation lines `"## explicit; go
  %s\n"`, `"## explicit\n"`, `"## go %s\n"`, `"## workspace\n"`, and one line per package,
  `fmt.Fprintf(w, "%s\n", pkg)`. No hash line was among those returned (a statement of the lines quoted,
  not of every byte of the file).
- *verified*, **test files and nested manifests left out**, the same source's help: *"Vendor resets the
  main module's vendor directory to include all packages"* *"needed to build and test all the main
  module's packages."* *"It does not include test code for vendored packages."*; code
  `strings.HasSuffix(info.Name(), "_test.go")` returning false; *"As of Go 1.17, we strip go.mod and
  go.sum files from dependency modules."* *"Otherwise, 'go' commands invoked within the vendor subtree
  may misidentify"* *"an arbitrary directory within the vendor tree as a module root."* (golang/go#42970
  is named there, not read). The reference's § `go` directive (offset 0): at `go 1.17` or higher *"`go
  mod vendor` omits `go.mod` and `go.sum` files for vendored dependencies"*.
- *verified*, **which files of a package's directory are copied**, the same source: `copyDir` *"copies
  all regular files satisfying match(info) from src to dst"*, skipping `file.IsDir()`, so a package's
  subdirectory is copied only where it is itself a vendored package; of a file the command cannot
  classify: *"We don't know anything about this file, so optimistically assume that it is"* *"needed."*;
  files tagged `ignore` are left out; metadata files (`"AUTHORS", "CONTRIBUTORS", "COPYLEFT", "COPYING",
  "COPYRIGHT", "LEGAL", "LICENSE", "NOTICE", "PATENTS"`) are copied *"from parents of copied
  directories"*. That a `.c` or `.h` file **beside** a package's Go files is kept is the reader's reading
  of `matchPotentialSourceFile`, not a sentence of the file.
- *verified*, **why the copy holds only that**, golang/go#27337, *"cmd/go: go mod vendor should ignore
  README, CONTRIBUTING and any other non metadata file is part of a subdirectory of the module"*, opened
  by didrocks Aug 29, 2018, closed by rsc Oct 25, 2018: *"The files that are copied now are the ones
  that may be legally required to copy the code"* *"(basically, copyright notices and supporting
  files)."* *"There is no attempt to make the vendored copy useful in any other way, such as by copying
  READMEs etc."* *"The assumption is that people who want to look at the code will look at an original
  copy, not the trimmed copy in a vendor directory."* *"I think the copy set should be left alone."*
  (https://github.com/golang/go/issues/27337, read 2026-11-15).
- *verified*, **what that grain cost, and the ruling that kept it**, golang/go#26366, *"cmd/go: go mod
  -vendor prunes non-package directories"*, opened by ainar-g Jul 13, 2018: *"The problem is that `go
  mod -vendor` pruned the `external/aacenc` folder,"* *"which contains the C code needed to build this
  package."*, error *"fatal error: voAAC.h: No such file or directory"*. bcmills, Jul 13, 2018: *"The
  cause here seems to be that we prune out directories that are not Go packages."* rsc, Jul 17, 2018:
  *"Sorry, but the model for go builds is that what's needed to build a package is in that directory,
  not subdirectories."* *"The C code needs to be moved up to the package directory to make it build
  properly, not to mention vendor properly."* Closed as completed by rsc Jul 17, 2018, no milestone
  (https://github.com/golang/go/issues/26366, read 2026-11-14). The flag asked for later,
  golang/go#29102, vcaesar, December 5, 2018: *"`go mod vendor` command can not copy c and other files
  to the vendor directory"* *"this makes a lot of packages have problems, maybe can add `go mod vendor
  -a`."*; closed the same day by myitcv, *"this is a dup of #26366"*, with no comment on the flag
  (https://github.com/golang/go/issues/29102, read 2026-11-14). golang/go#43317 and #29479, named in a
  search summary, were not read and are not cited.
- *verified*, **what the sum covers**: the whole module. *"the hash is for the files within the
  module's `.zip` file"* (the survey, `235-reports/historian-survey.md:73`, read 2026-11-07, not
  re-read). `go mod verify` *"hashes each downloaded module `.zip` file and extracted directory"* in the
  **module cache** and *"does not use `go.sum` files to verify module content"* (https://go.dev/ref/mod,
  offset 100000, read 2026-11-14).
- *verified*, **what checks the vendored copy, and when**: nothing at build (§ Where the briefs ...,
  item 2). Why, in the accepted proposal that made vendoring automatic, golang/go#33848, *"cmd/go:
  automatically check and use vendored packages"*, opened by bcmills Aug 26, 2019, milestone Go1.14,
  labels *Proposal-Accepted*: *"Verify that the set of modules annotated as `explicit` in
  `vendor/modules.txt` is exactly equal"* to *"the set of modules (and versions) found in `require`
  directives"*; *"The above checks _do not_ detect intentional modifications to the vendored sources."*,
  the checks being cheap, *"which in the current proposal only requires us to read the `go.mod` and
  `vendor/modules.txt` files"*; for contents, users *"will need to re-run `go mod vendor` and look for
  diffs"*; rsc, *"we should accept this proposal for Go 1.14"* (https://github.com/golang/go/issues/33848,
  read 2026-11-15; fragments as the reader returned them).
- *verified*, **the per-package sum Go has not added**, golang/go#27348, *"cmd/go: allow verifying
  vendored code"*, opened by krancour Aug 29, 2018, **open**, milestone Backlog: *"It would be great if
  projects that choose to vendor their modules"* *"had a similar command to verify the integrity of
  modules in that directory."* bcmills, Feb 28, 2022: *"`go mod verify` cannot be implemented for
  `vendor` trees in a way that is independent of the module cache."*; Mar 2, 2022: *"In theory you could
  checksum each package, but the `go.sum` file"* *"only contains checksums for modules and I don't see
  that changing."* qmuntal (*Member*), May 17, 2023: *"what if `go mod vendor` creates a `vendor.sum`
  file next to `modules.txt`"*; bcmills, May 24, 2023: *"I guess a `vendor.sum` could help defend
  against stray edits, at least."*; May 30, 2023: *"Then check the `vendor` directory for diffs (to make
  sure it matches what was downloaded)."* (https://github.com/golang/go/issues/27348, read 2026-11-15).
  A request for `go mod vendor -check`, golang/go#43706, Jan 14, 2021, was closed the next day by its
  opener, *"#27348 pretty much covered it"* (https://github.com/golang/go/issues/43706, read
  2026-11-15).
- *verified*, **what a build says when the program imports what the copy lacks**, the `go` command's
  source (https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/modload/import.go, read
  2026-11-15): *"In vendor mode, we cannot use the network or module cache, so we"* *"shouldn't try to
  look up the module"*; the error *"import lookup disabled by -mod=%s"* and *"import lookup disabled by
  -mod=%s\n\t(%s)"* (what the parenthesis holds was not returned); and of a directory added by hand,
  *"go: ignoring package %s which exists in the vendor directory but is missing from
  vendor/modules.txt."* *"To sync the vendor directory run go %s vendor."*, with the comment *"Go 1.23
  and later require vendored packages to be present in modules.txt to be imported."*. So a package
  present in the copy and absent from its listing is ignored and told, and from Go 1.23 not importable;
  that a newly written import fails the build until `go mod vendor` runs again is this seat's reading of
  *"cannot use the network or module cache"*, not a sentence returned.
- *verified*, **the vocabulary**: *"A module is a collection of packages that are released, versioned,
  and distributed together."* *"Each package within a module is a collection of source files in the same
  directory that are compiled together."* (the survey, `historian-survey.md:200`, read 2026-11-07, not
  re-read). Go's *module* is the unit of version and of `go.sum`; its *package*, a directory, the unit
  of compilation, import and vendoring. 00-shared.md's *entry* maps onto the first, its *Go package*
  onto the second.

### 2. Cargo: `cargo vendor` copies each crate whole, with a per-file listing inside its folder; a build command checks the listed files

- *verified*, **the grain is the crate, whole**: Cargo's vendor code
  (https://doc.rust-lang.org/nightly/nightly-rustc/src/cargo/ops/cargo_vendor.rs.html, read
  2026-11-14) walks a registry crate's unpacked directory, every regular file or symlink, and a path or
  git package through `PathSource::list_files`; its filter `vendor_this` leaves out only `.git`,
  `.gitattributes`, `.gitignore` and `.cargo-ok`: *"Skip git config files as they're not relevant to
  builds most of"* *"the time and if we respect them (e.g. in git) then it'll"* *"probably mess with the
  checksums when a vendor dir is checked"* *"into someone else's source control"*. The command's page:
  *"This cargo subcommand will vendor all crates.io and git dependencies for a project into the
  specified directory at `<path>`."* *"Cargo treats vendored sources as read-only as it does to registry
  and git sources. If you intend to modify a crate from a remote source, use `[patch]` or a `path`
  dependency pointing to a local copy of that crate."* No option to vendor fewer files of a crate is
  listed (https://doc.rust-lang.org/cargo/commands/cargo-vendor.html, read 2026-11-14).
- *verified*, **the listing**: the same code writes `.cargo-checksum.json` per crate:
  `"$comment": "This file only protects against accidental modifications. It is not a security mechanism
  and does not protect against malicious changes."`, `"package": checksums.get(id)` (the lock's checksum
  of the `.crate` archive, possibly null) and `"files": file_cksums` (each file's path relative to the
  crate's root, mapped to its SHA-256). The struct
  (https://doc.rust-lang.org/nightly/nightly-rustc/src/cargo/sources/directory.rs.html, read
  2026-11-14): *"Checksum of the package. Normally it is computed from the `.crate` file."* *"Checksums of
  each source file."*; and of the folder, *"There must be a [`Checksum`] file `.cargo-checksum.json` file
  at the same level of `Cargo.toml`"*.
- *verified*, **what is checked**: `verify` (same file) loops over `cksum.files` alone; a listed file
  missing fails *"failed to calculate checksum of: {}"*; a listed file changed fails *"the listed
  checksum of `{}` has changed:\nexpected: {}\nactual:   {}\n\ndirectory sources are not intended to be
  edited, if modifications are required then it is recommended that `[patch]` is used with a forked copy
  of the source"*. **A file present and not listed is not checked** (the loop, as the reader read it; no
  comment states it); an empty `files` map checks nothing; a missing or unparsable checksum file
  *"results in a denial of loading any package in this source"*.
- *verified*, **when**: at a build command. Cargo's test `bad_file_checksum` vendors a crate, overwrites
  its `src/lib.rs`, runs `cargo check`, and expects exit 101 with *"[ERROR] the listed checksum of
  `[ROOT]/index/bar/src/lib.rs` has changed:"* (https://rust.googlesource.com/cargo/+/HEAD/tests/testsuite/directory.rs,
  read 2026-11-14). No test there builds, edits and builds again, so **that the check runs at every
  build is not on a page read**: a question, unrun.
- *verified*, **what Cargo says the check is**: *"Each crate in a directory source also has an associated
  metadata file `.cargo-checksum.json` to protect against accidental modifications. It is not a security
  mechanism and does not protect against malicious changes."*; vendored sources *"are subsets of the
  source that they're replacing"*, subsets of crates (https://doc.rust-lang.org/cargo/reference/source-replacement.html,
  read 2026-11-14; the first two sentences were 242's too, its lines 235-241).
- *verified*, **who chooses a crate's files**: the publisher, by `include` and `exclude`, *"Files to
  include when publishing."*, *"Files to exclude when publishing."* (the survey's Table 1B,
  `historian-survey.md:135-136`, read 2026-11-07, not re-read).
- *verified*, **the consumer asking to vendor less**, § 8 below: four requests, three open with
  *S-needs-design*, one closed *not planned*, and a refusal to compute reach from the source.

### 3. Deno and JSR: files served one by one, a listing per version, one lock hash per version, and a vendor copy checked only on the way in

- *verified*, **per-file serving and a per-version listing**: *"Version metadata is served as JSON at the
  following URL:"* `https://jsr.io/@<scope>/<package-name>/<version>_meta.json`; it *"contains
  information about the requested version, such as the list of files in the package version"*, with
  *"the `exports` field, and a `moduleGraph1` or `moduleGraph2` field that contains information about the
  module graph"*, *"to allow for less waterfall when downloading modules"*; *"Modules are served using
  standard HTTP GET requests as would be initiated by a browser `import` statement."*, at
  `https://jsr.io/@<scope>/<package-name>/<version>/<path>`; *"Modules are immutable, so they can be
  cached indefinitely."*; *"The version metadata field is immutable, so it can be cached indefinitely."*
  Each file's `size` and `checksum` appear only in the page's example JSON (https://jsr.io/docs/api,
  read 2026-11-14; the survey's Table 3 quoted the example, `historian-survey.md:618`).
- *verified*, **the lock's hash for a JSR package, and when files are compared**, denoland/deno#22421,
  *"feat(unstable): single checksum per JSR package in the lockfile"*, by dsherret, merged February 15,
  2024: *"This changes the lockfile to not store JSR specifiers in the "remote" section."* *"Instead a
  single JSR integrity is stored per package in the lockfile, which is a hash of the version's"*
  *"`x.x.x_meta.json` file, which contains hashes for every file in the package."* *"The hashes in this
  file are then compared against when loading."* (https://github.com/denoland/deno/pull/22421, read
  2026-11-14). Before it, by its first sentence, JSR modules stood in the lock's `"remote"` section.
- *verified*, **the vendor copy**, the same pull request: *"Additionally, when using `{ "vendor": true }`
  in a deno.json, the files can be modified without causing lockfile errors—"* *"the checksum is only
  checked when copying into the vendor folder and not afterwards (eventually we should"* *"add this
  behaviour for non-jsr specifiers as well)."*
- *verified*, **what Deno's guide says**: vendoring is *"especially useful when building your project on
  a CI server or in a Docker container, or patching or otherwise modifying the remote dependencies."*;
  *"Deno will cache all dependencies locally in a `vendor` directory"* *"when the project is run,"* *"or
  you can optionally run the `deno install --entrypoint` command to cache the dependencies
  immediately"*; that command *"installs only the dependencies that are actually (transitively) imported
  by the specified entrypoint file."*; *"Deno automatically maintains a `deno.lock` file recording the
  exact version and integrity hash of every dependency,"* *"and verifies it on subsequent runs."*
  (https://docs.deno.com/runtime/packages/, read 2026-11-14). The page says neither whether the vendor
  folder holds only the modules used nor what is hashed per kind of specifier (the reader). So in Deno
  the vendor folder is written **when the program runs**, the reverse of R8.
- *verified*, **that JSR fetches only what is imported**, Ryan Dahl, *What we got wrong about HTTP
  imports*, July 29, 2024: *"This means JSR inherits the parts of HTTP imports that are really great."*
  *"For example: granular downloads of only the code that is actually being imported (no large
  tarballs!)."*; and *"Unlike npm, which downloads large tarballs, HTTP imports fetch only the necessary
  source code."* (https://deno.com/blog/http-imports, read 2026-11-14; panel 235's historian read the
  post for other sentences, its § 1 row 50; these are quoted here for the first time). A post by the
  runtime's author, not a reference page.
- *verified*, **`deno vendor` became a setting**: *"The `deno vendor` command has been replaced by a
  `"vendor": true` configuration option in `deno.json`."*, with no version on the sentence; the page's
  title names *"Deno 1.x to 2.x"* (https://docs.deno.com/runtime/reference/migration_guide/, read
  2026-11-15).

### 4. npm, pip and uv, Zig: whole archives, and who chooses their files

- *verified*, **npm, the publisher chooses the files**: *"The optional `files` field is an array of file
  patterns that describes the entries to be included"* *"when your package is installed as a
  dependency."* *"Omitting the field will make it default to `["*"]`, which means it will include all
  files."*; `package.json`, `README`, `LICENSE`, the `main` and `bin` files *"are always included"*
  (https://docs.npmjs.com/cli/v11/configuring-npm/package-json, read 2026-11-15). The lock's
  `integrity` is *"for the artifact that was unpacked in this location"* (the survey's Table 2.6,
  `historian-survey.md:443`, read 2026-11-07). A page of npm's own describing a check of
  `node_modules`' files after install: **not found** (the search in § search returned secondary pages
  only, none cited).
- *verified*, **a wheel, per-file hashes checked at install**: *"RECORD is a list of (almost) all the
  files in the wheel and their secure hashes."* *"Unlike PEP 376, every file except RECORD, which cannot
  contain a hash of itself, must include its hash."* *"During extraction, wheel installers verify all
  the hashes in RECORD against the file contents."* *"Apart from RECORD, RECORD.jws and RECORD.p7s,
  installation will fail if any file in the archive"* *"is not both mentioned and correctly hashed in
  RECORD."* The page says nothing of checking installed files afterwards (the reader)
  (https://packaging.python.org/en/latest/specifications/binary-distribution-format/, read 2026-11-15).
- *verified*, **the installed record**: *"Each record is composed of three elements: the file's **path**,
  the **hash** of the contents, and its **size**."* *"For other files, leaving the information out is
  discouraged,"* *"as it prevents verifying the integrity of the installed project."*; *"If the `RECORD`
  file is present, it must list all installed files of the project,"*; *"To completely uninstall a
  package, a tool needs to remove all files listed in `RECORD`,"*. The specification does not say a tool
  should verify installed files against it (the reader)
  (https://packaging.python.org/en/latest/specifications/recording-installed-packages/, read
  2026-11-15).
- *partly verified*, **pip's hash-checking mode**: *"This mode uses local hashes, embedded in a
  requirements.txt file, to protect against remote tampering and network issues."*; of a wheel built
  from cache, *"pip verifies the hashes against those of the original source distribution that was used
  to build the wheel."*; nothing on installed files (https://pip.pypa.io/en/stable/topics/secure-installs/,
  read 2026-11-15). The word *archive* is not in the sentences returned. `pylock.toml` hashes *"the
  file"*, the archive (the survey's Table 2.9, `historian-survey.md:529`). **What `uv.lock` hashes is
  `unverified`**: uv-resolver's `lock/mod.rs` on docs.rs was read to 100,000 of 308,026 characters and
  did not reach the struct; the search's other answers were a third-party handbook, not cited.
- *verified*, **Zig, the publisher's `paths`** (re-read 2026-11-15 for the hash's sentence, which the
  survey quoted only in part): `paths` *"Specifies the set of files and directories that are included
  in this package."* *"Only files included in the package are used to compute a package's `hash`."*; a
  dependency's `hash` *"is computed from the file contents of the directory of files that is obtained
  after fetching `url`"* *"and applying the inclusion rules given by `paths`."* *"This field is the
  source of truth; packages do not come from a `url`; they come from a `hash`."*. The page says nothing
  of files outside `paths` on disk, and nothing of a consumer fetching fewer
  (https://raw.githubusercontent.com/ziglang/zig/master/doc/build.zig.zon.md, read 2026-11-15). `paths`
  stands in the package's own `build.zig.zon`, so the publisher chooses (this seat's reading of whose
  file it is).

### 5. dep: a subset by import graph, a digest of the pruned tree in the lock, checked on a command

- *verified*, **the subset**, https://golang.github.io/dep/docs/Gopkg.toml.html (read 2026-11-14):
  *"`prune` defines the global and per-project prune options for dependencies."* *"The options determine
  which files are discarded when writing the `vendor/` tree."* *"`unused-packages` indicates that files
  from directories that do not appear in the package import graph should be pruned."* *"`non-go` prunes
  files that are not used by Go."* *"`go-tests` prunes Go test files."* *"Out of an abundance of caution,
  dep non-optionally preserves files that may have legal significance."* *"Pruning options are disabled
  by default."* *"However, generating a `Gopkg.toml` via `dep init` will add lines to enable `go-tests`
  and"* *"`unused-packages` prune options at the root level."* Of `non-go`: *"as dep only has a clear
  model for the role played by Go files, and non-Go files"* *"necessarily fall outside that model, there
  can be no comparable general definition of safety."*
- *verified*, **the digest and the declared subset, in the lock**, https://golang.github.io/dep/docs/Gopkg.lock.html
  (read 2026-11-14): `digest`, *"The hash digest of the contents of vendor/ for this project, after
  pruning rules have been applied."* *"The digest is versioned, by way of a colon-delimited prefix"*;
  version 1 is SHA-256; *"Line endings are normalized to LF"*; `pruneopts`, *"A compactly-encoded form of
  the prune options designated in Gopkg.toml ."*; `packages`, *"A complete list of directories from
  within the source that dep determined to be necessary for the build."*
- *verified*, **why, and how it was checked**, sam boyer, *Announcing dep v0.5.0*, July 25, 2018
  (https://golang.github.io/dep/blog/2018/07/25/announce-v0.5.0.html, read 2026-11-14/15): *"Vendor
  verification is the notion that `Gopkg.lock` should contain enough information to be able to verify"*
  *"whether the _current_ contents of `vendor/` are exactly as they should be"* *"including whatever
  pruning options you've set."* *"We've now done this, by adding the `digest` and `pruneopts` fields to
  each `[[project]]` stanza in `Gopkg.lock`."* *"it is no longer necessary for dep to rewrite the
  entirety of `vendor/` on every `dep ensure` run."* `dep check` reports *"hash of vendored tree not equal
  to digest in Gopkg.lock"*; *"If any of its checks fail, it will exit 1."* *"It's very fast; the checks
  it performs by default cannot hit the network."* *"It's also strongly recommended for use in CI."* And
  the escape: *"there are cases where you absolutely need to make modifications to certain projects in
  vendor"*; *"we have added `noverify` to `Gopkg.toml`, where you can provide a list of project roots"*
  *"(_not_ packages) for which vendor verification should be skipped."* The CHANGELOG
  (https://raw.githubusercontent.com/golang/dep/master/CHANGELOG.md, read 2026-11-14/15), undated
  entries: v0.4.0 *"Absorb `dep prune` into `dep ensure`."*; v0.5.0 *"Hash digests of vendor contents
  are now stored in `Gopkg.lock`,"* *"and the contents of vendor are only rewritten on change or hash
  mismatch"*. dep's daily guide: *"Never directly edit anything in `vendor/`; dep will unconditionally
  overwrite such changes."* (https://golang.github.io/dep/docs/daily-dep.html, read 2026-11-14).
- *verified*, **what happened to dep**: *"As of 2020, Dep is deprecated and archived in favor of Go
  modules"*, archived Sep 9, 2020 (the survey, `historian-survey.md:231-233`, read 2026-11-07). Go
  modules kept dep's package grain in `go mod vendor` (§ 1) and recorded no digest in `modules.txt`;
  **no page read says why the digest was not carried over**: looked for in #27348 and #33848, not found.
- *verified*, **the vendor-spec's grain**: *"A vendor file Package entry does NOT match a package tree; a
  vendor file Package entry matches a single Go package."* No `checksumSHA1` field is on the page read
  (https://github.com/kardianos/vendor-spec, read 2026-11-14); govendor's per-package checksum, which
  this seat had as a question from memory, was not verified.

### 6. Others

- *verified*, **Bazel fetches by reach at the repository's grain**: *"External repos (non-main repos) are
  fetched on demand"*; *"Normally, Bazel only fetches a repo when it needs something from the repo, and
  the repo hasn't already been fetched."*; `http_archive` *"downloads an archive from a URL and extracts
  it."* Whether a repo is fetched whole: not stated (https://bazel.build/external/overview, read
  2026-11-15). That is 00-shared.md's *entry* grain, not its *module* grain.
- *verified*, **Rust's own source tree vendors by crate, through `cargo vendor`**: *"This step executes
  `cargo vendor` to collect all dependencies"*; a field *"Only vendor crates necessary by the library
  workspace."*, which the file's `make_run` sets false; the one reason given, *"Reuse vendored
  dependencies when building source tarball for offline support."*
  (https://doc.rust-lang.org/nightly/nightly-rustc/src/bootstrap/core/build_steps/vendor.rs.html, read
  2026-11-15).
- **Dart, Swift, Gleam, Nim**: the survey's rows 6 (`historian-survey.md:373, 473, 507, 558`) record
  where each keeps fetched code, whole packages in a cache or a build folder; none describes placing a
  subset. Not searched further by this seat.

### 7. What each sum covers, when it is checked, and against R7

| system | placed grain | sum over | checked at fetch | checked at build | a placed copy edited by hand |
|---|---|---|---|---|---|
| Go vendor | reached Go packages (§ 1) | whole module (`go.sum`) | yes, module download | versions only (`modules.txt` against `go.mod`); *"does not check that packages in the `vendor` directory have not been modified"* | *"Local changes should not be made"*; re-vendor and diff |
| Go, its own `cmd` | reached Go packages (§ 9) | whole module | yes | none read; a test, short mode *"does NOT ensure"* contents (242) | the test's long mode, on builders |
| Cargo vendor | crate, whole (§ 2) | archive (`package`) and each file (`files`) | yes | listed files, at a build command (`cargo check`); every build: not shown | refused, *"not intended to be edited"*; unlisted files pass |
| Deno + JSR | reached modules, by its author's post (§ 3) | per version, a hash of the listing of per-file hashes | each file against its line, *"when loading"* | vendor copy: *"only checked when copying into the vendor folder and not afterwards"* | allowed, *"without causing lockfile errors"* |
| dep | reached packages, by option (§ 5) | the pruned tree, per project (`digest`) | written at `dep ensure` | on a command, `dep check`, CI recommended | `dep ensure` overwrites; `noverify` exempts |
| wheel | archive, whole | each file in `RECORD` | each file, at extraction | none described | none described |
| npm | tarball, whole (publisher's `files`) | the tarball (`integrity`) | yes | none found | none found |
| Zig | publisher's `paths` | the selected files | yes, *"source of truth"* | not read | not read |

**Against R7** (*"checked against its fingerprint at every build"*, `docs/panel/235-*.md:635-651`, as
00-shared.md quotes it): of the systems read, **Cargo alone checks a vendored copy at a build command**,
over the files its listing names, and calls the check *"not a security mechanism"*; that it does so at
every build is unread. Go and Deno say in their own words they do not re-check the placed copy. None of
the systems that place a **subset** checks it at build. R7 over a subset would be a combination this
seat found nowhere: JSR's per-file listing (§ 3) checked at every build as Cargo checks its own.

### 8. What each corrected, and why; and where each writes it

- **Go, 2018 to 2023**: Jul 2018, non-package directories dropped, kept by rsc's ruling (#26366); Oct
  2018, the copy set *"should be left alone"* (#27337); Aug 2018 onward, a check of vendored contents
  asked and not added, Backlog (#27348); Go 1.14, vendor used by default with a version check of
  `modules.txt` (#33848, the reference's *"`1.14` or higher"*); Go 1.17, `go.mod` and `go.sum` stripped
  from vendored modules (`vendor.go`'s comment); Go 1.23, a vendored package must be in `modules.txt`
  to be imported (`import.go`'s comment). Every one is a page of the tool, the reference or the tracker.
- **dep, 2018 to 2020**: v0.4.0 moved pruning into `dep ensure`; v0.5.0, July 25, 2018, added the
  digest, `pruneopts`, `dep check` and `noverify`; archived Sep 9, 2020 for modules (§ 5).
- **Deno, 2024**: February 15, 2024, the lock moved JSR modules out of per-module `"remote"` lines to
  one hash per version over the listing, and the vendor copy stopped being checked after copying
  (#22421); the `deno vendor` command became the `"vendor"` setting (the migration guide).
- **Cargo, 2019 to 2026, the requests to vendor less, none adopted**: rust-lang/cargo#7058, *"Support
  for vendoring on a single platform"*, opened June 21, 2019, **open**, *S-needs-design*; alexcrichton,
  quoted on October 9, 2019: *"Supporting this will be pretty difficult though, unfortunately."* *"if you
  were to remove the winapi folder, for example, then a lock file would no longer be correctly
  generated"*; epage, February 13, 2026: *"How would a CRAN packager know what is a sufficient set of
  packages?"* *"the idea of filtering the vendored packages for a use case like that has me concerned
  about supporting such a feature."* (https://github.com/rust-lang/cargo/issues/7058, read 2026-11-15).
  rust-lang/cargo#7065, *"cargo vendor without dev-dependencies?"*, June 23, 2019, **open**; weihanglo
  (*Member*), Dec 5, 2021: *"However, it will be frustrating when someone tries to run `cargo test` and
  fails,"* *"since the de facto way to use vendor sources is to replace the whole crates.io source."*
  (https://github.com/rust-lang/cargo/issues/7065, read 2026-11-15). rust-lang/cargo#13474, *"`cargo
  vendor` should offer the ability to omit the `tests/` directory"*, joshlf, Feb 21, 2024, **open**;
  epage: *"For vendoring, tests cause more vendoring churn"* *"I worry that the crate authors don't have
  enough information"* *"about what the person doing the vendoring is going to need, or not."*
  (https://github.com/rust-lang/cargo/issues/13474, read 2026-11-15). rust-lang/cargo#11929, *"`cargo
  vendor` pull large amounts of unrelated crates."*, WhyNotHugo, April 3, 2023: *"The total size of the
  `vendor` directory is 230M, but the unnecessary dependencies are over 182M (slightly over 80%)."*;
  closed *not planned* April 11, 2023 by weihanglo as a duplicate of #7058; **and the refusal to compute
  reach from the source**: eslerm, Dec 17, 2023, *"Possibly an AST could determine which dependencies the
  base package actually uses, to debloat vendored packages."*; weihanglo (*Member*), Dec 18, 2023:
  *"Cargo knows nothing about Rust AST, so this is out of the scope of Cargo right now."*
  (https://github.com/rust-lang/cargo/issues/11929, read 2026-11-15). A third-party tool fills the gap:
  cargo-vendor-filterer, *"it doesn't offer any filtering; today cargo includes all platforms"*,
  `exclude-crate-paths` *"Remove files and directories from target crates."*, *"some projects are not
  interested by vendoring test code or development dependencies of used crates."*; what it does to
  `.cargo-checksum.json` is not on its README (https://github.com/coreos/cargo-vendor-filterer, read
  2026-11-15).
- **Language reference or tool page.** Every placement and coverage statement above stands on a tool's
  page: the Go Modules Reference (go.dev/ref/mod), a document apart from the language specification,
  whose import sentence is *"The interpretation of the ImportPath is implementation-dependent"* (the
  survey, `historian-survey.md:195`); the Cargo book; Deno's documentation; Zig's `build.zig.zon.md`;
  the PyPA specifications. **None read puts placement or the sum's coverage in a language's
  reference**, the shape panel 244's R1 chose (*"Out: `heroes fetch` and `heroes.packages/`"*). Only Go's
  specification was read for it, by the survey.

### 9. The two alternatives

- *verified*, **the compiler's own tree under the reached grain: Go's**. `src/cmd/vendor/modules.txt`
  lists, under `# golang.org/x/tools v0.52.1-0.20261030190145-0f433775b36d`, seven package lines
  (`cmd/bisect`, `cover`, `go/gcexportdata`, `go/types/objectpath`, `internal/bisect`,
  `internal/gcimporter`, `internal/pkgbits`), nine under `golang.org/x/mod v0.42.0`, three under
  `golang.org/x/sys v0.49.0`, 63 package lines in the file (the reader's count)
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/vendor/modules.txt, read 2026-11-15; panel
  242's historian read the same file for its 12 modules, its lines 78-80, and did not quote its package
  lines, so it was re-read). README.vendor names `go mod vendor` in the update route (242's reading, its
  line 108). So Go's toolchain carries in its tree the reached packages of the modules it depends on,
  not the modules whole. **A ruler counted by modules reached rather than files placed: not found as a
  stated rule** on any page read; in Go the two coincide by the vendor command's construction, a
  reading.
- *verified*, **a first-party library split into versioned units**: Deno, *The Deno Standard Library is
  now available on JSR*, May 14, 2024 (Hinosawa, Franchetti, Casonato, Dahl, Gomez): *"The Deno Standard
  Library is now organized into 37 distinct packages, such as `@std/fs` and `@std/path`."* *"By adopting
  independent versioning for each package, we can stabilize mature packages promptly while continuing to
  refine and experiment with the less mature ones."*, a decision *"driven by the varied maturity,
  complexity, and usage of its components."*; *"deno.land/std will still be available indefinitely."*
  (https://deno.com/blog/std-on-jsr, read 2026-11-15). Go: *"The `golang.org/x/...` repositories are part
  of the Go Project but outside the main Go tree."* *"They are developed under looser compatibility
  requirements than the Go core."*; whether each is its own module is not on that page
  (https://go.dev/wiki/X-Repositories, read 2026-11-15). Nim 2.0 moved standard modules to nimble
  packages, `std/sha1` to `checksums/sha1`, with no reason given (panel 242's historian, its lines
  308-313, read 2026-11-10, not re-read).

### 10. The routes nobody listed in the first briefs, where a page bears

- **Route 1, panel 234's grain restored, held by a check**: this is Go's vendor model in every part a
  page states: per-program copies of the reached packages, a version check at build, contents held by
  re-running the copy and diffing (§ 1), and for Go's own tree a test (242). The pre-235 net's
  `packages/copies`, *"each file of a copy is its original's, byte for byte"* (00-shared.md), is that
  diff.
- **Route 4, a listing beside the manifest rather than in the tree**: dep's `digest` and `pruneopts` in
  `Gopkg.lock`, beside `Gopkg.toml` (§ 5); a `vendor.sum` beside `modules.txt`, floated in golang/go#27348
  and not added (§ 1). Cargo's, JSR's and the wheel's listings are not beside the manifest (§ Where the
  briefs ..., item 7).
- **Route 6, the publisher declares the grain**: Zig's `paths`, npm's `files`, Cargo's
  `include`/`exclude` each declare what the version holds, and its sum covers that (§ 2, § 4). JSR's
  per-version listing with per-file hashes and a module graph is the index declaring every file and the
  graph between them (§ 3). None read declares per-face listings within one version.
- **Route 7, a sum over a subset the catalogue declares**: dep, exactly: `pruneopts` declared, the digest
  over the pruned tree, both in the lock, checked by `dep check` (§ 5), with `noverify` added because
  some projects *"absolutely need to make modifications"*.
- **Route 8, M as a flag, W by default**: dep's prune options are *"disabled by default"* while `dep init`
  writes two of them on (§ 5); Go asked for the opposite flag, `go mod vendor -a`, closed as a duplicate
  (§ 1); Cargo's requests for a narrowing option stand open (§ 8).
- **Routes 2, 3, 5, 9**: no page read bears on them.
- **Who computes the reached set**: in Go and dep, the tool that already reads the language's imports
  (a reading of § 1's *"all packages needed to build and test"* and § 5's *"package import graph"*);
  Cargo declined to compute reach from the source, *"Cargo knows nothing about Rust AST"* (§ 8).
  00-shared.md finds no module of `selfhost/fetch/` reading a `use` line; the precedent puts the
  computation in the part of a toolchain that reads the language.
- **What a placed subset owes C files**: Go's copy lost C code in a non-package directory, and Go's
  answer was a layout rule, *"what's needed to build a package is in that directory"* (§ 1). That reads
  for H, every non-`.hero` file placed whole, or G, the Go package's whole folder, over a C closure
  computed by M.
- **What a placed subset owes a reader**: Go's copy is *"the trimmed copy in a vendor directory"*, not
  meant to be read (#27337, § 1). Heroes places cards and the site's build reads placed trees
  (00-shared.md); under M a program's tree would hold only its reached faces' cards.

### 11. Why this project went from the reached grain to the whole: what panel 235's records quote

**None found.** Read whole with the file reader: `docs/panel/235-reports/historian-survey.md` (lines 1
to 864) and `docs/panel/235-reports/historian.md` (lines 1 to 577). The survey's placement rows are Go's
row 6, *"The `go mod vendor` command constructs a directory named `vendor` in the main module's root
directory"* (`historian-survey.md:203`), a sentence that stops before the grain; Deno's row 6, *"`"vendor":
true` caches into a local `vendor` directory"* (`:427`); and JSR's per-file metadata (`:618`). The
report quotes no page on placement; its § 6 and § 8 are versions, indexes and names. Neither weighs a
subset against the whole, nor quotes a page on why one places the whole. Panel 235's briefs and record
were not read by this seat; the critic's search of them is 00-shared.md's, carried.

### 12. Pages fetched, and files written outside the repository's root

No fetch result in this session said it had saved a file, and this seat's own writes were to this
report alone. This seat has no shell and cannot list the fetching tool's results folder, so **whether
the tool wrote any file outside `/Users/joseph/Temp/heroes/heroes-lang` is unknown to this seat**, not
shown absent; panel 247's historian found one in `~/.claude/projects/…/tool-results/`
(`docs/panel/247-reports/historian.md` § One thing written outside the repository's root), and the same
folder is where to look.

---

## argument

Placing what a program reaches has lasted: Go's `go mod vendor` copies the reached packages, and Go's
own toolchain carries seven packages of `golang.org/x/tools` that way; dep pruned by import graph; Deno
fetches only imported JSR modules. Whole placement is Cargo's, whose requests to vendor less have stood
open since 2019. The sums differ: Go hashes whole modules and checks no vendored byte at build, by its
reference's own words; JSR lists per-file hashes per version, Deno checking vendored files only when
copying them; dep hashed the pruned tree, checked on a command. R7's every-build check over a subset
has no precedent read, for or against. Go's recorded cost, C code dropped from non-package directories,
reads for G or H.

## condition

This seat's reading would change on:

- a system that placed the reached grain and returned to whole placement for a stated cause, which
  would argue for W;
- a page recording a failure of a per-version listing of per-file hashes (JSR's shape), such as a
  partial or stale copy passing its check, which would argue against F1;
- a system that checks a placed subset at every build against its listing, with a recorded cost or
  failure, which would settle R7 under M by precedent rather than leave it a departure;
- a page saying why Go modules did not carry dep's vendor digest over, which may hold the argument
  against F3;
- a page of Go's showing a newly written import refused in vendor mode with a message naming `go mod
  vendor`, which would turn the verdict's reading of the reader's cost into a sentence.

## search

All pages were fetched with the page-reading tool on the days § The instrument gives. The web
searches, with their exact queries:

- `golang issue "go mod vendor" does not copy C files directories without Go files cgo 26366`
  (golang/go#26366 and #29102 read; #43317 and #29479 named in the summary, not read).
- `deno.lock jsr package integrity hash of version metadata "_meta.json" lockfile format` (a mirror's
  commit led to denoland/deno#22421, read at github.com).
- `deno.json "vendor": true vendor directory remote modules jsr docs "deno vendor" removed Deno 2` (the
  migration guide and the packages guide read).
- `JSR "only the modules" OR "only downloads" OR "individual modules" imported files downloaded on
  demand not whole package tarball Deno jsr` (Deno's HTTP-imports post read; a third-party mirror of the
  docs not cited).
- `cargo source directory.rs "directory sources are not intended to be edited" checksum
  ".cargo-checksum.json" verify` (directory.rs and the test suite read).
- `cargo vendor source "cp_sources" ".cargo-checksum.json" "Cargo.toml.orig" ".cargo-ok" vendor.rs`
  (`ops/vendor.rs` answered 404 at rust.googlesource.com and at doc.rust-lang.org nightly and stable;
  `ops/cargo_vendor.rs` read instead).
- `dep v0.5.0 release notes "dep check" digest vendor verification "dep prune" removed prune options
  Gopkg.toml 2018` (secondary summaries; the announcement and CHANGELOG read instead).
- `uv.lock format hashes sdist wheels "hash" archive uv documentation lockfile verify` (third-party only;
  not cited); `pip install --require-hashes "hash" of the downloaded archive file wheel sdist "secure
  installs" pip documentation hash-checking mode` (pip's page read).
- `Deno standard library on JSR @std packages versioned independently blog 2024 deno.land/std` (the
  std-on-JSR post read).
- `golang proposal "go mod vendor" copy entire module all files instead of only imported packages issue`
  (#33848 and #27337 read; no proposal for whole-module vendoring found among the results).
- `golang issue cmd/go verify vendor directory contents hash go.sum "go mod verify" vendor modified
  detect proposal` (#27348 and #43706 read).
- `npm verify installed node_modules files integrity after install tampered "npm ci" integrity checked
  only at extraction cacache` (secondary sources only, none cited: **looked for and not found**, an npm
  page describing a post-install check of `node_modules`).
- `rust-lang rust source tarball vendor directory size windows crates trimmed "cargo vendor" rustc-src
  tarball too large issue` (Rust's bootstrap `vendor.rs` and cargo#11929 read; the 2017 internals thread
  *Minimizing the rust-src component* named in the results, not read, not cited).

Pages read without a search: go.dev/ref/mod (offsets 0 and 100000); Go's `modcmd/vendor.go`,
`modload/import.go` and `cmd/vendor/modules.txt`, raw at `master`; `modload/vendor.go` raw was refused
by the tool (*"Unable to verify if domain raw.githubusercontent.com is safe to fetch"*) and is not
cited; cargo's `cargo-vendor` page, *Source Replacement*, `directory.rs`, `cargo_vendor.rs`, the
`directory.rs` test suite; rust-lang/cargo#7058, #7065, #13474, #11929 (twice, the second for the
comment the refusal answers); coreos/cargo-vendor-filterer; jsr.io/docs/api; docs.deno.com's modules
page (it holds no vendoring text and points to the packages guide), packages guide and migration guide;
dep's `Gopkg.toml`, `Gopkg.lock` and daily-dep pages, the v0.5.0 announcement, the CHANGELOG;
kardianos/vendor-spec; Zig's `build.zig.zon.md`; the PyPA binary-distribution-format and
recording-installed-packages specifications; pip's secure-installs page; uv-resolver's `lock/mod.rs`
on docs.rs (first 100,000 characters); npm's `package.json` reference; bazel.build's external overview;
Rust's bootstrap `vendor.rs`; go.dev/wiki/X-Repositories; Deno's std-on-JSR post.

**Looked for and not found, by where**: why Go modules did not carry dep's digest (golang/go#27348,
#33848); a page of npm's own on checking installed files (the npm search above); a consumer-side option
to vendor fewer files of a crate in Cargo itself (the `cargo vendor` page; the four issues, all open or
closed unadopted); a consumer fetching fewer files than Zig's `paths` (the zon document); what `uv.lock`
hashes (uv-resolver's source, to its first 100,000 characters); a JSR page saying clients fetch only
what they import (jsr.io/docs/api); govendor's `checksumSHA1` (kardianos/vendor-spec); a toolchain stating
a ruler of its dependencies by modules compiled (Go's `modules.txt` and README.vendor through 242, Rust's
bootstrap `vendor.rs`). Panel 235's survey and report were read whole for a page on a subset (§ 11).
The briefs read: `250-briefs/historian.md` and `00-shared.md`; the seat's definition
`.claude/agents/historian.md`; panel 242's historian report whole; panel 247's historian report, its
first 60 lines.

---

## Candidate prior-art lines (the brief's closing requirement)

For the `## Prior art` of the decision that ratifies this sitting, in the two forms
`.claude/rules/records.md` § And the record says whose idea it was allows. **Which form applies is the
synthesis's**: *Taken from* where the resolution adopts a form on that page's evidence, *Reached first
by* where a seat or the author reached it and the page was found after; a route not adopted has no line.
Each says only what its page states and still owes the second reading that section asks. Another
project's issue or pull request goes through `redirect.github.com`; a raw file links to its rendered
page.

**If a program places the reached grain (M or G)**:

- **Reached first by Go**: [Go Modules Reference, `go mod vendor`](https://go.dev/ref/mod#go-mod-vendor): "The `go mod vendor` command constructs a directory named `vendor` in the main module's root directory that contains copies of all packages needed to support builds and tests of packages in the main module."
- **Reached first by dep**: [Gopkg.toml, prune](https://golang.github.io/dep/docs/Gopkg.toml.html): "`unused-packages` indicates that files from directories that do not appear in the package import graph should be pruned."
- **Reached first by Deno**: [What we got wrong about HTTP imports](https://deno.com/blog/http-imports): "For example: granular downloads of only the code that is actually being imported (no large tarballs!)."

**If the compiler places only what it reaches, as every program does**:

- **Reached first by Go**: [src/cmd/vendor/modules.txt](https://github.com/golang/go/blob/master/src/cmd/vendor/modules.txt): `# golang.org/x/tools v0.52.1-0.20261030190145-0f433775b36d` followed by seven package lines, `golang.org/x/tools/cmd/bisect` to `golang.org/x/tools/internal/pkgbits`.

**If F1, the version's listing placed and each placed file checked against its line**:

- **Reached first by Deno**: [denoland/deno#22421](https://redirect.github.com/denoland/deno/pull/22421): "Instead a single JSR integrity is stored per package in the lockfile, which is a hash of the version's `x.x.x_meta.json` file, which contains hashes for every file in the package." "The hashes in this file are then compared against when loading."
- **Reached first by Cargo**: [directory.rs, `Checksum`](https://doc.rust-lang.org/nightly/nightly-rustc/src/cargo/sources/directory.rs.html): "Checksum of the package. Normally it is computed from the `.crate` file." "Checksums of each source file."
- **Reached first by Python packaging**: [Binary distribution format](https://packaging.python.org/en/latest/specifications/binary-distribution-format/): "During extraction, wheel installers verify all the hashes in RECORD against the file contents."

**If a file present in a placed tree and absent from its listing is refused**:

- **Reached first by Go**: [modload/import.go](https://github.com/golang/go/blob/master/src/cmd/go/internal/modload/import.go): "go: ignoring package %s which exists in the vendor directory but is missing from vendor/modules.txt." "Go 1.23 and later require vendored packages to be present in modules.txt to be imported."

**If F3 or route 7, a sum over the placed subset with the subset declared**:

- **Reached first by dep**: [Gopkg.lock](https://golang.github.io/dep/docs/Gopkg.lock.html): `digest`, "The hash digest of the contents of vendor/ for this project, after pruning rules have been applied."; `pruneopts`, "A compactly-encoded form of the prune options designated in Gopkg.toml ."

**If F4, the sum checked when placed and not at build**:

- **Reached first by Go**: [Go Modules Reference, `go mod vendor`](https://go.dev/ref/mod#go-mod-vendor): "The `go` command does not check that packages in the `vendor` directory have not been modified, but one can verify the integrity of the `vendor` directory by running `go mod vendor` and checking that no changes were made."
- **Reached first by Deno**: [denoland/deno#22421](https://redirect.github.com/denoland/deno/pull/22421): "the checksum is only checked when copying into the vendor folder and not afterwards".

**If R7 is kept, a placed tree checked at build**:

- **Reached first by Cargo**: [tests/testsuite/directory.rs, `bad_file_checksum`](https://github.com/rust-lang/cargo/blob/master/tests/testsuite/directory.rs): `cargo check` over a vendored file overwritten after vendoring expects "[ERROR] the listed checksum of `[ROOT]/index/bar/src/lib.rs` has changed:".

**If H or G, so that no C closure is computed**:

- **Taken from Go**: [golang/go#26366](https://redirect.github.com/golang/go/issues/26366): "The cause here seems to be that we prune out directories that are not Go packages."; "Sorry, but the model for go builds is that what's needed to build a package is in that directory, not subdirectories."

**If the reached set is computed by the part of the toolchain that reads `use` lines**:

- **Taken from Cargo**: [rust-lang/cargo#11929](https://redirect.github.com/rust-lang/cargo/issues/11929): "Cargo knows nothing about Rust AST, so this is out of the scope of Cargo right now."

**If W is kept, the version placed whole with a listing**:

- **Reached first by Cargo**: [cargo vendor](https://doc.rust-lang.org/cargo/commands/cargo-vendor.html): "This cargo subcommand will vendor all crates.io and git dependencies for a project into the specified directory at `<path>`."; "Cargo treats vendored sources as read-only as it does to registry and git sources."

**If route 6, the publisher declares which files a version's sum covers**:

- **Reached first by Zig**: [build.zig.zon](https://github.com/ziglang/zig/blob/master/doc/build.zig.zon.md): "Only files included in the package are used to compute a package's `hash`."
- **Reached first by npm**: [package.json, `files`](https://docs.npmjs.com/cli/v11/configuring-npm/package-json): "The optional `files` field is an array of file patterns that describes the entries to be included when your package is installed as a dependency."

**If route 8, a narrowing chosen by a setting, off by default**:

- **Reached first by dep**: [Gopkg.toml, prune](https://golang.github.io/dep/docs/Gopkg.toml.html): "Pruning options are disabled by default."

**If `core` is split into entries with a version each**:

- **Reached first by Deno**: [The Deno Standard Library is now available on JSR](https://deno.com/blog/std-on-jsr): "By adopting independent versioning for each package, we can stabilize mature packages promptly while continuing to refine and experiment with the less mature ones."

report complete
