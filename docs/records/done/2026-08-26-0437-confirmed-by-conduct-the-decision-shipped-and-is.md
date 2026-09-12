- [x] M-selfhost-port, the CLI department | **CONFIRMED BY CONDUCT — the decision shipped and is load-bearing.** The prose below ended *"Veto or confirm"*; nobody vetoed and `system()` is how the compiler reaches clang today (`selfhost/cli_toolchain.hero:14`, and since M-separate-compilation step 5 every call routes through `cli_shell.shell` because an `extern` may not cross a module boundary). The decision verbatim:

  The language has read_file/write_file/args/exit and no subprocess form. The
  port's CLI needs to invoke clang and run the produced binary. The decision
  taken (revocable): bind C's own `system()` via the language's FFI —
  `extern "stdlib.h" function system(command: cstr) -> i64` — with clang's
  stderr redirected to a file under build/ and read back with read_file, so
  the ffi error mapping keeps working. This is §1.11 eating its own cooking:
  the compiler written in Heroes reaches its toolchain the way any Heroes
  program reaches C. The alternative (a runtime entry point wrapping
  posix_spawn with argv separation) is safer against quoting but grows the
  runtime; system() + careful quoting of the few paths we pass is v1. Veto or
  confirm.

  | selfhost/cli_toolchain.hero · selfhost/cli_shell.hero | the compiler written in Heroes reaches its toolchain the way any Heroes program reaches C, which is §1.11 eating its own cooking
