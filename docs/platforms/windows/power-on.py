#!/usr/bin/env python3
"""Turn on the project's Windows box by talking to appOnFly's own APIs.

The box is docs/platforms/windows/WINDOWS-MACHINE.md's, billed by the hour and
off between sessions. This program does what the portal's web page does, but
over plain HTTP with no browser: it signs in, finds the subscription's virtual
machine, starts it if it is stopped, waits until it reports Running, and arms
(or, with --hours, lengthens) the auto shutdown so the machine cannot sit on
the clock forever. Then it waits for the box to answer on SSH.

It uses only the Python standard library. Run it with -I so no site packages
and no $PYTHONPATH can shadow a stdlib module:

    python3 -I docs/platforms/windows/power-on.py            # start and wait for SSH
    python3 -I docs/platforms/windows/power-on.py --hours 8  # ... with 8 h before shutdown
    python3 -I docs/platforms/windows/power-on.py --dry-run  # read the state, change nothing

HOW IT WORKS, every call read from the portal's bundle (main.452fce04.js) and
confirmed against the live account on 2026-11-20:

  1. Sign in. POST to Firebase's identitytoolkit accounts:signInWithPassword
     with the portal's public web API key. The project does not enforce
     reCAPTCHA on password sign-in (probed: a wrong password returns
     INVALID_PASSWORD, not a challenge), so this needs no browser. It returns an
     idToken (a bearer token good for one hour), a refreshToken and the uid.

  2. Find the machine. A Firestore runQuery over customers/<uid>/subscriptions,
     filtered by status the way the page filters it, yields the subscription;
     its id is what the Cloud Functions take as subscriptionId, and its
     metadata.virtualMachineId names the VM.

  3. Read the live state. The VM's state is the .status field of the top-level
     Firestore document virtualMachines/<vmId> -- one of the portal's states
     (Running, Stopped, Starting, ...). POSTing subscription-checkstatus is only
     a nudge: it makes the backend re-read the cloud and rewrite that field,
     which this program then reads back.

  4. Start it. POST subscription-start {subscriptionId}; a 200 means accepted.
     Then poll virtualMachines/<vmId>.status, nudging with checkstatus, until it
     reads Running.

  5. Arm the auto shutdown. The timer is the autoShutdownTime field of
     customers/<uid>/shutdowns/<subId>; the backend stops the machine when that
     moment passes. The page writes it, so this program writes it too: on
     Running it sets autoShutdownTime to now plus the account's default
     (defaultAutoShutdownTime, e.g. "4h 0m"), or, with --hours H, to now plus H,
     but only where that lengthens it: a timer already set further, by another
     session for a lane still on the box, is left as it is. A start that armed
     nothing could leave the machine billing with no timer, so a real run always
     makes sure one is armed.

  6. Press Connect. POST subscription-select, what the portal's Connect button
     does. The author observed that the box is not truly up until Connect is
     pressed; whether SSH alone needs it is unrun, and --no-connect skips it.

  It never stops the machine: it has no stop, by design.

WHERE THE CREDENTIALS GO. The program never takes a password on the command
line and never prints one.

    APPONFLY_EMAIL     the account's email, in the environment or in the
                       repository's .env (KEY=value, one per line)
    the password       preferred: the login keychain, an item named
                       `apponfly-portal` whose account is that email --
                         security add-generic-password -s apponfly-portal \\
                           -a you@example.com -w
                       otherwise APPONFLY_PASSWORD, in the environment or .env.
                       .env is git-ignored and is sourced into every lane's
                       shell, so it is the natural home; the keychain keeps the
                       secret off disk entirely.

EXIT: 0 the machine is Running (and answering on SSH unless --no-ssh), or a dry
run that read the state; 1 the portal refused, a state this program does not
turn on, or a wait ran out; 2 the environment, the credentials or a bad option.
"""

from __future__ import annotations

import argparse
import datetime as dt
import json
import os
import re
import shutil
import subprocess
import sys
import time
import urllib.error
import urllib.request
from pathlib import Path

# --- what the portal exposes, all public (read from its web bundle) --------------

