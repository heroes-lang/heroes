function brief(s,   r) { r = s
  if (match(r, /panic: stack exhausted/)) t = "told"; else if (match(r, /ran on a thread/)) t = "THREAD-MSG"; else if (match(r, /ERROR: AddressSanitizer: stack-overflow/)) t = "asan-overflow"; else if (match(r, /nested bug/)) t = "nested"; else t = ""
  w = ""; if (match(r, /: [0-9]+ bytes of/)) { w = substr(r, RSTART+2, RLENGTH-11) }
  low = ""; if (match(r, /lowest [0-9]+/)) low = substr(r, RSTART+7, RLENGTH-7)
  return t " w=" w "/" low }
{ key = $2 " KB" substr($3,3) " " $4; ex = $5 " " $6; rest = $0; sub(/^.{60}/, "", rest)
  if ($1 == "base") { b[NR] = key; bv[key] = ex " " brief(rest) } else { printf "%-34s base: %-34s c1: %s %s\n", key, bv[key], ex, brief(rest) } }
