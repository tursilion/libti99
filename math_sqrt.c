// adapted from https://www.quora.com/How-do-I-write-a-program-for-finding-the-square-root-of-a-number-without-using-the-sqrt-function
// adapted to remove floats, int only, rounding errors galore!
int sqrt(int x) {
  int prev,k = 0;
  int kmax = 1000;
  int s = 1;

  if (x == 1) return 1;
  if (x < 2) return 0;
  
  for(k=0;k<kmax;k++)
  {
    prev = s;
    s = (s + x/s)/2;
    if(prev == s)
    {
      break;
    }
  }
  return s;
}
