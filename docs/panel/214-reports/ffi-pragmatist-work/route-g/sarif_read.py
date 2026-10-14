import json, sys, re
for path in sys.argv[1:]:
    raw = open(path, encoding="utf-8", errors="replace").read()
    start = raw.find("{")
    end = raw.rfind("}")
    tail = raw[end+1:].strip().replace("\n", " | ")
    head = raw[:start].strip().replace("\n", " | ")
    doc = json.loads(raw[start:end+1])
    res = doc["runs"][0]["results"]
    print(path.split("/")[-1], "results:", len(res), "| before JSON:", head[:90] or "-", "| after JSON:", tail[:60] or "-")
    for r in res:
        loc = r.get("locations", [{}])[0].get("physicalLocation", {})
        uri = loc.get("artifactLocation", {}).get("uri", "?")
        line = loc.get("region", {}).get("startLine", "?")
        rel = r.get("relatedLocations", [])
        msg = r.get("message", {}).get("text", "")[:55]
        print("   ", r.get("level"), uri.split("/")[-1] + ":" + str(line), "related=" + str(len(rel)), "|", msg)
