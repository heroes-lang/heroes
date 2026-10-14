#!/bin/bash
mkdir -p /tmp/p && cd /tmp/p && cp /w/purec_a.c /w/purec_b.c .
clang -g -O2 -fsanitize=memory -fsanitize-recover=memory purec_a.c purec_b.c -o p
MSAN_OPTIONS=halt_on_error=0 ./p > /dev/null 2> m.txt
grep -E "WARNING|#[0-4] |Uninitialized value was" m.txt | sed -E 's/0x[0-9a-f]+/0x?/g; s#/tmp/p/##'
