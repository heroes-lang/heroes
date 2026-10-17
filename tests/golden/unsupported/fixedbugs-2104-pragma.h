/* Defect 2104: a value whose own _Pragma switches off the warning the value
   check makes an error. */
#define P_SHIFT (_Pragma("clang diagnostic ignored \"-Wshift-count-overflow\"") 1 << 40)
