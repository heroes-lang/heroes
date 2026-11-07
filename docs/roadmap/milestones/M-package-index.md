# M-package-index — the index a Heroes package is published to and fetched from

**Scheduled 2026-11-07, at row 78, by the author's instruction of 2026-11-06**,
immediately after M-package-manager, meant as: *one day there will be a Heroes
index, a site we download these packages from as well as from git, like `pip
install`; we have to work out how to run that big index one day, and the
catalogue's structure, a YAML file or whatever it becomes, must allow for it
now*. Given in conversation while M-package-manager's plan was being written.

**Why it is a row of its own, and why it sits here.** M-package-manager
delivers the catalogue, the file that names a program's dependencies, and the
author ruled the same day that each of its entries says where its dependency
comes from: the index, by name and version, or git, by address and reference,
always with its fingerprint (`M-package-manager.md`, the author's position,
point 6). So the index's shape reaches the catalogue before the index exists,
and row 77's opening sitting, panel 235, decides what of it the first version
must already respect, so that the catalogue does not change when the index
arrives. Delivering the index itself, the site, publishing a package to it,
who may take a name and what happens to a withdrawn one, is a different
deliverable, so a different row (CLAUDE.md § 14).

**What its opening sitting inherits**, none of it measured yet: the survey of
precedents written for panel 235 carries a table of the existing indexes
(PyPI, crates.io, npm, Hex, pub.dev, Go's module proxy protocol, JSR), what
each holds, the protocol a client reads it by, whether it is a static site or
a service, who may publish a name and what a withdrawn name does; and panel
235 measures a static index, files served by a site, as one variant, because
it has no service to keep running and can be tried from a local folder. The
code does not change when a dependency moves from git to the index: it writes
`<json>` either way, and only the catalogue's entry changes.
