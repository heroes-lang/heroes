/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * heroes_reserved_close.h: the names C reserves for the implementation that the
 * C this compiler writes holds, given back after it.
 *
 * NO HEADER REDEFINES A BUILTIN, A KEYWORD OR A PREDEFINED MACRO THE
 * COMPILER'S OWN C WRITES (defect 2333, 2026-11-08;
 * selfhost/emit/guarded_names.hero). `__builtin_add_overflow` defined to
 * report nothing, in the header a group names or in a `stdint.h` a program's
 * folder held, turned an `i64` sum past the top into `-9223372036854775808`
 * at exit 0; `_Static_assert` defined to nothing let a binding of the wrong
 * type build. So each name below is saved before a header the compiler does
 * not write is read and given back after it: around a unit's groups, inside
 * `heroes_guard_open.h` and `heroes_guard_close.h`; around the C library's
 * headers `heroes_standard.h` reads after them; and around the headers of
 * every probe an emitter module writes. With no include guard, since each
 * use saves and gives back again. The compiler holds the same list, and its
 * test holds both files to it. */
#pragma pop_macro("_Alignof")
#pragma pop_macro("_Atomic")
#pragma pop_macro("_Bool")
#pragma pop_macro("_CRT_SECURE_NO_WARNINGS")
#pragma pop_macro("_Generic")
#pragma pop_macro("_Noreturn")
#pragma pop_macro("_Static_assert")
#pragma pop_macro("_USE_MATH_DEFINES")
#pragma pop_macro("_WIN32")
#pragma pop_macro("__DBL_MANT_DIG__")
#pragma pop_macro("__DBL_MAX_EXP__")
#pragma pop_macro("__DBL_MIN_EXP__")
#pragma pop_macro("__FLT_EVAL_METHOD__")
#pragma pop_macro("__INT16_C_SUFFIX__")
#pragma pop_macro("__INT16_MAX__")
#pragma pop_macro("__INT16_TYPE__")
#pragma pop_macro("__INT32_C_SUFFIX__")
#pragma pop_macro("__INT32_MAX__")
#pragma pop_macro("__INT32_TYPE__")
#pragma pop_macro("__INT64_C_SUFFIX__")
#pragma pop_macro("__INT64_MAX__")
#pragma pop_macro("__INT64_TYPE__")
#pragma pop_macro("__INT8_C_SUFFIX__")
#pragma pop_macro("__INT8_MAX__")
#pragma pop_macro("__INT8_TYPE__")
#pragma pop_macro("__INTPTR_TYPE__")
#pragma pop_macro("__LDBL_MANT_DIG__")
#pragma pop_macro("__LDBL_MAX_EXP__")
#pragma pop_macro("__LDBL_MIN_EXP__")
#pragma pop_macro("__OPTIMIZE__")
#pragma pop_macro("__SANITIZE_ADDRESS__")
#pragma pop_macro("__UINT16_TYPE__")
#pragma pop_macro("__UINT32_TYPE__")
#pragma pop_macro("__UINT64_C_SUFFIX__")
#pragma pop_macro("__UINT64_TYPE__")
#pragma pop_macro("__UINT8_TYPE__")
#pragma pop_macro("__UINTPTR_TYPE__")
#pragma pop_macro("__asm__")
#pragma pop_macro("__atomic_always_lock_free")
#pragma pop_macro("__attribute__")
#pragma pop_macro("__builtin_add_overflow")
#pragma pop_macro("__builtin_classify_type")
#pragma pop_macro("__builtin_constant_p")
#pragma pop_macro("__builtin_frame_address")
#pragma pop_macro("__builtin_huge_val")
#pragma pop_macro("__builtin_memcpy")
#pragma pop_macro("__builtin_memmove")
#pragma pop_macro("__builtin_memset")
#pragma pop_macro("__builtin_mul_overflow")
#pragma pop_macro("__builtin_offsetof")
#pragma pop_macro("__builtin_return_address")
#pragma pop_macro("__builtin_strlen")
#pragma pop_macro("__builtin_sub_overflow")
#pragma pop_macro("__builtin_types_compatible_p")
#pragma pop_macro("__cleanup__")
#pragma pop_macro("__has_feature")
#pragma pop_macro("__noinline__")
#pragma pop_macro("__optnone__")
#pragma pop_macro("__typeof__")
