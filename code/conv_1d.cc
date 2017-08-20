#include <iostream>

#define SIZEOF(x) (sizeof(x) / sizeof(*x))


using std::cout;
using std::endl;


int main()
{
  int x[] = { 3, 4, 5 };
  int k[] = { 2, 1 };
  int y[] = { 0, 0, 0};
  int pad = 0;

  for (int i = 0; i < SIZEOF(x); i++)
  {
    for (int j = 0; j < SIZEOF(k); j++)
    {
      int val = pad;
      if (i - j >= 0) { val = x[i - j]; }
      y[i] += val * k[j];
    }
  }

  for (int i = 0; i < SIZEOF(y); i++) { cout << y[i] << endl; }

  return 0;
}
