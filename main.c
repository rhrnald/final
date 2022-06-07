#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SIZE 128

int main(int argc, char** argv)
{
  int i;

  int foo = open("/dev/mem", O_RDWR);
  float *fpga_bram = mmap(NULL, SIZE * sizeof(float), PROT_READ|PROT_WRITE, MAP_SHARED, foo, 0x40000000);
  float *fpga_ip   = mmap(NULL, sizeof(float), PROT_READ|PROT_WRITE, MAP_SHARED, foo, 0x43C00000);

  // initialize memory
  for (i = 0; i < 128; i++)
    *(fpga_bram + i) = (i * 2); 

  printf("%-10s%-10s\n", "addr", "FPGA(hex)");
  for (i = 0; i < SIZE * 2; i++)
    printf("%-10d%f\n", i, *(fpga_bram + i));

  // run ip
  *(fpga_ip) = 0x5555;
  while (*fpga_ip == 0x5555);

  printf("%-10s%-10s\n", "addr", "FPGA(hex)");
  for (i = 0; i < SIZE * 2; i++)
    printf("%-10d%f\n", i, *(fpga_bram + i));

  return 0;
}

