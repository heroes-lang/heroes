/* Defect 2501, lane b34-fsl, 2026-11-13: twenty-two inline bodies, each
   calling a name nothing on the link line defines. */
int base2501_01(void);
int base2501_02(void);
int base2501_03(void);
int base2501_04(void);
int base2501_05(void);
int base2501_06(void);
int base2501_07(void);
int base2501_08(void);
int base2501_09(void);
int base2501_10(void);
int base2501_11(void);
int base2501_12(void);
int base2501_13(void);
int base2501_14(void);
int base2501_15(void);
int base2501_16(void);
int base2501_17(void);
int base2501_18(void);
int base2501_19(void);
int base2501_20(void);
int base2501_21(void);
int base2501_22(void);
static inline int top2501_01(void) { return base2501_01() + 1; }
static inline int top2501_02(void) { return base2501_02() + 2; }
static inline int top2501_03(void) { return base2501_03() + 3; }
static inline int top2501_04(void) { return base2501_04() + 4; }
static inline int top2501_05(void) { return base2501_05() + 5; }
static inline int top2501_06(void) { return base2501_06() + 6; }
static inline int top2501_07(void) { return base2501_07() + 7; }
static inline int top2501_08(void) { return base2501_08() + 8; }
static inline int top2501_09(void) { return base2501_09() + 9; }
static inline int top2501_10(void) { return base2501_10() + 10; }
static inline int top2501_11(void) { return base2501_11() + 11; }
static inline int top2501_12(void) { return base2501_12() + 12; }
static inline int top2501_13(void) { return base2501_13() + 13; }
static inline int top2501_14(void) { return base2501_14() + 14; }
static inline int top2501_15(void) { return base2501_15() + 15; }
static inline int top2501_16(void) { return base2501_16() + 16; }
static inline int top2501_17(void) { return base2501_17() + 17; }
static inline int top2501_18(void) { return base2501_18() + 18; }
static inline int top2501_19(void) { return base2501_19() + 19; }
static inline int top2501_20(void) { return base2501_20() + 20; }
static inline int top2501_21(void) { return base2501_21() + 21; }
static inline int top2501_22(void) { return base2501_22() + 22; }