FIREBASE_API_KEY = "AIzaSyBsmgtKDJ_-ZJBbVrZVKHqYh5U2SVM9pjY"
PROJECT = "apponfly-prod"
FUNCTIONS = "https://europe-west2-apponfly-prod.cloudfunctions.net"
IDENTITY = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword"
SECURETOKEN = "https://securetoken.googleapis.com/v1/token"
FIRESTORE = f"https://firestore.googleapis.com/v1/projects/{PROJECT}/databases/(default)/documents"

ROOT = Path(__file__).resolve().parents[3]
KEYCHAIN_SERVICE = "apponfly-portal"
MAX_MINUTES = 24 * 60  # the portal's own ceiling for the shutdown timer

# The portal's machine states, spelled as its enum spells them, and what each
# one asks of this program.
GOAL = "Running"
PRESS = "Stopped"
WAIT = {"Starting", "Stopping", "ChangingPassword", "ResetingToSnapshot", "ResetingToFactory", "Creating", "PendingCreation"}
REFUSE = {"Deleting", "Unhealthy"}
STATES = {GOAL, PRESS} | WAIT | REFUSE
# The subscription statuses the page lists, so a canceled plan is still seen.
SUB_STATUSES = ["active", "incomplete", "canceled", "past_due", "trialing", "unpaid"]
# The ones whose machine can actually be turned on; a canceled or unpaid plan's
# VM is decommissioned, so it is never picked automatically.
STARTABLE_SUB = {"active", "trialing", "past_due", "incomplete"}


class Failure(Exception):
    def __init__(self, code: int, message: str) -> None:
        super().__init__(message)
        self.code = code


def fail(message: str):
    raise Failure(1, message)


def broken(message: str):
    raise Failure(2, message)


def log(line: str) -> None:
    print(f"[{time.strftime('%H:%M:%S')}] {line}", flush=True)


def hm(minutes: float) -> str:
    minutes = int(round(minutes))
    return f"{minutes // 60}h {minutes % 60}m"


# --- HTTP, on the standard library alone -----------------------------------------


def request(method: str, url: str, *, token: str | None = None, body: dict | None = None,
            timeout: float = 30) -> tuple[int, dict | str]:
    """One HTTP call. Returns (status, parsed-json-or-text). A non-2xx is
    returned, not raised, so each caller reads the portal's own error."""
    data = json.dumps(body).encode() if body is not None else None
    headers = {"Content-Type": "application/json"} if data is not None else {}
    if token:
        headers["Authorization"] = "Bearer " + token
    req = urllib.request.Request(url, data=data, headers=headers, method=method)
    try:
        with urllib.request.urlopen(req, timeout=timeout) as r:
            raw, status = r.read().decode("utf-8", "replace"), r.status
    except urllib.error.HTTPError as e:
        raw, status = e.read().decode("utf-8", "replace"), e.code
    except urllib.error.URLError as e:
        broken(f"cannot reach {urllib.parse.urlsplit(url).netloc}: {e.reason}")
    except TimeoutError:
        fail(f"{urllib.parse.urlsplit(url).netloc} did not answer in {timeout:.0f} s")
    if not raw:
        return status, ""
    try:
        return status, json.loads(raw)
    except json.JSONDecodeError:
        return status, raw


import urllib.parse  # noqa: E402  (used by request's error paths above)


# --- Firestore's wire shapes (the "typed value" encoding) ------------------------


def untype(value: dict):
    """One Firestore field value -> a plain Python value."""
    (kind, v), = value.items()
    if kind == "mapValue":
        return {k: untype(x) for k, x in (v.get("fields") or {}).items()}
    if kind == "arrayValue":
        return [untype(x) for x in (v.get("values") or [])]
    if kind == "integerValue":
        return int(v)
    if kind == "doubleValue":
        return float(v)
    if kind == "booleanValue":
        return bool(v)
    if kind == "nullValue":
        return None
    return v  # stringValue, timestampValue, referenceValue, ... stay strings


def fields_of(document: dict) -> dict:
    return {k: untype(v) for k, v in (document.get("fields") or {}).items()}


def rfc3339(moment: dt.datetime) -> str:
    return moment.astimezone(dt.timezone.utc).strftime("%Y-%m-%dT%H:%M:%S.%f")[:-3] + "Z"


