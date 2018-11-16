#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>


long random() {
  int a = open("/dev/random", O_RDONLY);

  if (a == -1)
       printf("%s\n", strerror(a));

  int c;
  int b = read(a, &c, sizeof(c));

  if (c < 0)
    c = -1 * c;
  c = ( c % 15 ) + 5;

  close(a);
  return c;
}


int main() {
  int status, rando;
  printf("This is the initial message before forking.\n");

  int x = getpid();
  printf("Parent ppid: %d\n", x);
  printf("-----------------------------\n");


  if (fork())
    fork();


  if (getpid() != x) {
    printf("Child pid: %d\n", getpid());

    rando = random();
    printf("Sleep time: %d s.\n", rando);
    sleep(rando);

    int file = open("errors.txt",  O_CREAT | O_RDWR, 0777);
    if (file == -1)
         printf("%s\n", strerror(file));

    int a = write(file, &rando, sizeof(rando));
    if (a == -1)
         printf("%s\n", strerror(file));

    printf("I finished sleeping!\n");
    close(file);

  }


  int y = waitpid(-1, &status, 0);
  printf("-----------------------------\n");
  if(WIFEXITED(status)) {
    printf("Process completed: %d\n", y) ;
    int result;
    int file = open("errors.txt", O_RDWR);
    if (file == -1)
         printf("%s\n", strerror(file));

    int d = read(file, &result, sizeof(int));
    printf("I slept for %d s.\n", result);

    printf("The parent process is all done now. Bye!\n");
    kill(x, SIGTERM);
  }

  return 0;
}
