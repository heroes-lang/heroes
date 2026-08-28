#!/usr/bin/env python3
"""Serve site/dist over HTTPS on localhost, for previewing the site.

It serves `dist/`, which is what `npm run build` writes and what the deploy
uploads, so what is read here is the artifact and not its ingredients. Before
the site became an Astro project this served `public/`, and `public/` is now
the assets alone: serving it would answer 404 for every page.

Why this exists, and why it is not a `heroes` subcommand: CLAUDE.md §10 says
every new capability is a subcommand of the one binary, and §10's own stopping
rule is what keeps this out of it — a capability enters that surface only if the
fixpoint invocation, the golden harness or the Part 11 harness must type it, or
it has a measured Part 11 effect. Previewing a static directory is none of
those, and it is not a property of the language at all. It lives here, beside
the thing it serves, and outside both `public/` and `dist/` so it can never be
deployed.

Why it still exists next to `npm run dev`, which is faster: the dev server does
not answer on the site's real name over HTTPS, and that is the entire point of
this script. Every absolute URL on these pages is `https://heroes-lang.org/...`,
and under any other name none of them is exercised.

Why HTTPS rather than `python3 -m http.server`: the pages are meant to be read
the way a browser will really see them. Over plain HTTP a browser applies a
different security context, and anything that ever gets added to these pages —
a service worker, a clipboard call, a font from another origin — behaves
differently or not at all. Serving them over TLS locally removes that whole
class of "works on my machine, not on the real site" surprise before it starts.

    python3 site/serve.py                              # https://localhost:8443/
    sudo python3 site/serve.py --host heroes-lang.org  # the real name, port 443
    python3 site/serve.py --it                         # the Italian edition

The second form is the one worth having: with `127.0.0.1 heroes-lang.org` in
/etc/hosts the site answers on its own name, so every absolute URL, every
`canonical`, every `hreflang` and the whole two-edition switch behave exactly as
they will in public, and a mistake in any of them shows up here instead of after
the launch.

Certificates are per host, made on first run, and kept in site/.cache/, which
git ignores. If `mkcert` is installed the certificate is signed by its local CA
and the browser shows no warning at all; otherwise the script falls back to a
self-signed one from `openssl` and the browser warns once, correctly. Deleting
site/.cache/ is how you get fresh ones.

No dependencies beyond the standard library and the `openssl` that ships with
macOS. Nothing here is part of the build, and nothing here ships.
"""

from __future__ import annotations

import argparse
import http.server
import os
import shutil
import ssl
import subprocess
import sys
import webbrowser
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE / "dist"
CACHE = HERE / ".cache"
CERT_DAYS = 365


def ca_is_trusted() -> bool:
    """Is mkcert's root CA actually in the system trust store?

    `mkcert` creates its CA the first time it is used, but installing it into
    the trust store is a separate step that needs an admin password. Skip that
    step and every certificate it signs is untrusted, which looks exactly like a
    broken server. This is worth one cheap check so the script can say which of
    the two situations you are in.
    """
    try:
        r = subprocess.run(
            ["security", "find-certificate", "-c", "mkcert",
             "/Library/Keychains/System.keychain"],
            capture_output=True, text=True,
        )
        return r.returncode == 0
    except FileNotFoundError:
        return False


