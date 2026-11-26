# backup

backup: a deduplicating, encrypted backup tool in the family of restic
and borg. A folder is backed up as a snapshot into a repository, kept in
a folder or in a bucket of any S3 service: every file cut into chunks
where its content says, each chunk named by a keyed hash, stored once
however many files and snapshots hold it, compressed by Zstandard where
that helps, and sealed by AES-256-GCM under a master key that only the
repository's password opens. A run reads only the files that changed
since the last snapshot of the same folder, and stores only the chunks
the repository lacks, asking a Bloom filter before the index. A
snapshot, or one path of it, is restored with every chunk and every
file checked; old snapshots are forgotten by a retention policy and
what they alone held is pruned; a snapshot is exported as a tar
archive; a repository is copied offsite, still sealed.

Run for real:

    heroes build rebel/examples/backup/main.hero -O2 -o backup
    export BACKUP_REPOSITORY=/var/backups/repo    # or s3:https://host/bucket/prefix
    ./backup init                                 # asks the password twice
    ./backup run ~/Documents                      # the AWS_* variables for a bucket
    ./backup snapshots
    ./backup restore latest --target /tmp/restored [--path reports/a.txt]
    ./backup forget --keep-daily 7 --keep-weekly 4 --prune
    ./backup prune
    ./backup check
    ./backup export latest --output documents.tar.gz
    ./backup copy --to s3://offsite-bucket/laptop

The password is read from `--password-file` (`BACKUP_PASSWORD_FILE`),
else `BACKUP_PASSWORD`, else the terminal. A folder may hold a
`.backupignore` file, read as git reads a `.gitignore`. Build it with
`-O2`: the cryptography and the codecs are Heroes, and the password's
cost is calibrated to half a second on the machine that makes the
repository.

The demo, `./backup demo`, writes a folder under `build/backup-demo/`,
backs it up three times with edits between, lists the snapshots with
the deduplication ratio, restores and compares, forgets and prunes,
checks, exports a tar archive and reads it back, copies the repository
to an S3 stand-in it starts on loopback and restores from there, and
last changes a byte of a pack for the check to find; its transcript is
`main.expected`.

Rebel's packages, and what for:

- `chunks`: the line ends a text file is cut at;
- `hash`: the chunker's table (XXH64 under the repository's seed) and
  the Bloom filter's places; the stand-in's CRC-32 check;
- `crypto`: the HMAC-SHA256 that names chunks, trees and snapshots, the
  SHA-256 of every file, scrypt for the password; SigV4's HMACs in the
  stand-in;
- `crypto/seal`: AES-256-GCM over every object the repository writes;
- `compress/zstd`: every chunk and record, where it is smaller;
- `compress`: telling a file compressed already by gzip or zlib, which
  is stored as it is; the demo's gzip log;
- `containers/indexed`: the chunk index, whose positions the pruner
  marks; `containers/bits`: the Bloom filter and the pruner's marks;
- `s3`: the repository in a bucket, and the signing the stand-in checks;
  `retries`: the backoff of every S3 request;
- `archive`: `export` as tar, and the demo's reading of it back;
- `fs`, `path`: the folders walked, restored and written, the
  `.backupignore` files, the folder storage;
- `term`: the progress bar, the table of snapshots, the password typed;
  `flags`: the command line;
- `json`: every record the repository holds;
- `http`, `net`, `url`: the S3 stand-in, on a thread of the language's
  own (`spawn`, a mailbox);
- `io`, `os`, `time`, `bytes`, `strings`, `numbers`: throughout.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/backup/main.hero -- demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 25 faces on its `packages` line, each linked here to its reference: [`archive`](https://heroes-lang.org/rebel/archive/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`chunks`](https://heroes-lang.org/rebel/chunks/), [`compress`](https://heroes-lang.org/rebel/compress/), [`compress/zstd`](https://heroes-lang.org/rebel/compress/zstd/), [`containers/bits`](https://heroes-lang.org/rebel/containers/bits/), [`containers/indexed`](https://heroes-lang.org/rebel/containers/indexed/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`crypto/seal`](https://heroes-lang.org/rebel/crypto/seal/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`retries`](https://heroes-lang.org/rebel/retries/), [`s3`](https://heroes-lang.org/rebel/s3/), [`strings`](https://heroes-lang.org/rebel/strings/), [`term`](https://heroes-lang.org/rebel/term/), [`time`](https://heroes-lang.org/rebel/time/), [`url`](https://heroes-lang.org/rebel/url/).

## Its folders

10 folders, 49 files and 8,492 lines, this page aside; of them 46 modules, `.hero` files, of 8,379 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 201 | 1 | 88 |
| `chunking/` | 2 | 392 | 2 | 392 |
| `cli/` | 5 | 805 | 5 | 805 |
| `demo/` | 5 | 1,066 | 5 | 1,066 |
| `engine/` | 9 | 1,669 | 9 | 1,669 |
| `model/` | 2 | 482 | 2 | 482 |
| `repo/` | 10 | 1,933 | 10 | 1,933 |
| `standins/` | 4 | 776 | 4 | 776 |
| `store/` | 5 | 746 | 5 | 746 |
| `vault/` | 3 | 422 | 3 | 422 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
