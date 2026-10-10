# Panel 206, ffi-pragmatist's brief

Read `00-shared.md` first. Your seat measures the FFI's half: arithmetic over
a group's `constant` (`INT64_MAX + 1`, `UINT8_MAX` declared `u8`, a C
`#define` used as a count), whose value only clang knows; what `build`
already refuses there (`ffi_constant_type`) and what it lets abort at run
time; and for each route of the sitting, whether an extern constant joins
the refusal (asked of clang at build, as a probe), stays an abort, or is out
of reach, on this Mac and Linux arm64.
