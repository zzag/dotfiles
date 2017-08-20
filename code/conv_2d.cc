#include <iostream>


using std::cout;
using std::endl;


int main()
{
  int pad = 0;
  int x[][3] = {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 9 }
  };
  int k[][3] = {
    { -1, -2, -1 },
    {  0,  0,  0 },
    {  1,  2,  1 }
  };
  int y[3][3];
  int cx = 3 / 2;
  int cy = 3 / 2;
  
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      y[i][j] = 0;

      for (int ki = 0; ki < 3; ki++)
      {
        for (int kj = 0; kj < 3; kj++)
        {
          int ii = i - (ki - cx);
          int jj = j - (kj - cy);
          int val = pad;
          if (0 <= ii && ii < 3 && 0 <= jj && jj < 3) { val = x[ii][jj]; }
          y[i][j] += val * k[ki][kj];
        }
      }
    }
  }

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++) { cout << y[i][j] << "\t"; }
    cout << endl;
  }

  return 0;
}
