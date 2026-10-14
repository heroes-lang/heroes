# Route D: C receives a copy of the record, field by field into a temporary zeroed whole first.
our $GAPS; BEGIN { my $d = $0; $d =~ s{[^/]*$}{}; require "${d}common.pl"; }
s/(#include "heroes_standard.h"\n)/$1$GAPS static inline struct cd *hero_d_cd(struct cd *z, const struct cd *s) { memset(z, 0, sizeof *z); z->c = s->c; z->d = s->d; return z; }\n/;
s/^(\s*(?:t\d+ = |\(void\))(?:write|send|memcmp|SHA256_Update)\(.*?\);)$/my $c=$1; $c =~ s{&(h\d+_[abef]\b|\(\*ph0_x\))}{hero_d_cd(&(struct cd){0}, &$1)}g; $c/gme;
