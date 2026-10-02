#include <sys/wait.h>
static inline int hero_wexitstatus(int status) { return WEXITSTATUS(status); }
