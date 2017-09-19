#include "omsp_de1soc.h"

// The reference implementation
unsigned transpose(unsigned a) {
  unsigned r1, r2, r3, r4;
  unsigned c, d, i;

  r1 = (a >> 12);
  r2 = (a >>  8) & 0xf;
  r3 = (a >>  4) & 0xf;
  r4 = (a      ) & 0xf;

  c = 0;
  for (i = 0; i<4; i++) {
    d = ((r1 & 1) << 3)
      | ((r2 & 1) << 2)
      | ((r3 & 1) << 1)
      | ((r4 & 1));
      
    c = d * 4096 + (c >> 4);

    r1 = r1 >> 1;
    r2 = r2 >> 1;
    r3 = r3 >> 1;
    r4 = r4 >> 1;
  }
  
  return c;
}

// The hardware-accelerated implementation
unsigned transpose_hw(unsigned a) {
  volatile unsigned k = transpose(a);
  return k;
}

//--------------------- NO CHANGES ALLOWED BELOW THIS LINE --------------------//

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
  unsigned i, t, c, k;
  unsigned long sw_tt, sw_ct;
  unsigned long hw_tt, hw_ct;
  
  TACTL  |= (TASSEL1 | MC1 | TACLR);
  de1soc_init();
  
  while (1) {
    sw_tt = 0;
    sw_ct = 0;
    hw_tt = 0;
    hw_ct = 0;
    for (i=0; i<32768; i++) {
      TimerLap();
      t = transpose(i);
      c = TimerLap();
      sw_tt += t;
      sw_ct += c;
      
      TimerLap();
      t = transpose_hw(i);
      c = TimerLap();
      hw_tt += t;
      hw_ct += c;      
    }
    
    putchar('S');
    putchar('W');
    putchar(' ');
    
    printfhex(sw_tt >> 16);
    printfhex(sw_tt);
    
    putchar(' ');
    
    printfhex(sw_ct >> 16);
    printfhex(sw_ct);
    
    putchar(' ');
    putchar('H');
    putchar('W');
    putchar(' ');
    
    printfhex(hw_tt >> 16);
    printfhex(hw_tt);
    
    putchar(' ');
    
    printfhex(hw_ct >> 16);
    printfhex(hw_ct);
    
    putchar('\n');    
  }
  
  LPM0;
  
  return 0;
}
 
