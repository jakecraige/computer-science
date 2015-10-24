#include <stdio.h>
#include <cs50.h>

int GetUserInput();
void PrintLine(int spaces, int markers);

int main()
{
  int n = GetUserInput();
  for(int i = 1; i <= n; i++) {
    PrintLine(n - i, i);
  }
}

void PrintLine(int spaces, int markers)
{
  int i;
  char c;
  for(i = 0; i < spaces + markers; i++)
  {
    if (i < spaces)
    {
      c = ' ';
    }
    else
    {
      c = 'x';
    }
    printf("%c", c);
  }
  printf("x\n");
}

int GetUserInput()
{
  int n;
  printf("Height: ");
  n = GetInt();
  while(n <= 0 || n > 23)
  {
    printf("Retry: ");
    n = GetInt();
  }
  return n;
}
