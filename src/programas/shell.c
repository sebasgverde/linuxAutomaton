m#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main() {

  int tuberia[2];

  pipe(tuberia);

  if (fork() == 0) { // Hijo
    close(1);
    dup(tuberia[1]);
    close(tuberia[0]);
    execl("/usr/bin/ls", "ls", "-l", NULL);
  }
  else { // Proceso padre
    if (fork() == 0) { // Hijo
      close(0);
      dup(tuberia[0]);
      close(tuberia[1]);
      execl("/usr/bin/wc", "wc", NULL);
    }
    else {
      close(tuberia[0]);
      close(tuberia[1]);
      int status;
      wait(&status);
      wait(&status);
    }
  }
  return 0;
}
