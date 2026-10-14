# Route F: every by-value crossing of the record is re-zeroed in its gaps where it arrives:
# the callee's parameter on entry, the caller's slot after the return.
our $GAPS; BEGIN { my $d = $0; $d =~ s{[^/]*$}{}; require "${d}common.pl"; }
s/(#include "heroes_standard.h"\n)/$1$GAPS/;
s/(struct cd h_rec_through\(struct cd h0_x\) \{.*?bb0:\n)/$1    HERO_ZERO_GAPS_CD(&h0_x);\n/s;
s/^(\s*)((t\d+) = h_rec_through\(t\d+\);)$/$1$2 HERO_ZERO_GAPS_CD(&$3);/gm;
