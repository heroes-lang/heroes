#!/usr/bin/env python3
"""Serve site/public over HTTPS on localhost, for previewing the site.

Why this exists, and why it is not a `heroes` subcommand: CLAUDE.md §10 says
every new capability is a subcommand of the one binary, and §10's own stopping
rule is what keeps this out of it — a capability enters that surface only if the
fixpoint invocation, the golden harness or the Part 11 harness must type it, or
it has a measured Part 11 effect. Previewing a static directory is none of
those, and it is not a property of the language at all. It lives here, beside
the thing it serves, and outside `public/` so it can never be deployed.

Why HTTPS rather than `python3 -m http.server`: the pages are meant to be read
the way a browser will really see them. Over plain HTTP a browser applies a
different security context, and anything that ever gets added to these pages —
a service worker, a clipboard call, a font from another origin — behaves
differently or not at all. Serving them over TLS locally removes that whole
class of "works on my machine, not on the real site" surprise before it starts.

    python3 site/serve.py              # https://localhost:8443/
    python3 site/serve.py --port 9443
    python3 site/serve.py --it         # open the Italian edition instead

The certificate is self-signed and made on first run, so the browser will warn
once. That warning is correct and you accept it: the certificate says nothing
except "this is localhost". It is written to site/.cache/, which is ignored by
git, and deleting that directory is how you get a fresh one.

No dependencies beyond the standard library and the `openssl` that ships with
macOS. Nothing here is part of the build, and nothing here ships.
"""

from __future__ import annotations

import argparse
import http.server
import os
import ssl
import subprocess
import sys
import webbrowser
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE / "public"
CACHE = HERE / ".cache"
CERT = CACHE / "localhost.pem"
KEY = CACHE / "localhost-key.pem"
CERT_DAYS = 365


def ensure_certificate() -> None:
    """Make a self-signed localhost certificate, once.

    `subjectAltName` is the part that matters and the part everyone forgets: a
    certificate with only a common name is rejected outright by every browser
    released this decade, and the failure looks like a server bug rather than a
    certificate one.
    """
    if CERT.exists() and KEY.exists():
        return
    CACHE.mkdir(exist_ok=True)
    print("making a self-signed certificate for localhost (once)…")
    try:
        subprocess.run(
            [
                "openssl", "req", "-x509", "-newkey", "rsa:2048", "-nodes",
                "-keyout", str(KEY), "-out", str(CERT),
                "-days", str(CERT_DAYS), "-subj", "/CN=localhost",
                "-addext", "subjectAltName=DNS:localhost,IP:127.0.0.1",
            ],
            check=True, capture_output=True, text=True,
        )
    except FileNotFoundError:
        sys.exit("openssl is not on PATH, so the certificate cannot be made.")
    except subprocess.CalledProcessError as e:
        sys.exit(f"openssl failed:\n{e.stderr.strip()}")
    KEY.chmod(0o600)


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
    ap = argparse.ArgumentParser(description="Preview site/public over HTTPS.")
    ap.add_argument("--port", type=int, default=8443)
    ap.add_argument("--host", default="localhost")
    ap.add_argument("--it", action="store_true", help="open the Italian edition")
    ap.add_argument("--no-open", action="store_true", help="do not open a browser")
    args = ap.parse_args()

    if not (ROOT / "index.html").is_file():
        sys.exit(f"{ROOT} does not look like the site: no index.html in it.")

    ensure_certificate()

    ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ctx.load_cert_chain(certfile=CERT, keyfile=KEY)

    httpd = http.server.ThreadingHTTPServer((args.host, args.port), Handler)
    httpd.socket = ctx.wrap_socket(httpd.socket, server_side=True)

    base = f"https://{args.host}:{args.port}"
    page = f"{base}/it/index.html" if args.it else f"{base}/index.html"
    print(f"serving {ROOT}")
    print(f"  english   {base}/index.html")
    print(f"  italiano  {base}/it/index.html")
    print("the certificate is self-signed: accept the warning once. ctrl-c to stop.")
    if not args.no_open:
        webbrowser.open(page)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nstopped.")
        httpd.server_close()


if __name__ == "__main__":
    main()
