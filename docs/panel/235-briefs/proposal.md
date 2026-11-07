# Panel 235, the proposal in ten lines

Heroes gets its dependency system in M-package-manager, decided and built
before the core packages. A program names a fetched dependency in a `use` line
by a name its catalogue resolves (the author's proposal, `use <json>/parse`);
a `use` with no mark stays a module of the program's own tree, as today. The
catalogue gives each entry its source (the Heroes index by name and version,
git by address and tag, or a local folder while a package is developed), an
exact version and a fingerprint. The semantics copy one widely used system
whole, Go's modules or Cargo, in its newest release, without workspaces, with
Heroes' own names. The sitting chooses the base, the `use` spelling, the
catalogue's file and format, where fetched code lives, how versions are
selected, how the first version fetches over C, and the verbs; it rewrites the
rules this touches; and it measures every choice against the author's
criteria: the most solid, robust and simple, the most modern, the best for
language models.
