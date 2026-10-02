#!/bin/bash
C=/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/185-critic
cd $C
f="$1"
HEROES_RUNTIME=$C/runtime ./heroes build "$f" --emit-c > /dev/null 2> /tmp/null-$$ ; e=$?
code=$(grep -o -m1 'error\[[a-z_]*\]' /tmp/null-$$ | head -1)
rm -f /tmp/null-$$
printf "%s\t%s\t%s\n" "$e" "$f" "$code"
