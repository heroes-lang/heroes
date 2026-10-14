# Route G: at each lend of the record to C, its gaps are zeroed in place, immediately before the call.
our $GAPS; BEGIN { my $d = $0; $d =~ s{[^/]*$}{}; require "${d}common.pl"; }
s/(#include "heroes_standard.h"\n)/$1$GAPS/;
s/^(\s*)((?:t\d+ = |\(void\))(?:write|send|memcmp|SHA256_Update)\(.*?\);)$/my ($i,$c)=($1,$2); my @r = ($c =~ m{&(h\d+_[abef]\b|\(\*ph0_x\))}g); join('', map { "${i}HERO_ZERO_GAPS_CD(&$_);\n" } @r) . "$i$c"/gme;