def parse_rfc3339(text: str) -> dt.datetime:
    cleaned = re.sub(r"(\.\d{6})\d+", r"\1", text.replace("Z", "+00:00"))
    return dt.datetime.fromisoformat(cleaned)


# --- the account's session -------------------------------------------------------


class Session:
    """A signed-in session: the token, the uid, and the account's one VM."""

    def __init__(self, email: str, password: str, source: str) -> None:
        log(f"signing in as {email} (password from {source})")
        status, body = request("POST", f"{IDENTITY}?key={FIREBASE_API_KEY}",
                               body={"email": email, "password": password,
                                     "returnSecureToken": True, "clientType": "CLIENT_TYPE_WEB"})
        if status != 200:
            code = body.get("error", {}).get("message", body) if isinstance(body, dict) else body
            if code in ("INVALID_PASSWORD", "INVALID_LOGIN_CREDENTIALS", "EMAIL_NOT_FOUND"):
                broken("the portal rejected the email or password; check APPONFLY_EMAIL and the stored password")
            if isinstance(code, str) and code.startswith("TOO_MANY_ATTEMPTS"):
                broken("the portal is rate-limiting sign-in attempts; wait a while and try again")
            broken(f"sign-in failed: {code}")
        self.token = body["idToken"]
        self.refresh_token = body["refreshToken"]
        self.uid = body["localId"]
        self.token_deadline = time.monotonic() + max(int(body.get("expiresIn", "3600")) - 300, 60)
        self.subscription_id: str | None = None
        self.vm_id: str | None = None

    def fresh_token(self) -> str:
        """The idToken lasts an hour; a long wait refreshes it in time."""
        if time.monotonic() < self.token_deadline:
            return self.token
        status, body = request("POST", f"{SECURETOKEN}?key={FIREBASE_API_KEY}",
                               body={"grant_type": "refresh_token", "refresh_token": self.refresh_token})
        if status == 200 and isinstance(body, dict) and "id_token" in body:
            self.token = body["id_token"]
            self.refresh_token = body.get("refresh_token", self.refresh_token)
            self.token_deadline = time.monotonic() + max(int(body.get("expires_in", "3600")) - 300, 60)
        else:
            self.token_deadline = time.monotonic() + 60  # try again shortly
        return self.token

    def find_machine(self, chosen: str | None) -> None:
        """The subscription and its VM, from a Firestore query the rules allow."""
        query = {"structuredQuery": {
            "from": [{"collectionId": "subscriptions"}],
            "where": {"fieldFilter": {"field": {"fieldPath": "status"}, "op": "IN",
                                      "value": {"arrayValue": {"values": [{"stringValue": s} for s in SUB_STATUSES]}}}}}}
        status, body = request("POST", f"{FIRESTORE}/customers/{self.uid}:runQuery",
                               token=self.fresh_token(), body=query)
        if status != 200:
            fail(f"could not read the subscriptions (HTTP {status}): {body}")
        machines = []
        for row in body if isinstance(body, list) else []:
            doc = row.get("document")
            if not doc:
                continue
            f = fields_of(doc)
            vm_id = (f.get("metadata") or {}).get("virtualMachineId")
            if vm_id:
                # `product` is a Firestore reference path; its last segment is a
                # compact id, enough for a log line.
                product = (f.get("product") or "").split("/")[-1] or "a VM"
                machines.append((doc["name"].split("/")[-1], vm_id, f.get("status"), product))
        if not machines:
            fail("no subscription with a virtual machine on this account")
        if chosen:
            picked = [m for m in machines if m[0] == chosen]
            if not picked:
                listing = "\n".join(f"  {sid}  ({st})" for sid, _, st, _ in machines)
                fail(f"no subscription with id {chosen} on this account; it has:\n{listing}")
        else:
            # A canceled or unpaid plan's VM cannot be started, so pick among the
            # startable ones; fall back to all only if none is startable.
            picked = [m for m in machines if m[2] in STARTABLE_SUB] or machines
        if len(picked) > 1:
            listing = "\n".join(f"  {sid}  ({st})" for sid, _, st, _ in picked)
            fail(f"this account has {len(picked)} startable machines; name one with --subscription <id>:\n{listing}")
        self.subscription_id, self.vm_id, status, product = picked[0]
        log(f"machine: {product or 'a VM'} (subscription {self.subscription_id}, {status})")

    def vm_status(self) -> str | None:
        status, body = request("GET", f"{FIRESTORE}/virtualMachines/{self.vm_id}", token=self.fresh_token())
        if status == 404:
            return None
        if status != 200:
            fail(f"could not read the machine's state (HTTP {status}): {body}")
        return fields_of(body).get("status")

    def nudge(self) -> None:
        """Ask the backend to refresh the VM's state into Firestore."""
        request("POST", f"{FUNCTIONS}/subscription-checkstatus", token=self.fresh_token(),
                body={"subscriptionId": self.subscription_id})

    def refreshed_status(self) -> str | None:
        self.nudge()
        return self.vm_status()

    def start(self) -> None:
        status, body = request("POST", f"{FUNCTIONS}/subscription-start", token=self.fresh_token(),
                               body={"subscriptionId": self.subscription_id})
        if status != 200:
            fail(f"the portal refused the start (HTTP {status}): {body}")
        log("the portal accepted the start (HTTP 200)")

    def connect(self) -> str | None:
        """What the portal's Connect button does: POST subscription-select. The
        page redirects to the RDP gateway only as a side effect; the call itself
        returns {actionTaken, key, expiration}, and the user reports that
        pressing Connect is what makes the machine truly reachable, so the
        program makes this call too. The gateway key is not printed."""
        status, body = request("POST", f"{FUNCTIONS}/subscription-select", token=self.fresh_token(),
                               body={"subscriptionId": self.subscription_id})
        if status != 200:
            log(f"WARNING: Connect (subscription-select) returned HTTP {status}: {body}")
            return None
        action = body.get("actionTaken") if isinstance(body, dict) else None
        log(f"Connect (subscription-select): actionTaken={action!r}")
        return action

    # --- the auto shutdown, the one write a real run makes ----------------------

    def shutdown_doc(self) -> dict:
        status, body = request("GET", f"{FIRESTORE}/customers/{self.uid}/shutdowns/{self.subscription_id}",
                               token=self.fresh_token())
        return fields_of(body) if status == 200 else {}

    def minutes_left(self) -> int | None:
        when = self.shutdown_doc().get("autoShutdownTime")
        if not when:
            return None
        delta = (parse_rfc3339(when) - dt.datetime.now(dt.timezone.utc)).total_seconds() / 60
        return int(delta) if delta > 0 else 0

    def set_shutdown(self, minutes: int) -> None:
        """Write autoShutdownTime = now + minutes, as the page does on Running."""
        when = dt.datetime.now(dt.timezone.utc) + dt.timedelta(minutes=minutes)
        url = (f"{FIRESTORE}/customers/{self.uid}/shutdowns/{self.subscription_id}"
               "?updateMask.fieldPaths=autoShutdownTime&updateMask.fieldPaths=id")
        patch = {"fields": {"autoShutdownTime": {"timestampValue": rfc3339(when)},
                            "id": {"stringValue": self.subscription_id}}}
        status, body = request("PATCH", url, token=self.fresh_token(), body=patch)
        if status != 200:
            fail(f"could not arm the auto shutdown (HTTP {status}): {body}; "
                 "the machine may be running with no timer -- turn it off from the portal if so")


