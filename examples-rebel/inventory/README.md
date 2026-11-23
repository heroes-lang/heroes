# inventory

inventory: stock management for a small chain of shops. It keeps the
chain's locations, its products and their variants, every variant's stock
at every location, and every movement of stock (receipts from suppliers,
sales at the tills, transfers between locations, adjustments and counts)
with an audit trail of everything asked of it, refusals included. It
values stock at moving average cost in exact cents, tells which shelves
are at or below their reorder point, imports catalogues and count sheets
from CSV and XLSX (every rejected row told with its line and reason), and
exports the stock the same two ways. One model of the books stands over
three stores the operator chooses by configuration: MySQL (or MariaDB),
any database the system's ODBC manager reaches, and MongoDB, which holds
the same books as documents in BSON. Every key is a UUID of version 7
the program makes, so a catalogue keeps its keys in every store.

Run for real:

    heroes build examples-rebel/inventory/main.hero -o inventory
    ./inventory init shop.toml                   # make the tables or indexes
    ./inventory location shop.toml WH Warehouse
    ./inventory import shop.toml products catalogue.xlsx
    ./inventory import shop.toml counts opening.csv
    ./inventory receive shop.toml TEE-BLK-M WH 24 7.10 PO-1042
    ./inventory sell shop.toml TEE-BLK-M SHOP-N 2
    ./inventory account shop.toml
    ./inventory export shop.toml stock.xlsx

`conf/settings.hero` documents the configuration file; `cli/commands.hero`
every command. A password stands in the environment
(`INVENTORY_STORE__MYSQL__PASSWORD`), never in the file. MySQL is reached
over TLS checked against the system's store unless the URL says
`ssl-mode=DISABLED`; `sql/mysql` begins no TLS on macOS, so there a MySQL
store is reached over loopback or a tunnel.

The demo, `./inventory demo` (`main.args`), starts on loopback the two
stand-ins it carries, a MySQL server (the client/server protocol's
handshake with `caching_sha2_password`, the text protocol and prepared
statements, its statements run on SQLite in memory) and a MongoDB server
(OP_MSG with BSON: `hello`, `insert`, `update`, `delete`, `find`,
`aggregate`, `createIndexes`); it runs one scripted day against each
store, prints the account once and, for each store, the digest of the
account that store gave, which is the same for every store; then it
imports a CSV and a spreadsheet full of bad rows and exports the stock as
CSV and XLSX. The ODBC store runs through SQLite's ODBC driver
(`libsqliteodbc`, registered as `SQLite3`) where the machine has the
driver manager and the driver; elsewhere its one line reads
`odbc: skipped: no ODBC driver`. That line is the transcript's one line
that may differ: `main.expected` holds it as a machine without ODBC
prints it, and a machine with both, as the CI's Linux and macOS legs
install them, prints there the line the MySQL and MongoDB stores print,
its first word `odbc:`, the same digest among them, and no other line
changes.

Rebel's packages, and what for:

- `sql`, `sql/mysql`, `sql/odbc`, `sql/build`: the SQL store, its
  queries composed as values and re-spelt for MySQL and ODBC;
- `mongodb`, `bson`: the MongoDB store, keys as BSON UUIDs, a variant's
  levels one document so a movement is one atomic write; and the
  stand-in's OP_MSG;
- `sql/sqlite`: the engine behind the MySQL stand-in;
- `xlsx`, `csv`, `fs`, `io`: imports read by line or row, exports written;
- `uuid`: every key; `config`, `json`: the configuration in layers;
- `containers/indexed`: an import's SKUs and shelves in file order, each
  with the line that first named it;
- `crypto`, `random`, `bytes`: the MySQL stand-in's login, the digests of
  the accounts;
- `net`, `os`: the stand-ins' sockets and the stop, the stand-ins on
  threads of the language's own (`spawn`, mailboxes);
- `time`, `strings`, `numbers`, `lists`, `maps`: throughout.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/inventory/main.hero -- demo

A line `main.varies` names is printed in another form where another machine decides it, each form written there.

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 25 faces on its `packages` line, each linked here to its reference: [`bson`](https://heroes-lang.org/rebel/bson/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`config`](https://heroes-lang.org/rebel/config/), [`containers/indexed`](https://heroes-lang.org/rebel/containers/indexed/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`csv`](https://heroes-lang.org/rebel/csv/), [`fs`](https://heroes-lang.org/rebel/fs/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`lists`](https://heroes-lang.org/rebel/lists/), [`maps`](https://heroes-lang.org/rebel/maps/), [`mongodb`](https://heroes-lang.org/rebel/mongodb/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`random`](https://heroes-lang.org/rebel/random/), [`sql`](https://heroes-lang.org/rebel/sql/), [`sql/build`](https://heroes-lang.org/rebel/sql/build/), [`sql/mysql`](https://heroes-lang.org/rebel/sql/mysql/), [`sql/odbc`](https://heroes-lang.org/rebel/sql/odbc/), [`sql/sqlite`](https://heroes-lang.org/rebel/sql/sqlite/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/), [`uuid`](https://heroes-lang.org/rebel/uuid/), [`xlsx`](https://heroes-lang.org/rebel/xlsx/).

## Its folders

14 folders, 43 files and 10,080 lines, this page aside; of them 39 modules, `.hero` files, of 9,944 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 230 | 1 | 94 |
| `cli/` | 1 | 288 | 1 | 288 |
| `conf/` | 1 | 165 | 1 | 165 |
| `demo/` | 3 | 617 | 3 | 617 |
| `exports/` | 1 | 195 | 1 | 195 |
| `files/` | 1 | 198 | 1 | 198 |
| `imports/` | 3 | 668 | 3 | 668 |
| `model/` | 6 | 1,344 | 6 | 1,344 |
| `report/` | 1 | 208 | 1 | 208 |
| `service/` | 1 | 175 | 1 | 175 |
| `standins/` | 1 | 124 | 1 | 124 |
| `standins/mongo/` | 6 | 1,840 | 6 | 1,840 |
| `standins/mysql/` | 6 | 1,779 | 6 | 1,779 |
| `store/` | 7 | 2,249 | 7 | 2,249 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
