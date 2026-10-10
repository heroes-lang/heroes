/* Defect 587, 2026-10-10: a macro under `#pragma clang deprecated`, bound as a constant. */
#define OLD_MAC 5
#pragma clang deprecated(OLD_MAC, "use NEW_MAC")
