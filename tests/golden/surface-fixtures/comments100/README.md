# comments100: a comment that is not a doc is not printed where it reads as one

Defect 100 (the skeptic seat over defect 096's repair, 2026-09-25). The
parser takes as an item's doc the run of comments directly above it at its
own column (`selfhost/cursor.hero`'s `take_docs`). `fmt` prints every comment
above an item at that item's column, so a comment the parser did NOT take,
because it sat at another column or under a second `extern` head that `fmt`
merges away, came out directly above the item at its column, and the
re-parse took it: `check` exit 0, `fmt` exit 2, *changed the TREE*.

`members.hero` holds the shapes, each measured on the trunk's compiler the
same day at exit 2: a comment at column 0 between two members of a group,
between two fields and between two cases; a remark directly under a second
head of the same group; and a comment at column 4 above the file's first
declaration. `fmt` prints each at the item's column now with a blank line
under it, the one layout that keeps it a remark; the output is pinned in
`tests/harness/suite_surface.hero`.

`kepthead.hero` is the shape beside the second one: a doc above the second
head and a remark under it. Merged, the remark would stand between the
member and its doc and the re-parse would take it instead, so no layout that
keeps the comments in source order holds both; `fmt` keeps the second head
(`selfhost/print/page.hero`'s `keeps_head`, named `comment_under_doc` until
defect 101 gave it a second reason), where the trunk refused at exit 2. That
file is canonical.

The repair is `page.hero`'s `comments_above`, which every item that takes a
doc calls (a declaration, a group member, a field, a case): it prints the
item's doc directly above it, and a blank line under the last remark before
it wherever the source has one or the re-parse would otherwise read it as
the doc. A `##` heading is never read as one, so it gains none.
