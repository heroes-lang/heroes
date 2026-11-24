# kanban

kanban: a desktop kanban board in Tauri's shape. Its interface is a page,
HTML, CSS and a script in `web/`, shown in the system's own web view (WebKit
on macOS, WebView2 on Windows, WebKitGTK on Linux) and served from the
program's own memory at `app://localhost`, never from a port; its logic is
this program, which keeps the boards, their columns and their cards in
SQLite. Every act of the person, a card added, moved by dragging, edited,
a search, an undo, is a call of the page's script over the bridge
(`heroes.invoke`), answered by the program, which then tells the page the
board changed.

    heroes build rebel/examples/kanban/main.hero -o kanban
    ./kanban --web rebel/examples/kanban/web    # the board in a window
    ./kanban --data boards.sqlite               # another database
    ./kanban --demo --web rebel/examples/kanban/web

The demo runs the back end on the headless side with no screen: the window
and the page's files made as on the desktop, a scripted session of the
page's calls sent through the same handler, a database under
`build/kanban-demo/` with a fixed clock and counted names, and a transcript
that is the same on every run (`main.args`, `main.expected`).

Rebel's packages, and what for: `ui` the window, its menu bar and the
event loop, and `ui/webview` the web view, its assets, its policy and its
bridge; `json` every call and answer and the boards' files; `sql` and
`sql/sqlite` the boards, columns, cards and the history undo reads;
`markdown` a card's notes as HTML and `html/sanitize` that HTML cleaned;
`template` the page's index and the board's report in its HTML mode;
`uuid` the names of boards, columns and cards; `time` their moments;
`search` the cards ranked by BM25; `fs` the database's folder and the
files a board is exported to and imported from; `path` and `os` where
they stand.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/kanban/main.hero -- --demo --web rebel/examples/kanban/web

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 14 faces on its `packages` line, each linked here to its reference: [`fs`](https://heroes-lang.org/rebel/fs/), [`html/sanitize`](https://heroes-lang.org/rebel/html/sanitize/), [`json`](https://heroes-lang.org/rebel/json/), [`markdown`](https://heroes-lang.org/rebel/markdown/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`search`](https://heroes-lang.org/rebel/search/), [`sql`](https://heroes-lang.org/rebel/sql/), [`sql/sqlite`](https://heroes-lang.org/rebel/sql/sqlite/), [`template`](https://heroes-lang.org/rebel/template/), [`time`](https://heroes-lang.org/rebel/time/), [`ui`](https://heroes-lang.org/rebel/ui/), [`ui/webview`](https://heroes-lang.org/rebel/ui/webview/), [`uuid`](https://heroes-lang.org/rebel/uuid/).

## Its folders

9 folders, 37 files and 3,480 lines, this page aside; of them 29 modules, `.hero` files, of 2,688 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 173 | 1 | 57 |
| `api/` | 5 | 554 | 5 | 554 |
| `app/` | 6 | 413 | 6 | 413 |
| `demo/` | 2 | 271 | 2 | 271 |
| `store/` | 11 | 1,134 | 11 | 1,134 |
| `support/` | 2 | 126 | 2 | 126 |
| `text/` | 1 | 44 | 1 | 44 |
| `transfer/` | 1 | 89 | 1 | 89 |
| `web/` | 5 | 676 | 0 | 0 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
