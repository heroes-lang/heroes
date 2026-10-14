import json, sys
for path in sys.argv[1:]:
    raw = open(path, encoding="utf-8", errors="replace").read()
    doc = json.loads(raw[raw.find("{"):raw.rfind("}")+1])
    print(path.split("/")[-1])
    for r in doc["runs"][0]["results"]:
        print("  ", r.get("level"), r.get("message", {}).get("text", "")[:40])
        for rl in r.get("relatedLocations", []):
            pl = rl.get("physicalLocation", {})
            print("      related:", pl.get("artifactLocation", {}).get("uri", "?").split("/")[-1] + ":" + str(pl.get("region", {}).get("startLine", "?")), "|", rl.get("message", {}).get("text", "")[:50])
