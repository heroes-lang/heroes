#!/bin/bash
# Panel 173, ffi-pragmatist: everything the Linux row needs, in one foreground run.
# /src is the prototype copy, /stock the stock copy, both mounted read-only.
set -u
mkdir -p /w/proto /w/stock
tar -C /src --exclude=.git --exclude=build -cf - . | tar -C /w/proto -xf -
tar -C /stock --exclude=.git --exclude=build -cf - . | tar -C /w/stock -xf -
echo "== machine"; uname -m; ldd --version | head -1; clang --version | head -1
cd /w/stock && clang -I runtime seed/heroes.c runtime/runtime.c -o heroes; echo "stock seed: $?"
cd /w/proto && clang -I runtime seed/heroes.c runtime/runtime.c -o heroes; echo "proto seed: $?"
B=/w/proto/docs/panel/173-briefs
run10() {
  local bin=$1; shift
  printf "%s x10: " "$(basename "$bin")"
  for i in $(seq 10); do "$bin" "$@" >/dev/null 2>/w/e; printf "%s/%sB " $? "$(wc -c </w/e)"; done; echo
  echo "   stderr of last run:"; sed 's/^/   | /' /w/e
}
for prog in lease070 control b_later b_callback b_out panic_lease; do
  echo "== $prog"
  cd /w/stock && CPATH=$B ./heroes build $B/$prog.hero -o /w/s_$prog 2>/w/be; echo "stock build: $? ($(wc -c </w/be)B)"; head -3 /w/be
  cd /w/proto && CPATH=$B ./heroes build $B/$prog.hero -o /w/p_$prog 2>/w/be; echo "proto build: $? ($(wc -c </w/be)B)"; head -3 /w/be
  [ -x /w/s_$prog ] && run10 /w/s_$prog
  [ -x /w/p_$prog ] && run10 /w/p_$prog
done
echo "== --sanitize lease070"
cd /w/stock && CPATH=$B ./heroes build --sanitize $B/lease070.hero -o /w/s_san 2>/w/be; echo "stock san build: $? ($(wc -c </w/be)B)"; head -3 /w/be
cd /w/proto && CPATH=$B ./heroes build --sanitize $B/lease070.hero -o /w/p_san 2>/w/be; echo "proto san build: $? ($(wc -c </w/be)B)"; head -3 /w/be
[ -x /w/s_san ] && run10 /w/s_san
[ -x /w/p_san ] && run10 /w/p_san
echo "== --sanitize control (LeakSanitizer composes?)"
cd /w/proto && CPATH=$B ./heroes build --sanitize $B/control.hero -o /w/p_san_control 2>/w/be; echo "proto san control build: $? ($(wc -c </w/be)B)"
[ -x /w/p_san_control ] && run10 /w/p_san_control
echo "== variants.c"
clang -o /w/variants $B/variants.c 2>/dev/null; echo "clang variants: $?"
for w in 0 1 2 3 4; do printf "variants %s x3: " $w; for i in $(seq 3); do /w/variants $w >/dev/null 2>/w/e; printf "%s/%sB " $? "$(wc -c </w/e)"; done; echo; sed 's/^/   | /' /w/e; done
echo "== sigprobe.c"
clang -o /w/sigprobe $B/sigprobe.c 2>/dev/null; echo "clang sigprobe: $?"
for w in 0 1 2 3 4 5; do printf "sigprobe %s: " $w; /w/sigprobe $w >/dev/null 2>/w/e; echo "exit $?"; sed 's/^/   | /' /w/e; done
grep -n 'define SI_USER\|define SI_TKILL\|define TRAP_BRKPT\|define SI_KERNEL' /usr/include/*/bits/siginfo-consts.h /usr/include/bits/siginfo-consts.h 2>/dev/null | head
echo "== q3 q4 q5 (prototype)"
for prog in q3_sqlite_ptr q3_sqlite_cstr q3_free_ptr q3_free_cstr q4_main q4_ctor q5_cabort q5_ctrap q5_own q5_own_interior q5_overflow q5_raise; do
  echo "== $prog"
  rm -f /w/p_$prog
  cd /w/proto && CPATH=$B ./heroes build $B/$prog.hero -o /w/p_$prog 2>/w/be; echo "proto build: $? ($(wc -c </w/be)B)"; head -4 /w/be
  [ -x /w/p_$prog ] && run10 /w/p_$prog
done
echo "== q4_ctor with LIB_CTOR=1 (library installs before the runtime)"
[ -x /w/p_q4_ctor ] && LIB_CTOR=1 run10 /w/p_q4_ctor
echo "== q4 on the stock runtime"
for prog in q4_main q4_ctor; do
  cd /w/stock && CPATH=$B ./heroes build $B/$prog.hero -o /w/s_$prog 2>/w/be; echo "stock build $prog: $? ($(wc -c </w/be)B)"
  [ -x /w/s_$prog ] && run10 /w/s_$prog
done
[ -x /w/s_q4_ctor ] && LIB_CTOR=1 run10 /w/s_q4_ctor
echo "== done"
