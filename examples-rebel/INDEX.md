# Rebel's programs

The programs of `examples-rebel/` written with Rebel, each a project of its own with its catalogue: 26 applications and 16 twins. `README.md` § A10 says what each kind is and how the check of Rebel's programs holds it; each program's own page says what it is and how to run it for real, its demo's command, the packages it uses and what its folders hold. Rebel's packages themselves are indexed in [`PACKAGES.md`](PACKAGES.md). The drivers beside them, `reach_<package>.hero`, run each package's tests and are no program of this list.

## Applications

Programs somebody would use, each doing one job through many packages; its demo is its test.

- [agent](agent/README.md): an AI assistant for a folder of documents, or a codebase's docs.
- [auth](auth/README.md): an identity server, a small Keycloak.
- [backup](backup/README.md): a deduplicating, encrypted backup tool in the family of restic and borg.
- [build](build/README.md): a build tool in make's and ninja's family.
- [certs](certs/README.md): a certificate manager in the manner of certbot.
- [chat](chat/README.md): a realtime chat service and its terminal client.
- [datakit](datakit/README.md): a data tool for the terminal, in the family of xsv, jq, Miller and a small pandas.
- [editor](editor/README.md): a terminal text editor in the manner of nano and micro.
- [explorer](explorer/README.md): a file manager in a window of the system's own widgets.
- [gateway](gateway/README.md): an API gateway in the manner of Envoy and Kong, in small.
- [ingest](ingest/README.md): an event pipeline in the manner of Kafka Connect's S3 sink and a Kafka Streams aggregation, in small.
- [inventory](inventory/README.md): stock management for a small chain of shops.
- [invoice](invoice/README.md): invoicing for a small business.
- [iothub](iothub/README.md): an IoT hub.
- [jq](jq/README.md): A small jq written with Rebel's `json/jq`: a filter run over the values of files, or of the standard input where no file is named, each value's outputs written one a line.
- [kanban](kanban/README.md): a desktop kanban board in Tauri's shape.
- [l10n](l10n/README.md): a translation management tool in gettext's family, for a program written in Heroes with HTML templates beside it.
- [lab](lab/README.md): a scientific calculator and notebook, in the family of a computer algebra system's REPL and of Pluto's reactive notebooks in small.
- [monitor](monitor/README.md): a system monitor with alerts and a terminal dashboard, in the family of htop, Netdata and supervisord in small.
- [notes](notes/README.md): a multi-user publishing service, a small blog and notes server written on Rebel's `web`.
- [registry](registry/README.md): the site that serves Heroes packages, and its client.
- [searchd](searchd/README.md): a search engine for one website, in the family of a small Meilisearch or Elasticsearch.
- [shell](shell/README.md): a POSIX-flavoured command shell.
- [sitegen](sitegen/README.md): a static site generator.
- [vcs](vcs/README.md): a small version control system in git's model, written from git's documented object model and behaviour.
- [worker](worker/README.md): a background job system on Redis, as Sidekiq and Celery are.

## Twins

Programs of `examples/`, the compiler's own, written again with Rebel: each prints its original's output byte for byte.

- [adventure](adventure/README.md), the twin of [`examples/adventure/`](../examples/adventure/): a text adventure played from a script, written with Rebel.
- [assembler](assembler/README.md), the twin of [`examples/assembler/`](../examples/assembler/): a tiny assembler, and the machine it assembles for, written with Rebel.
- [ini](ini/README.md), the twin of [`examples/ini/`](../examples/ini/): a configuration file read into sections, and the four ways a configuration file lies, written with Rebel.
- [interpreter](interpreter/README.md), the twin of [`examples/interpreter/`](../examples/interpreter/): a whole little language, scanned, parsed and run, written with Rebel.
- [json](json/README.md), the twin of [`examples/json/`](../examples/json/): a JSON document in, its shape and its canonical form out, written with Rebel.
- [ledger](ledger/README.md), the twin of [`examples/ledger/`](../examples/ledger/): a ledger kept in SQLite, and every number checked twice, written with Rebel.
- [logs](logs/README.md), the twin of [`examples/logs/`](../examples/logs/): a log summariser, a file of lines in and a page a person can read out, written with Rebel.
- [markdown](markdown/README.md), the twin of [`examples/markdown/`](../examples/markdown/): Markdown to plain text, written with Rebel.
- [maze](maze/README.md), the twin of [`examples/maze/`](../examples/maze/): the shortest way from `S` to `E`, drawn, written with Rebel.
- [query](query/README.md), the twin of [`examples/query/`](../examples/query/): a relational engine over a table this program writes, five thousand rows out to a file, read back, filtered, joined, ordered, grouped and counted, written with Rebel.
- [rpn](rpn/README.md), the twin of [`examples/rpn/`](../examples/rpn/): Reverse Polish evaluated on a stack, written with Rebel.
- [server](server/README.md), the twin of [`examples/server/`](../examples/server/): a small HTTP server, each connection served on a thread of its own, written with Rebel.
- [spreadsheet](spreadsheet/README.md), the twin of [`examples/spreadsheet/`](../examples/spreadsheet/): a file of cells, evaluated, written with Rebel.
- [todo](todo/README.md), the twin of [`examples/todo/`](../examples/todo/): a todo list kept in a file, written with Rebel.
- [wordbands](wordbands/README.md), the twin of [`examples/wordbands/`](../examples/wordbands/): words counted in text each thread writes for itself, written with Rebel.
- [wrap](wrap/README.md), the twin of [`examples/wrap/`](../examples/wrap/): text folded to a width, written with Rebel.

---

This page is written by `generators/rebel_index/`, never by hand, from each program's `main.hero` opening comment. Change that, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what the programs make.