def ensure_certificate(host: str) -> tuple[Path, Path]:
    """Return a certificate and key for `host`, making them on first use.

    `subjectAltName` is the part that matters and the part everyone forgets: a
    certificate carrying only a common name is rejected outright by every
    browser released this decade, and the failure reads as a server bug rather
    than a certificate one. Both paths below set it.
    """
    cert, key = CACHE / f"{host}.pem", CACHE / f"{host}-key.pem"
    if cert.exists() and key.exists():
        return cert, key
    CACHE.mkdir(exist_ok=True)
    names = [host, "localhost", "127.0.0.1", "::1"]

    if shutil.which("mkcert"):
        print(f"making a certificate for {host} with mkcert (once)…")
        try:
            subprocess.run(
                ["mkcert", "-cert-file", str(cert), "-key-file", str(key), *names],
                check=True, capture_output=True, text=True,
            )
        except subprocess.CalledProcessError as e:
            sys.exit(f"mkcert failed:\n{e.stderr.strip()}")
        if not ca_is_trusted():
            print("  note: mkcert's CA is not in the system trust store yet, so")
            print("  the browser will still warn. Run this once, it asks for your")
            print("  password, and it is the only step this script cannot do:")
            print("      mkcert -install")
    else:
        print(f"making a self-signed certificate for {host} (once)…")
        san = ",".join(
            ("IP:" if n[0].isdigit() or ":" in n else "DNS:") + n for n in names
        )
        try:
            subprocess.run(
                ["openssl", "req", "-x509", "-newkey", "rsa:2048", "-nodes",
                 "-keyout", str(key), "-out", str(cert), "-days", str(CERT_DAYS),
                 "-subj", f"/CN={host}", "-addext", f"subjectAltName={san}"],
                check=True, capture_output=True, text=True,
            )
        except FileNotFoundError:
            sys.exit("neither mkcert nor openssl is on PATH.")
        except subprocess.CalledProcessError as e:
            sys.exit(f"openssl failed:\n{e.stderr.strip()}")
        print("  self-signed, so the browser warns once. `brew install mkcert`")
        print("  then `mkcert -install` removes the warning for good.")
    key.chmod(0o600)
    return cert, key


class Handler(http.server.SimpleHTTPRequestHandler):
    """Static files, with two changes.

    Nothing is cached, because a preview server that serves yesterday's CSS is
    worse than no preview server. And the log line is one line, so a reload does
    not scroll the terminal.
    """

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(ROOT), **kwargs)

    def end_headers(self):
        self.send_header("Cache-Control", "no-store, must-revalidate")
        super().end_headers()

    def log_message(self, fmt, *args):
        sys.stderr.write(f"  {self.address_string()}  {fmt % args}\n")


def main() -> None:
    ap = argparse.ArgumentParser(description="Preview site/dist over HTTPS.")
    ap.add_argument("--host", default="localhost",
                    help="name to bind and to certify (default: localhost)")
    ap.add_argument("--port", type=int, default=None,
                    help="default: 443 for a real host name, 8443 for localhost")
    ap.add_argument("--it", action="store_true", help="open the Italian edition")
    ap.add_argument("--no-open", action="store_true", help="do not open a browser")
    args = ap.parse_args()

    # A real host name is being served so it can be read at its real URL, and a
    # real URL has no port in it. localhost keeps the high port, because there
    # is no reason to ask for a password to look at a draft.
    port = args.port if args.port is not None else (
        8443 if args.host in ("localhost", "127.0.0.1") else 443)

    if not (ROOT / "index.html").is_file():
        sys.exit(
            f"{ROOT} does not look like the site: no index.html in it.\n"
            f"The site is built rather than served from source now:\n"
            f"    cd {HERE.name} && npm ci && npm run build")
    if port < 1024 and hasattr(os, "geteuid") and os.geteuid() != 0:
        sys.exit(
            f"port {port} is privileged, so this needs root:\n"
            f"    sudo python3 {Path(__file__).name} --host {args.host}\n"
            f"or pick a high port:\n"
            f"    python3 {Path(__file__).name} --host {args.host} --port 8443"
        )

    cert, key = ensure_certificate(args.host)
    ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ctx.load_cert_chain(certfile=cert, keyfile=key)

    try:
        httpd = http.server.ThreadingHTTPServer((args.host, port), Handler)
    except OSError as e:
        sys.exit(f"cannot bind {args.host}:{port} — {e}\n"
                 f"if the name is not in /etc/hosts, add:  127.0.0.1  {args.host}")
    httpd.socket = ctx.wrap_socket(httpd.socket, server_side=True)

    base = f"https://{args.host}" + ("" if port == 443 else f":{port}")
    page = f"{base}/it/index.html" if args.it else f"{base}/index.html"
    print(f"serving {ROOT}")
    print(f"  english   {base}/index.html")
    print(f"  italiano  {base}/it/index.html")
    print("ctrl-c to stop.")
    if not args.no_open:
        webbrowser.open(page)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nstopped.")
        httpd.server_close()


if __name__ == "__main__":
    main()
