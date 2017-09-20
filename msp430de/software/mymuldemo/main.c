#include "omsp_de1soc.h"

#define HW_A      (*(volatile unsigned *)      0x140)
#define HW_B      (*(volatile unsigned *)      0x142)
#define HW_RETVAL (*(volatile unsigned long *) 0x144)
#define HW_CTL    (*(volatile unsigned *)      0x148)

unsigned long  __attribute__ ((noinline)) mymul(unsigned a, unsigned b) {
   unsigned long r;
   r = (unsigned long) a * b;
   return r;
}

unsigned long  __attribute__ ((noinline)) mymul_hw(unsigned a, unsigned b) {
  HW_A = a;
  HW_B = b;
  HW_CTL = 1;
  HW_CTL = 0;
  return HW_RETVAL;
}

unsigned count = 0;

unsigned TimerLap() {
  unsigned lap;
  TACTL &= ~(MC1 | MC0);
  lap = TAR - count;
  count = TAR;
  TACTL |= MC1;
  return lap;
}

char c16[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void printfhex(int k) {
  putchar(c16[((k>>12) & 0xF)]);
  putchar(c16[((k>>8 ) & 0xF)]);
  putchar(c16[((k>>4 ) & 0xF)]);
  putchar(c16[((k    ) & 0xF)]);
  long_delay(300);
}

int main(void) {
  unsigned i, j;
  unsigned long chw, csw;
  unsigned timehw, timesw;
  
  TACTL  |= (TASSEL1 | MC1 | TACLR);
  de1soc_init();
  
  while (1) {
    for (i=500; i<755; i++) 
      for (j=500; j<755; j++) {
	
	TimerLap();
	csw = mymul(i,j);
	timesw = TimerLap();
	
	TimerLap();
	chw = mymul_hw(i,j);
	timehw = TimerLap();
	
	de1soc_hexlo(csw);
	de1soc_hexhi(chw);
	printfhex(timesw);
	putchar(' ');
	printfhex(timehw);
	putchar(' ');
	printfhex(csw >> 16L);
	printfhex(csw);
	putchar(' ');
	printfhex(chw >> 16L);
	printfhex(chw);
	putchar('\n');
      }
  }
  LPM0;
  return 0;
}
