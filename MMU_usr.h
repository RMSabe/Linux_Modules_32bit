//Memory Mapping Utility

#ifndef MMU_USR_H
#define MMU_USR_H

#include <stdbool.h>
#include <stdint.h>

bool mmu_is_active(void);
bool mmu_init(void);
uint32_t mmu_get_phys_from_virt(void *virtaddr);
void *mmu_get_virt_from_phys(uint32_t physaddr);

#endif
