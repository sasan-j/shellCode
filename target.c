#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

void do_map_libc(void) {
int fd;
struct stat sb;
fd = open("libc-2.3.5.so", O_RDONLY, 0);
fstat(fd, &sb);
mmap((void *)0x03000000, sb.st_size,PROT_READ|PROT_EXEC, MAP_FIXED|MAP_SHARED, fd, 0);
}

void do_map_stack(void) {
int fd;
fd = open("/dev/zero", O_RDONLY, 0);
mmap(0x04f000000, 0x001000000, PROT_READ|PROT_WRITE,MAP_FIXED|MAP_PRIVATE, fd, 0);
}

void overflow(char *arg) {
char buf[64];
strcpy(buf, arg);
}

void move_stack(char *arg) {
__asm("mov $0x04fffff00, %esp\n");
overflow(arg);
_exit(0);
}

int main(int argc, char *argv[]) {
do_map_libc(); do_map_stack();
move_stack(argv[1]);
}
