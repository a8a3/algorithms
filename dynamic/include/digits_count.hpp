#pragma once

// ------------------------------------------------------------------
long get_digits_count(int base) {
   long a, a1, c, c1;
   a = 1;
   c = 0;

   while(--base) {
      a1 = a + c;
      c1 = a;
      a = a1;
      c = c1;
   }
   return a + a + c + c;
}