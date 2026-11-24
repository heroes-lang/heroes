# auth

auth: an identity server, a small Keycloak. People sign up with a password
and prove their address by a mailed link, add TOTP from a QR code and
passkeys (WebAuthn), and log in to the applications registered in its
configuration by OpenID Connect: the authorization code with PKCE, ID and
access tokens as JWTs signed by a key the system's keyring keeps and
published as a JWK set, refresh tokens rotated with replay detection,
revocation and introspection. People of the organisation's LDAP directory
are imported every night and log in with their directory password; a
service account logs in by SCRAM-SHA-256 over HTTP to run the import.

    heroes build examples-rebel/auth/main.hero -o auth
    auth serve auth.toml          # serve, the configuration `core/settings` shows
    auth import auth.toml         # run the directory import once
    auth verifier directory-sync  # a service account's SCRAM verifier, the password on stdin
    auth demo build/auth/demo     # the demonstration, its folder made and removed

The demonstration runs every flow once on loopback, against stand-ins of a
mail server and a directory it carries, with a fixed clock and seeded
draws, and prints what each step asked and what came back.

Rebel's packages, and what for:

- `web` routes the server's requests; `http` reads and writes them, and is
  the client of the demonstration's browser and service account;
- `oauth2` is the demonstration's relying party (discovery, PKCE, the code's
  exchange, the ID token's verification, refresh, revocation,
  introspection), and computes the server's own S256 check;
- `jose` signs and verifies every JWT and writes the JWK set;
  `crypto/keypairs` makes the authenticator's keys sign; `crypto` hashes
  (Argon2id for passwords, SHA-256, HMAC and PBKDF2 for SCRAM, TOTP);
- `webauthn` verifies passkey registrations and assertions; `x509` checks an
  attestation's certificate chain; `asn1` writes the authenticator vendor's
  certificates and the directory stand-in's BER; `cbor` the authenticator's
  attestation objects; `pem` the certificates' files;
- `keyring` keeps the signing key and the SMTP and LDAP passwords; `mail`
  writes the verification message and reads it back; `mail/smtp` submits it;
  `ldap` reads the directory and binds as a person to check a password;
  `scram` is the service account's client; `qr` draws the TOTP enrolment;
- `random` and `uuid` make every identifier and token, `time` the clock and
  the import's cron schedule; `toml` reads the configuration; `json`, `fs`,
  `path`, `net`, `io`, `threads`, `os`, `url`, `strings`, `bytes`,
  `numbers` and `maps` do the rest.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/auth/main.hero -- demo build/auth/demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 33 faces on its `packages` line, each linked here to its reference: [`asn1`](https://heroes-lang.org/rebel/asn1/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`cbor`](https://heroes-lang.org/rebel/cbor/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`crypto/keypairs`](https://heroes-lang.org/rebel/crypto/keypairs/), [`fs`](https://heroes-lang.org/rebel/fs/), [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`jose`](https://heroes-lang.org/rebel/jose/), [`json`](https://heroes-lang.org/rebel/json/), [`keyring`](https://heroes-lang.org/rebel/keyring/), [`ldap`](https://heroes-lang.org/rebel/ldap/), [`mail`](https://heroes-lang.org/rebel/mail/), [`mail/smtp`](https://heroes-lang.org/rebel/mail/smtp/), [`maps`](https://heroes-lang.org/rebel/maps/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`oauth2`](https://heroes-lang.org/rebel/oauth2/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`pem`](https://heroes-lang.org/rebel/pem/), [`qr`](https://heroes-lang.org/rebel/qr/), [`random`](https://heroes-lang.org/rebel/random/), [`scram`](https://heroes-lang.org/rebel/scram/), [`strings`](https://heroes-lang.org/rebel/strings/), [`threads`](https://heroes-lang.org/rebel/threads/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/), [`url`](https://heroes-lang.org/rebel/url/), [`uuid`](https://heroes-lang.org/rebel/uuid/), [`web`](https://heroes-lang.org/rebel/web/), [`webauthn`](https://heroes-lang.org/rebel/webauthn/), [`x509`](https://heroes-lang.org/rebel/x509/).

## Its folders

13 folders, 43 files and 7,466 lines, this page aside; of them 40 modules, `.hero` files, of 7,322 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 306 | 1 | 162 |
| `accounts/` | 3 | 596 | 3 | 596 |
| `api/` | 4 | 1,102 | 4 | 1,102 |
| `core/` | 4 | 725 | 4 | 725 |
| `demo/` | 10 | 1,642 | 10 | 1,642 |
| `directory/` | 1 | 261 | 1 | 261 |
| `mail/` | 1 | 98 | 1 | 98 |
| `oauth/` | 4 | 729 | 4 | 729 |
| `passkeys/` | 2 | 261 | 2 | 261 |
| `server/` | 4 | 501 | 4 | 501 |
| `service/` | 1 | 237 | 1 | 237 |
| `standins/` | 4 | 887 | 4 | 887 |
| `totp/` | 1 | 121 | 1 | 121 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
