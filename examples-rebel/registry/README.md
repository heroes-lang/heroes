# registry

registry: the site that serves Heroes packages, and its client.

A publisher packs a package's folder into a reproducible archive (a tar of
one folder `<name>-<version>`, compressed by Zstandard, xz or gzip), signs
a statement of its name, version and digest with an Ed25519 key, and
uploads both. The server opens the archive as a stranger's, validates its
`heroes.toml` against a JSON Schema, checks the signature against the
publisher's registered key, keeps the archive under its SHA-256 and the
version in SQLite, and indexes it for search. A package manager lists and
searches packages, resolves a SemVer range to the highest version not
yanked, reads the package index, fetches an archive and verifies its digest
and signature against its own trust file, and asks what changed between two
versions: the manifest as a JSON Patch, each file as a unified diff.
Publishers hold tokens with scopes; publications are rate-limited.

Run for real:

    registry token --config registry.toml --publisher ana
    registry serve --config registry.toml
    registry keygen --out ana.pem
    registry add-key --registry http://127.0.0.1:8470/ --token $T --key ana.pem
    registry pack ./textkit --out textkit.tar.zst
    registry publish textkit.tar.zst --registry http://127.0.0.1:8470/ --token $T --key ana.pem
    registry resolve textkit '^1.0' --registry http://127.0.0.1:8470/
    registry fetch textkit 1.1.0 --trust ana-keys.pem --out textkit.tar.zst --registry ...

and `registry demo build/registry-demo` runs the whole of it on loopback in
a folder it makes and removes, printing a transcript that is the same on
every run (`main.args`, `main.expected`).

Rebel's packages and what for:

- `web`: the router, the 404 and 405, the Bearer challenge, the publishers'
  rate limit, entity tags and the request log; `http`: the server's loop of
  requests and the client's session, multipart uploads, retries; `net`: the
  listener on loopback; `os`: the stop, secure randomness, the
  environment. The server's workers, the owner of its state and the demo's
  client run on threads of the language's own (`spawn`, mailboxes).
- `semver`: versions read and ordered, ranges resolved as Cargo reads them.
- `archive`: tar read and written; `compress`, `compress/zstd`,
  `compress/xz`: the three packings an upload may take.
- `hash`: the CRC-32 that ends every token, so a mistyped one is told
  without a database; `crypto`: SHA-256 digests and constant-time
  comparison; `crypto/keypairs`: Ed25519 signatures; `pem`: keys and
  detached signatures as PEM blocks with RFC 1421 headers.
- `sql`, `sql/sqlite`, `sql/migrate`: the database and its numbered
  migrations; `fs`, `path`: the archives kept by digest, folders walked with
  an ignore file, POSIX paths of archive entries checked.
- `toml`: the settings and every manifest; `json`, `json/schema`: the API's
  bodies and the manifest's schema; `json/patch`: a manifest's changes;
  `diff`: a file's changes, and a new name's likeness to another
  publisher's (typosquatting); `search`: BM25 over names and descriptions.
- `retries`: the client's waits, and a 429's `Retry-After` read; `url`: the
  registry's URL and every query; `flags`: this command line; `log`, `io`,
  `time`, `bytes`, `strings`, `numbers`: what the rest stands on.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/registry/main.hero -- demo build/registry-demo

`main.skips` names the platforms where the demo is built and not run, and why.

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 33 faces on its `packages` line, each linked here to its reference: [`archive`](https://heroes-lang.org/rebel/archive/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`compress`](https://heroes-lang.org/rebel/compress/), [`compress/xz`](https://heroes-lang.org/rebel/compress/xz/), [`compress/zstd`](https://heroes-lang.org/rebel/compress/zstd/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`crypto/keypairs`](https://heroes-lang.org/rebel/crypto/keypairs/), [`diff`](https://heroes-lang.org/rebel/diff/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`json/patch`](https://heroes-lang.org/rebel/json/patch/), [`json/schema`](https://heroes-lang.org/rebel/json/schema/), [`log`](https://heroes-lang.org/rebel/log/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`pem`](https://heroes-lang.org/rebel/pem/), [`retries`](https://heroes-lang.org/rebel/retries/), [`search`](https://heroes-lang.org/rebel/search/), [`semver`](https://heroes-lang.org/rebel/semver/), [`sql`](https://heroes-lang.org/rebel/sql/), [`sql/migrate`](https://heroes-lang.org/rebel/sql/migrate/), [`sql/sqlite`](https://heroes-lang.org/rebel/sql/sqlite/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/), [`url`](https://heroes-lang.org/rebel/url/), [`web`](https://heroes-lang.org/rebel/web/).

## Its folders

9 folders, 43 files and 6,802 lines, this page aside; of them 39 modules, `.hero` files, of 6,660 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 218 | 1 | 76 |
| `cli/` | 4 | 462 | 4 | 462 |
| `client/` | 3 | 546 | 3 | 546 |
| `demo/` | 6 | 834 | 6 | 834 |
| `packages/` | 8 | 1,460 | 8 | 1,460 |
| `search/` | 1 | 142 | 1 | 142 |
| `server/` | 10 | 1,971 | 10 | 1,971 |
| `signing/` | 2 | 414 | 2 | 414 |
| `store/` | 4 | 755 | 4 | 755 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