# --- credentials, never printed --------------------------------------------------


def dotenv() -> dict[str, str]:
    values: dict[str, str] = {}
    try:
        lines = (ROOT / ".env").read_text(encoding="utf-8").splitlines()
    except OSError:
        return values
    for line in lines:
        m = re.match(r"\s*(?:export\s+)?(APPONFLY_[A-Z_]+)\s*=\s*(.*?)\s*$", line)
        if m:
            v = m[2]
            if len(v) >= 2 and v[0] == v[-1] and v[0] in "\"'":
                v = v[1:-1]
            values[m[1]] = v
    return values


def keychain_password(email: str) -> str | None:
    if not shutil.which("security"):
        return None
    try:
        r = subprocess.run(["security", "find-generic-password", "-s", KEYCHAIN_SERVICE, "-a", email, "-w"],
                           capture_output=True, text=True, timeout=20)
    except subprocess.TimeoutExpired:
        log("the keychain did not answer in 20 s (a dialog may be waiting); skipping it")
        return None
    return r.stdout.rstrip("\n") if r.returncode == 0 and r.stdout.strip() else None


def credentials() -> tuple[str, str, str]:
    env = dotenv()

    def pick(key: str) -> str:
        return os.environ.get(key) or env.get(key) or ""

    email = pick("APPONFLY_EMAIL")
    if not email:
        broken("APPONFLY_EMAIL is not set; put it in the environment or in the repository's .env")
    password = keychain_password(email)
    if password:
        return email, password, f"the keychain item {KEYCHAIN_SERVICE}"
    password = pick("APPONFLY_PASSWORD")
    if password:
        return email, password, "APPONFLY_PASSWORD"
    broken(f"no password for {email}: add the keychain item {KEYCHAIN_SERVICE} "
           "(security add-generic-password -s apponfly-portal -a <email> -w) or set APPONFLY_PASSWORD")


