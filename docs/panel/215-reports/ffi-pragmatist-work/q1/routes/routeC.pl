# Route C: a group record crosses a Heroes call by address, and comes back through the caller's slot.
# perl -0pi routeC.pl main.c rec.c
our $GAPS; BEGIN { my $d = $0; $d =~ s{[^/]*$}{}; require "${d}common.pl"; }
s/struct cd h_rec_through\(struct cd h0_x\);/void h_rec_through(struct cd *hero_ret, const struct cd *hero_px);/g;
s/struct cd h_rec_through\(struct cd h0_x\) \{(.*?)t1 = h0_x;(.*?)return t1;/void h_rec_through(struct cd *hero_ret, const struct cd *hero_px) {$1t1 = *hero_px;$2*hero_ret = t1; return;/s;
s/(t\d+) = h_rec_through\((t\d+)\);/h_rec_through(&$1, &$2);/g;
