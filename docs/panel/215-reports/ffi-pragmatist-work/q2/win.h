/* ffi-pragmatist, panel 215: Windows' locks, held in structs of the program's own. */
#include <windows.h>
struct csbox { CRITICAL_SECTION cs; int tag; };
struct srwbox { SRWLOCK l; int tag; };
static inline void cs_init(struct csbox *b) { InitializeCriticalSection(&b->cs); }
static inline void cs_enter(struct csbox *b) { EnterCriticalSection(&b->cs); }
static inline int cs_try(struct csbox *b) { return TryEnterCriticalSection(&b->cs) ? 1 : 0; }
static inline void cs_leave(struct csbox *b) { LeaveCriticalSection(&b->cs); }
static inline void cs_delete(struct csbox *b) { DeleteCriticalSection(&b->cs); }
static inline long cs_lockcount(const struct csbox *b) { return (long)b->cs.LockCount; }
static inline void srw_init(struct srwbox *b) { InitializeSRWLock(&b->l); }
static inline void srw_lock(struct srwbox *b) { AcquireSRWLockExclusive(&b->l); }
static inline int srw_try(struct srwbox *b) { return TryAcquireSRWLockExclusive(&b->l) ? 1 : 0; }
static inline void srw_unlock(struct srwbox *b) { ReleaseSRWLockExclusive(&b->l); }
