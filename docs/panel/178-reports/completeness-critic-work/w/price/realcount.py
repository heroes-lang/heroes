# Panel 178 spec-warden: a replica of `heroes measure --refresh`'s request (selfhost/cli/refresh.hero,
# selfhost/cli/count_tokens.hero): claude-opus-5, one user message, minus (count("x") - 1),
# refused unless count("x x x x x") - count("x") == 4. The key is read from the environment, never printed.
import json, os, sys, urllib.request
URL = "https://api.anthropic.com/v1/messages/count_tokens"
def count(content):
    body = json.dumps({"model": "claude-opus-5", "messages": [{"role": "user", "content": content}]}).encode()
    req = urllib.request.Request(URL, data=body, headers={"x-api-key": os.environ["ANTHROPIC_API_KEY"],
        "anthropic-version": "2023-06-01", "content-type": "application/json"})
    return json.load(urllib.request.urlopen(req))["input_tokens"]
one, five = count("x"), count("x x x x x")
if five - one != 4:
    sys.exit(f"probes {one} {five}: refuse to subtract")
off = one - 1
for p in sys.argv[1:]:
    print(f"{count(open(p).read()) - off}\t{p}")
