#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

static const char *nowtime()
{
  time_t ntime = time(NULL);
  struct tm *now = localtime(&ntime);
  return asctime(now);
}

static int test_file()
{
  char file_name[128] = {0};
  char *data_path = getenv("MY_DATA_DIR");
  snprintf(file_name, sizeof(file_name) - 1, "%s/test_file.txt",
      data_path ? data_path : ".");
  printf("test file: %s\n", file_name);

  FILE *fp = fopen(file_name, "a++");
  if (NULL == fp) {
    printf("open file failed: %s\n", strerror(errno));
    return -1;
  }

  char buff[128] = {0};
  snprintf(buff, sizeof(buff) - 1, "Hello world, now is %s\n", nowtime());
  int ret = fwrite(buff, strlen(buff), 1, fp);
  if (ret < 0) {
    printf("write file failed: %s\n", strerror(errno));
  }

  fclose(fp);

  return 0;
}


int main()
{
  printf("Hello, here is docker demo!\n");
  printf("I am printf log to STDOUT.\n");
  syslog(LOG_INFO, "I am syslog.\n");

  test_file();

  return 0;
}

