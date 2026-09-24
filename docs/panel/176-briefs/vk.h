/* Panel 175 critic: a releaser that takes its context first, Vulkan's
 * vkDestroyBuffer(device, buffer, allocator) shape, with two releasers. */
#include <stdlib.h>
typedef struct dev dev;
typedef struct buf buf;
static __attribute__((noinline)) dev *dev_open(void) { return (dev *)malloc(8); }
static __attribute__((noinline)) void dev_close(dev *d) { free(d); }
static __attribute__((noinline)) buf *buf_make(dev *d) { (void)d; return (buf *)malloc(8); }
static __attribute__((noinline)) buf *buf_make_pooled(dev *d) { (void)d; return (buf *)malloc(8); }
static __attribute__((noinline)) void buf_destroy(dev *d, buf *b) { (void)d; free(b); }
static __attribute__((noinline)) void buf_destroy_pooled(dev *d, buf *b) { (void)d; free(b); }
