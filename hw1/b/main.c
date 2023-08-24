#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ALIGN(x, a) __ALIGN_KERNEL((x), (a))
#define __ALIGN_KERNEL(x, a) __ALIGN_KERNEL_MASK(x, (__typeof__(x)) (a) -1)
#define __ALIGN_KERNEL_MASK(x, mask) (((x) + (mask)) & ~(mask))

static inline uintptr_t align_up(uintptr_t sz, size_t alignment)
{
    uintptr_t mask = alignment - 1;
    if ((alignment & mask) == 0) {   /* power of two? */
        return (sz + mask) & ~mask;  // MMMM
    }
    return (((sz + mask) / alignment) * alignment);
}


int main()
{
    uintptr_t sz = 122;
    size_t alignment = 4;
    uintptr_t res = align_up(sz, alignment);
    uintptr_t macroRes = ALIGN(sz, alignment);
    printf("res = %lu\n", res);
    printf("macroRes = %lu\n", macroRes);
    return 0;
}