# --- SSH, the real proof the box is up -------------------------------------------


def ssh_answers(host: str) -> bool:
    try:
        r = subprocess.run(["ssh", "-o", "BatchMode=yes", "-o", "ConnectTimeout=8", host, "echo power-on-ok"],
                           capture_output=True, text=True, timeout=25)
    except subprocess.TimeoutExpired:
        return False
    return r.returncode == 0 and "power-on-ok" in r.stdout


def tailnet_line(host: str) -> str | None:
    try:
        cfg = subprocess.run(["ssh", "-G", host], capture_output=True, text=True, timeout=10).stdout
        name = next((ln.split()[1] for ln in cfg.splitlines() if ln.startswith("hostname ")), host)
        ts = shutil.which("tailscale") or "/Applications/Tailscale.app/Contents/MacOS/Tailscale"
        out = subprocess.run([ts, "status"], capture_output=True, text=True, timeout=10).stdout
    except (OSError, subprocess.SubprocessError):
        return None
    line = next((s for s in out.splitlines() if re.search(rf"\s{re.escape(name)}\s", s)), None)
    return " ".join(line.split()) if line else None


def wait_for_ssh(host: str, seconds: float) -> None:
    log(f"waiting for {host} to answer on SSH, up to {hm(seconds / 60)}")
    t0 = time.monotonic()
    while not ssh_answers(host):
        if time.monotonic() - t0 > seconds:
            line = tailnet_line(host)
            fail(f"Running, but {host} did not answer on SSH within {hm(seconds / 60)}"
                 + (f"; Tailscale reads: {line}" if line else ""))
        time.sleep(10)
    log(f"{host} answers on SSH, {time.monotonic() - t0:.0f} s after Running")


# --- the command line ------------------------------------------------------------


def duration(text: str) -> int:
    m = re.fullmatch(r"\s*(?:(\d+(?:\.\d+)?)\s*h?)?\s*(?:(\d+)\s*m)?\s*", text)
    if not m or not (m[1] or m[2]):
        raise argparse.ArgumentTypeError(f'not a duration: "{text}" (try 6, 6.5, 6h30m or 90m)')
    minutes = round(float(m[1] or 0) * 60) + int(m[2] or 0)
    if not 0 < minutes <= MAX_MINUTES:
        raise argparse.ArgumentTypeError(f"between 1 minute and {MAX_MINUTES // 60} hours (the portal's ceiling)")
    return minutes


def positive_seconds(text: str) -> float:
    try:
        n = float(text)
    except ValueError:
        n = 0
    if n <= 0:
        raise argparse.ArgumentTypeError(f'a positive number of minutes, not "{text}"')
    return n * 60


