#!/bin/sh
# The critic: one case's verdict in a platform-free form: the codes `check`
# prints, its exit, the build's exit, and every line the binary printed.
awk '
/^error\[/ { match($0, /^error\[[a-z_]+\]/); print substr($0, RSTART, RLENGTH); next }
/^check exit/ { print; next }
/^build .*exit/ { print; inrun = 1; next }
inrun && /^run exit/ { print; inrun = 0; next }
inrun && !/^(internal error|.*\.c:[0-9]+:|   *[0-9]* \||  *\^|.*error:|.*warning:|.*note:|wrote )/ { print "  " $0 }
' "$1"
