#include "communication.h"
#include <iostream>

using namespace std;
int main()
{
  int x;
  Spi testspi;
  testspi.init(50);
  char arr[50] = {0};
  if (testspi.read(arr, 50) != 0)
  {
  }
  for (int i = 0; i < 50; i++)
  {
    cout << arr[i];
  }

  return 0;
}