# certs

certs: a certificate manager in the manner of certbot. It keeps an ACME
account (RFC 8555) with a key in the system's keyring, obtains a
certificate for each set of domains its configuration names, proving each
name by HTTP-01 (a file served from a webroot, by a web server or by its
own responder) or DNS-01 (a TXT record written by RFC 2136 updates signed
with TSIG), the request built and signed here; keeps every version in
certbot's `live/` and `archive/` layout, with a renewal record a person
may annotate; renews each certificate when the CA's renewal window (RFC
9773) opens or when few days are left; revokes one; and serves HTTPS with
one.

Run for real, from a timer twice a day for `renew`:

    heroes build rebel/examples/certs/main.hero -o certs
    ./certs check certs.toml             # read the configuration, say what it holds
    ./certs issue certs.toml site        # obtain the certificate "site" now
    ./certs renew certs.toml             # renew every certificate that is due
    ./certs show certs.toml              # what is kept, and the days each has left
    ./certs revoke certs.toml site       # revoke "site" for its key's compromise
    ./certs respond certs.toml           # serve the webroot for HTTP-01, until Ctrl-C
    ./certs serve certs.toml             # serve HTTPS with [serve]'s certificate, until Ctrl-C
    ./certs secret certs.toml tsig < key # keep the TSIG secret, base 64, in the key store

`config/settings` documents the file; `main.stdin` is a whole one. The
demo, `./certs demo < main.stdin`, runs two months of a site's
certificates in seconds against stand-ins it starts on 127.0.0.1 (a DNS
authority, its own HTTP-01 responder, an ACME CA), and prints the
transcript `main.expected` holds.

Rebel's packages, and what for:

- `acme`: the client side of RFC 8555 and RFC 9773: accounts, orders,
  challenges' answers, finalize, download, revocation, renewal windows;
- `jose`: the account key as a JWK and its thumbprint, and the stand-in
  CA's check of every request's JWS;
- `x509`: requests and certificates read back, chains verified, a
  request's signature checked by the stand-in CA;
- `asn1`, `pem`, `crypto/keypairs`, `crypto`, `big`: certificate requests
  and the stand-in CA's certificates written in DER and signed, keys made,
  TSIG's HMAC;
- `dns`: RFC 2136 updates sent, the stand-in authority's messages, the
  stand-in CA's lookups of addresses, TXT and CAA records;
- `http`, `tls`, `net`, `io`: the CA's API over HTTPS, the HTTP-01
  responder, the HTTPS server and its client, the stand-ins' sockets;
- `keyring`: the private keys and the TSIG secret, files where no keyring
  answers; `toml`: the configuration, the renewal records, the demo's file
  edited in place;
- `time`: the injected clock, validities, renewal windows; `random`: the
  stand-in CA's seeded tokens, nonces and serials; `os`: the stop of the
  demo's services, which run on threads of the language's own (`spawn`,
  mailboxes); `fs`, `path`, `strings`, `bytes`,
  `json`, `numbers`: throughout.

## Its demo

Run from the repository's root with the words of `main.args` and `main.stdin` on its input, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/certs/main.hero -- demo < rebel/examples/certs/main.stdin

A line `main.varies` names is printed in another form where another machine decides it, each form written there.

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 25 faces on its `packages` line, each linked here to its reference: [`acme`](https://heroes-lang.org/rebel/acme/), [`asn1`](https://heroes-lang.org/rebel/asn1/), [`big`](https://heroes-lang.org/rebel/big/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`crypto/keypairs`](https://heroes-lang.org/rebel/crypto/keypairs/), [`dns`](https://heroes-lang.org/rebel/dns/), [`errors`](https://heroes-lang.org/rebel/errors/), [`fs`](https://heroes-lang.org/rebel/fs/), [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`jose`](https://heroes-lang.org/rebel/jose/), [`json`](https://heroes-lang.org/rebel/json/), [`keyring`](https://heroes-lang.org/rebel/keyring/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`pem`](https://heroes-lang.org/rebel/pem/), [`random`](https://heroes-lang.org/rebel/random/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/), [`tls`](https://heroes-lang.org/rebel/tls/), [`toml`](https://heroes-lang.org/rebel/toml/), [`x509`](https://heroes-lang.org/rebel/x509/).

## Its folders

11 folders, 42 files and 7,415 lines, this page aside; of them 37 modules, `.hero` files, of 7,282 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 6 | 443 | 1 | 310 |
| `challenges/` | 5 | 942 | 5 | 942 |
| `config/` | 2 | 530 | 2 | 530 |
| `core/` | 2 | 149 | 2 | 149 |
| `demo/` | 4 | 975 | 4 | 975 |
| `issue/` | 2 | 301 | 2 | 301 |
| `pki/` | 3 | 303 | 3 | 303 |
| `renew/` | 2 | 294 | 2 | 294 |
| `serve/` | 1 | 195 | 1 | 195 |
| `standins/` | 12 | 2,772 | 12 | 2,772 |
| `store/` | 3 | 511 | 3 | 511 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
