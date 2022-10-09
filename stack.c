#include <stdio.h>
 
int main() {

  int exploit = 0x1234;
  char arr[10];
 
  fgets(arr,13,stdin);
 
  if (exploit == 0x9876) {
     printf("It worked!\n");
  } else {
     printf("It failed.\n");
  }
  
  return 0;
}
