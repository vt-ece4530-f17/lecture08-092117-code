#include "omsp_de1soc.h"

#define HW_A      (*(volatile unsigned *)      0x140)
#define HW_B      (*(volatile unsigned *)      0x142)
#define HW_RETVAL (*(volatile unsigned long *) 0x144)
#define HW_CTL    (*(volatile unsigned *)      0x148)

unsigned long mymul(unsigned a, unsigned b) {
   unsigned long r;
   r = (unsigned long) a * b;
   return r;
}

unsigned long mymul_hw(unsigned a, unsigned b) {
  HW_A = a;
  HW_B = b;
  HW_CTL = 1;
  HW_CTL = 0;
  return HW_RETVAL;
}

int main(void) {
  unsigned i, j;
  unsigned long c;
  
  de1soc_init();
  
  while (1) {
    for (i=1; i<255; i++) 
      for (j=1; j<255; j++) {
	c = mymul(i,j);
	de1soc_hexlo(c);
	c = mymul_hw(i,j);
	de1soc_hexhi(c);
	long_delay(100);
      }
  }
  LPM0;
  return 0;
}
