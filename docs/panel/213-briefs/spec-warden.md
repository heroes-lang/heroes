# Panel 213, the spec-warden's brief

Read `00-shared.md` first. Your seat is the indicator, design.md §1.2's cost and
§1.6's budget, and Principle 0's burden of proof (`.claude/agents/spec-warden.md`).

**Your task**:
1. **Does any route move a token of `spec/heroes-spec.md`?** The spec says
   nothing of debuggers today (`grep -n -i -E "debug|lldb" spec/heroes-spec.md`
   in your copy; say what it returns). If a route must be stated there (a
   `heroes` flag is not the spec's), price it with `heroes measure` (offline,
   vendored; `--refresh` is a paid run you may not start, so a price is a lower
   bound, in those words).
2. **Principle 0** (CLAUDE.md § 2): the milestone is scheduled with no warrant
   (its page says so). Which route's cost is justified by what, and which would
   be compiler size bought for nothing? design.md's sentence *No typed variable
   inspection in v1* (`grep -n "typed variable" docs/design.md`: line 630, in
   Part 2, the non-goals): say whether this milestone retires it, and with what.
3. **The tool surface as a budget**: CLAUDE.md § 10 and
   `.claude/rules/cli-surface.md`. A flag, a verb, a file beside the binary or
   nothing: which one the stopping rule admits, applied to the routes as they
   would actually be built, and whether *never a script* reads on a file the
   build writes and nobody types.
4. A prediction scored by an instrument that exists (the `lldb` suite, a count,
   a compile), with the milestone that scores it.
