# Minimal DAP client: launch, one breakpoint, print the Locals scope's variables
# and one level of children. argv: lldb-dap program source line [initCommand...]
import json, subprocess, sys, threading, queue

dap, prog, src, line = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4])
init = sys.argv[5:]
p = subprocess.Popen([dap], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
q = queue.Queue()
seq = [0]

def reader():
    while True:
        hdr = b""
        while not hdr.endswith(b"\r\n\r\n"):
            c = p.stdout.read(1)
            if not c:
                q.put(None); return
            hdr += c
        n = int([l for l in hdr.split(b"\r\n") if l.lower().startswith(b"content-length")][0].split(b":")[1])
        q.put(json.loads(p.stdout.read(n)))

threading.Thread(target=reader, daemon=True).start()

def send(cmd, args):
    seq[0] += 1
    b = json.dumps({"seq": seq[0], "type": "request", "command": cmd, "arguments": args}).encode()
    p.stdin.write(b"Content-Length: %d\r\n\r\n" % len(b) + b); p.stdin.flush()
    return seq[0]

def wait(pred, timeout=60):
    while True:
        m = q.get(timeout=timeout)
        if m is None: raise SystemExit("dap exited")
        if m.get("type") == "event" and m.get("event") == "output":
            o = m["body"].get("output", "")
            if "rror" in o or "Traceback" in o: sys.stdout.write("[output] " + o)
        if pred(m): return m

def req(cmd, args):
    s = send(cmd, args)
    return wait(lambda m: m.get("type") == "response" and m.get("request_seq") == s)

req("initialize", {"adapterID": "lldb-dap", "clientID": "probe", "linesStartAt1": True, "columnsStartAt1": True, "pathFormat": "path"})
s = send("launch", {"program": prog, "initCommands": init, "stopOnEntry": False, "disableASLR": False})
wait(lambda m: m.get("event") == "initialized")
req("setBreakpoints", {"source": {"path": src}, "breakpoints": [{"line": line}]})
req("configurationDone", {})
wait(lambda m: m.get("event") == "stopped")
th = req("threads", {})["body"]["threads"][0]["id"]
fr = req("stackTrace", {"threadId": th, "levels": 1})["body"]["stackFrames"][0]
print("frame:", fr["name"], fr.get("line"))
for sc in req("scopes", {"frameId": fr["id"]})["body"]["scopes"]:
    if sc["name"] not in ("Locals", "Local"): continue
    for v in req("variables", {"variablesReference": sc["variablesReference"]})["body"]["variables"]:
        print("%s = %s   [%s]" % (v["name"], v["value"], v.get("type", "")))
        if v.get("variablesReference"):
            for c in req("variables", {"variablesReference": v["variablesReference"]})["body"]["variables"]:
                print("    %s = %s" % (c["name"], c["value"]))
send("disconnect", {"terminateDebuggee": True})
p.wait(timeout=20)
