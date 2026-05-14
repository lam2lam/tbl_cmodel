#include <assert.h>
#include <stdlib.h>

long long sqr(long long Xl, int sqr_prec) {
  long long tmp;
  long long pp_tmp;
  int i;
  tmp = Xl;
  tmp = ((tmp | (tmp << 16)) & 0x0000ffff0000ffffull);
  tmp = ((tmp | (tmp << 8))  & 0x00ff00ff00ff00ffull);
  tmp = ((tmp | (tmp << 4))  & 0x0f0f0f0f0f0f0f0full);
  tmp = ((tmp | (tmp << 2))  & 0x3333333333333333ull);
  tmp = ((tmp | (tmp << 1))  & 0x5555555555555555ull);
//  bit interleave Xl to form the Xi*Xj (i == j) case
  tmp >>= (34 - sqr_prec);
  for(i = 0; i < 16; i++) {
    pp_tmp = ((Xl >> i) & 1) ? Xl : 0;
    pp_tmp &= (~((1<<(i+1))-1)); // clears from i bit to 0
    pp_tmp <<= (i+1); // since the symetry, everything x 2
    pp_tmp >>= (34 - sqr_prec);
    tmp += pp_tmp;
  }
  if(sqr_prec == 34)
    assert(Xl*Xl == tmp);
  return tmp;
}
