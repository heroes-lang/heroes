#include "shapes.h"
#line 1 "hero_lk_ty"
typedef pthread_mutex_t hero_lk_t_0; extern hero_lk_t_0 hero_lk_v_0; _Static_assert(__builtin_classify_type(hero_lk_v_0) == 12 || __builtin_classify_type(hero_lk_v_0) == 13, "hero_lk_ty");
#line 2 "hero_lk_ty"
typedef sem_t hero_lk_t_1; extern hero_lk_t_1 hero_lk_v_1; _Static_assert(__builtin_classify_type(hero_lk_v_1) == 12 || __builtin_classify_type(hero_lk_v_1) == 13, "hero_lk_ty");
#line 3 "hero_lk_ty"
typedef CRITICAL_SECTION hero_lk_t_2; extern hero_lk_t_2 hero_lk_v_2; _Static_assert(__builtin_classify_type(hero_lk_v_2) == 12 || __builtin_classify_type(hero_lk_v_2) == 13, "hero_lk_ty");
#line 4 "hero_lk_ty"
typedef pthread_cond_t hero_lk_t_3; extern hero_lk_t_3 hero_lk_v_3; _Static_assert(__builtin_classify_type(hero_lk_v_3) == 12 || __builtin_classify_type(hero_lk_v_3) == 13, "hero_lk_ty");
#line 1 "hero_lk_has"
typedef __typeof__(((struct counter *)0)->lock) hero_lk_m_0;
#line 2 "hero_lk_has"
typedef __typeof__(((struct counter *)0)->n) hero_lk_m_1;
#line 3 "hero_lk_has"
typedef __typeof__(((struct counter *)0)->nosuch) hero_lk_m_2;
#line 4 "hero_lk_has"
typedef __typeof__(((struct arr *)0)->many[0]) hero_lk_m_3;
#line 5 "hero_lk_has"
typedef __typeof__(((struct arr *)0)->many[0].lock) hero_lk_m_4;
#line 6 "hero_lk_has"
typedef __typeof__(((struct anon *)0)->m) hero_lk_m_5;
#line 7 "hero_lk_has"
typedef struct cond hero_lk_m_6;
#line 1 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_0, hero_lk_t_0), "hero_lk_is");
#line 2 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_0, hero_lk_t_1), "hero_lk_is");
#line 3 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_0, hero_lk_t_2), "hero_lk_is");
#line 4 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_0, hero_lk_t_3), "hero_lk_is");
#line 5 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_1, hero_lk_t_0), "hero_lk_is");
#line 6 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_1, hero_lk_t_1), "hero_lk_is");
#line 7 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_1, hero_lk_t_2), "hero_lk_is");
#line 8 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_1, hero_lk_t_3), "hero_lk_is");
#line 9 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_2, hero_lk_t_0), "hero_lk_is");
#line 10 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_2, hero_lk_t_1), "hero_lk_is");
#line 11 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_2, hero_lk_t_2), "hero_lk_is");
#line 12 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_2, hero_lk_t_3), "hero_lk_is");
#line 13 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_3, hero_lk_t_0), "hero_lk_is");
#line 14 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_3, hero_lk_t_1), "hero_lk_is");
#line 15 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_3, hero_lk_t_2), "hero_lk_is");
#line 16 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_3, hero_lk_t_3), "hero_lk_is");
#line 17 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_4, hero_lk_t_0), "hero_lk_is");
#line 18 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_4, hero_lk_t_1), "hero_lk_is");
#line 19 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_4, hero_lk_t_2), "hero_lk_is");
#line 20 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_4, hero_lk_t_3), "hero_lk_is");
#line 21 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_5, hero_lk_t_0), "hero_lk_is");
#line 22 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_5, hero_lk_t_1), "hero_lk_is");
#line 23 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_5, hero_lk_t_2), "hero_lk_is");
#line 24 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_5, hero_lk_t_3), "hero_lk_is");
#line 25 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_6, hero_lk_t_0), "hero_lk_is");
#line 26 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_6, hero_lk_t_1), "hero_lk_is");
#line 27 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_6, hero_lk_t_2), "hero_lk_is");
#line 28 "hero_lk_is"
_Static_assert(!__builtin_types_compatible_p(hero_lk_m_6, hero_lk_t_3), "hero_lk_is");
