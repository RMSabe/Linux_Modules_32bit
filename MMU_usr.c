//Memory Mapping Utility

#include "MMU_usr.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#define MMU_PROC_FILE_DIR "/proc/MMU"
#define MMU_WAIT_TIME_US 1

#define MMU_DATAIO_SIZE_BYTES 8

#define MMU_CMD_GET_PHYSICAL_ADDR 1
#define MMU_CMD_GET_VIRTUAL_ADDR 2

int mmu_proc_fd = -1;
void *mmu_data_io = NULL;

void mmu_wait(void)
{
	clock_t start_time = clock();
	while(clock() < (start_time + MMU_WAIT_TIME_US));
	return;
}

bool mmu_is_active(void)
{
	return (mmu_proc_fd >= 0);
}

bool mmu_init(void)
{
	if(mmu_is_active()) return true;

	mmu_proc_fd = open(MMU_PROC_FILE_DIR, O_RDWR);
	if(mmu_proc_fd < 0) return false;

	mmu_data_io = malloc(MMU_DATAIO_SIZE_BYTES);
	return true;
}

uint32_t mmu_get_phys_from_virt(void *virtaddr)
{
	uint8_t *pbyte = (uint8_t*) mmu_data_io;
	uint32_t *puint = (uint32_t*) &pbyte[4];
	pbyte[0] = MMU_CMD_GET_PHYSICAL_ADDR;
	puint[0] = (uint32_t) virtaddr;

	write(mmu_proc_fd, mmu_data_io, MMU_DATAIO_SIZE_BYTES);
	mmu_wait();
	read(mmu_proc_fd, mmu_data_io, MMU_DATAIO_SIZE_BYTES);

	return puint[0];
}

void *mmu_get_virt_from_phys(uint32_t physaddr)
{
	uint8_t *pbyte = (uint8_t*) mmu_data_io;
	uint32_t *puint = (uint32_t*) &pbyte[4];
	pbyte[0] = MMU_CMD_GET_VIRTUAL_ADDR;
	puint[0] = physaddr;

	write(mmu_proc_fd, mmu_data_io, MMU_DATAIO_SIZE_BYTES);
	mmu_wait();
	read(mmu_proc_fd, mmu_data_io, MMU_DATAIO_SIZE_BYTES);

	return (void*) puint[0];
}