def parse(argv) -> argparse.Namespace:
    p = argparse.ArgumentParser(
        prog="python3 -I docs/platforms/windows/power-on.py",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description="Turn on the Windows box through appOnFly's APIs, no browser.",
        epilog="credentials: APPONFLY_EMAIL (env or .env) and either the login keychain item "
               "`apponfly-portal` or APPONFLY_PASSWORD.")
    p.add_argument("--hours", type=duration, metavar="H",
                   help="arm the auto shutdown H from now instead of the account default (6, 6.5, 6h30m; at most 24)")
    p.add_argument("--subscription", metavar="ID", help="which subscription, if the account has more than one")
    p.add_argument("--dry-run", action="store_true", help="read and print the state, change nothing")
    p.add_argument("--no-connect", action="store_true",
                   help="skip the Connect step (subscription-select); start and arm the timer only")
    p.add_argument("--timeout", type=positive_seconds, default=600, metavar="MIN",
                   help="how long to wait for Running (default 10)")
    p.add_argument("--ssh", default="win", metavar="HOST", help="the SSH alias to wait for afterwards (default win)")
    p.add_argument("--no-ssh", action="store_true", help="do not wait for SSH")
    p.add_argument("--ssh-timeout", type=positive_seconds, default=600, metavar="MIN",
                   help="how long to wait for SSH (default 10)")
    return p.parse_args(argv)


# --- the run ---------------------------------------------------------------------


def wait_until_running(s: Session, first: str, timeout: float) -> None:
    deadline = time.monotonic() + timeout
    status = first
    accepted = None
    while status != GOAL:
        if status in REFUSE:
            fail(f"the machine is {status}, not one this program turns on; see https://app.apponfly.com/")
        if status == PRESS and accepted is None:
            s.start()
            accepted = time.monotonic()
        if status == PRESS and accepted is not None and time.monotonic() - accepted > 180:
            fail("the portal accepted the start and the machine still reads Stopped three minutes later")
        if time.monotonic() > deadline:
            fail(f"not Running after {hm(timeout / 60)}; the machine last read {status}")
        time.sleep(6)
        now = s.refreshed_status()
        if now and now != status:
            status = now
            log(f"the machine reads: {status}")


def arm(s: Session, hours: int | None) -> None:
    doc = s.shutdown_doc()
    enabled = doc.get("defaultAutoShutdownEnabled", True)
    if hours is None and not enabled:
        log("WARNING: the account's auto shutdown is off and no --hours was given; "
            "the machine will run until someone turns it off")
        return
    if hours is not None:
        target = hours
    else:
        default = doc.get("defaultAutoShutdownTime")  # e.g. "4h 0m"
        parsed = sum(int(n) * (60 if u == "h" else 1) for n, u in re.findall(r"(\d+)\s*([hm])", default or ""))
        target = parsed or 240
    # Only ever lengthen. Another session may have lengthened the timer for a
    # lane still using the box; writing now + target over it would shorten it
    # and stop the box under that lane.
    current = s.minutes_left()
    if current is not None and current >= target:
        log(f"auto shutdown already armed: about {hm(current)} left, at least the {hm(target)} asked; left as it is")
        return
    s.set_shutdown(target)
    left = s.minutes_left()
    if left is None or left < target - 3:
        fail(f"the auto shutdown did not take: the portal reads {hm(left) if left is not None else 'nothing'} left")
    log(f"auto shutdown armed: the machine stops in about {hm(left)}")


def run(argv=None) -> int:
    args = parse(argv)
    email, password, source = credentials()
    s = Session(email, password, source)
    s.find_machine(args.subscription)

    status = s.refreshed_status()
    if status is None:
        fail("the machine's state document does not exist yet; open the portal once in a browser")
    if status not in STATES:
        fail(f"the machine reads a state this program does not know: {status}")
    log(f"the machine reads: {status}")

    if args.dry_run:
        left = s.minutes_left()
        if status == PRESS:
            log(f"dry run: would start it, then arm the auto shutdown {hm(args.hours) if args.hours else '(account default)'}")
        else:
            log(f"dry run: nothing to start while {status}")
        if left is not None:
            log(f"the auto shutdown currently reads about {hm(left)} left")
        return 0

    wait_until_running(s, status, args.timeout)
    log("Running")
    arm(s, args.hours)
    if not args.no_connect:
        s.connect()  # what the portal's Connect button does; reported as needed to be truly up
    if not args.no_ssh:
        wait_for_ssh(args.ssh, args.ssh_timeout)
    return 0


if __name__ == "__main__":
    try:
        sys.exit(run())
    except Failure as e:
        print(f"power-on: {e}", file=sys.stderr)
        sys.exit(e.code)
    except KeyboardInterrupt:
        print("power-on: interrupted", file=sys.stderr)
        sys.exit(130)
