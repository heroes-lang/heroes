# invoice

invoice: invoicing for a small business. It keeps the company's customers
and items in SQLite, issues numbered invoices and credit notes with their
VAT computed per line and per country in exact decimals, writes each
document as a PDF drawn from a layout template, with the company's logo
and, on a euro invoice, the payment QR code of a SEPA credit transfer, and
as an XLSX sheet beside it, mails the PDF to the customer in the
customer's language, and at the year's end exports the accountant's CSV,
reports the totals by VAT rate and makes the year's archive of every
document, checked against its manifest.

Run for real:

    heroes build examples-rebel/invoice/main.hero -o invoice
    ./invoice init invoice.toml                  # the data folder and books
    ./invoice import invoice.toml customers customers.csv
    ./invoice import invoice.toml items items.csv
    ./invoice issue invoice.toml ROSSI 2026-03-02 BOOK-ATLAS=3 DESIGN-HOUR=6.5
    ./invoice issue invoice.toml HARBOR today BOOK-ATLAS=40 --rate 1.0850
    ./invoice credit invoice.toml INV-2026-0001 today BOOK-ATLAS=1
    ./invoice mail invoice.toml INV-2026-0001
    ./invoice export invoice.toml 2026 accounts-2026.csv
    ./invoice report invoice.toml 2026
    ./invoice archive invoice.toml 2026

`conf/settings.hero` documents the configuration file, which names the
data folder the books (`books.db`), the documents
(`documents/<year>/<number>.pdf` and `.xlsx`) and the archives
(`archives/<year>.tar.gz`) are kept in; `cli/commands.hero` documents
every command. The mail server's password stands in the environment,
`INVOICE_SMTP__PASSWORD`, never in the file. `render/sheet.hero` documents
the layout language a company's own layout is written in.

The demo, `./invoice demo` (`main.args`), starts on loopback the SMTP
server it carries (`standins/smtp_server`), which checks each message and
its PDF before it takes it, and runs a month of a publisher's invoicing
under a fixed clock: three customers in Italy, France and the United
States, three invoices and a credit note, two requests refused, every
document mailed and one login refused, the CSV, the totals and the
archive. Each document is told by digests of what its files hold read
back, never by a timestamp.

Rebel's packages, and what for:

- `big`: every amount, quantity, rate and exchange rate an exact decimal,
  and the IBAN's and the RF reference's check digits on integers of any
  length;
- `sql`, `sql/sqlite`: the books;
- `i18n`, `i18n/numbers`, `i18n/dates`: the customer's language, its
  catalog of words read from `.po` files, amounts with their currency and
  days with their month's name;
- `template`: the page's layout and the mail's subject and text;
- `pdf`, `image`, `image/png`, `qr`: the document drawn, the logo read and,
  in the demo, drawn, the payment code made and drawn;
- `xlsx`: the spreadsheet of each document;
- `mail`, `mail/smtp`: the message built, sent, and read by the stand-in;
- `csv`: the imports and the accountant's export;
- `archive`, `crypto`: the year's archive and its manifest of SHA-256;
- `config`, `json`: the configuration in layers, TOML's file under the
  environment, and the values a template reads;
- `net`, `io`, `os`: the stand-in's socket, the stop, the environment,
  the stand-in on a thread of the language's own (`spawn`, a mailbox);
- `fs`, `path`, `time`, `strings`, `bytes`, `numbers`: throughout.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/invoice/main.hero -- demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 28 faces on its `packages` line, each linked here to its reference: [`archive`](https://heroes-lang.org/rebel/archive/), [`big`](https://heroes-lang.org/rebel/big/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`config`](https://heroes-lang.org/rebel/config/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`csv`](https://heroes-lang.org/rebel/csv/), [`fs`](https://heroes-lang.org/rebel/fs/), [`i18n`](https://heroes-lang.org/rebel/i18n/), [`i18n/dates`](https://heroes-lang.org/rebel/i18n/dates/), [`i18n/numbers`](https://heroes-lang.org/rebel/i18n/numbers/), [`image`](https://heroes-lang.org/rebel/image/), [`image/png`](https://heroes-lang.org/rebel/image/png/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`mail`](https://heroes-lang.org/rebel/mail/), [`mail/smtp`](https://heroes-lang.org/rebel/mail/smtp/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`pdf`](https://heroes-lang.org/rebel/pdf/), [`qr`](https://heroes-lang.org/rebel/qr/), [`sql`](https://heroes-lang.org/rebel/sql/), [`sql/sqlite`](https://heroes-lang.org/rebel/sql/sqlite/), [`strings`](https://heroes-lang.org/rebel/strings/), [`template`](https://heroes-lang.org/rebel/template/), [`time`](https://heroes-lang.org/rebel/time/), [`xlsx`](https://heroes-lang.org/rebel/xlsx/).

## Its folders

16 folders, 38 files and 7,869 lines, this page aside; of them 35 modules, `.hero` files, of 7,744 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 212 | 1 | 87 |
| `bank/` | 3 | 578 | 3 | 578 |
| `billing/` | 2 | 315 | 2 | 315 |
| `books/` | 2 | 412 | 2 | 412 |
| `cli/` | 1 | 319 | 1 | 319 |
| `conf/` | 1 | 320 | 1 | 320 |
| `demo/` | 3 | 574 | 3 | 574 |
| `l10n/` | 2 | 434 | 2 | 434 |
| `mailing/` | 1 | 175 | 1 | 175 |
| `model/` | 2 | 443 | 2 | 443 |
| `money/` | 1 | 349 | 1 | 349 |
| `ops/` | 2 | 402 | 2 | 402 |
| `render/` | 6 | 1,426 | 6 | 1,426 |
| `standins/` | 3 | 575 | 3 | 575 |
| `store/` | 3 | 850 | 3 | 850 |
| `tax/` | 2 | 485 | 2 | 485 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
