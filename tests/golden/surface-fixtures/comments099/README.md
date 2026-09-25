# comments099: a comment after the last item of a block stays in that block

Defect 099 (the parser seat that repaired defect 096, 2026-09-25, measuring
the shapes beside its reproducer). A `#` comment written at a block's
indent after its last item (a body's last statement, a group's last member,
a record's last field) was printed by the NEXT item's walk, at that item's
column. With a blank line under it, the comment left the block at exit 0,
the tree unchanged, so the tree guard could not see it: a remark about the
end of a body became a remark about the function below. Without one, the
next declaration took it as its doc and the guard refused at exit 2 a
program `check` accepts.

`inside.hero` holds every kind of block with such a comment after its last
item: an `if` block, a loop body, a match's arms, a body, a record's fields,
a variant's last case, a case's last field, a group record's last field with
a member following, and a group's last member. Measured one at a time on the
trunk's compiler the same day, the case's field and the group record's field
were refused at exit 2 and the other seven moved at exit 0. The file is
canonical: `fmt` prints it back byte for byte.

`tight.hero` is the refusal: the same comment directly above the next
declaration, after a body, a record's fields and a group's last member, and
three nested ends in one function. `fmt` exited 2 on each; it now prints each
comment as its block's last line and adds only the blank line that separates
two declarations, which is what `tests/harness/suite_surface.hero` pins as
the output.

The repair is `selfhost/print/page.hero`'s `block_end`, called as every block
closes, innermost first: a comment is the block's when it opens its line at
the block's column or deeper, with only blank lines between it and the line
printed last. A comment at a shallower column still goes to the item after it.
