#include <linux/unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <elf.h>
#include <math.h>


#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)


/* File types for d_type */
#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12

#define BUF_SIZE 1024
char buf[BUF_SIZE];

struct linux_dirent64
{
    long d_ino;              /* 64-bit inode number */
    long d_off;              /* 64-bit offset to next structure */
    unsigned short d_reclen; /* Size of this dirent */
    unsigned char d_type;    /* File type */
    char d_name[];           /* Filename (null-terminated) */
};

static inline long long syscall3(long long syscallnum, long long arg0, long long arg1, long long arg2)
{
    register long long syscallnum_ __asm__("rax");
    register long long arg0_ __asm__("rdi");
    register long long arg1_ __asm__("rsi");
    register long long arg2_ __asm__("rdx");
    syscallnum_ = syscallnum;
    arg0_ = arg0;
    arg1_ = arg1;
    arg2_ = arg2;
    asm volatile(
        "syscall"
        : "+r"(syscallnum_)
        : "r"(arg0_), "r"(arg1_), "r"(arg2_));
    return syscallnum_;
}

int getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count)
{
    return (int)syscall3(__NR_getdents64, fd, buf, count);
}

void readelfoffile(char* filename) {
    FILE *file = fopen(filename, "rw");

	if(file==NULL){
		perror("error opening file");
		return 1;
	}

	// Lire des données à partir de la position actuelle (sizeof(Elf64_Ehdr) octets à partir du début)
    uint8_t buffer[8192];
    size_t bytes_read = fread(buffer, 1, sizeof(Elf64_Ehdr), file);

    if (bytes_read == 0) {
        perror("Error reading from file");
        fclose(file);
        return 1;
    }

	unsigned int offset_counter = EI_NIDENT;

    //check if elf
    if(buffer[0] == 0x7f && buffer[1] == 0x45 && buffer[2] == 0x4c && buffer[3] == 0x46){
        printf("true       ");
    } else {
        printf("false\n");
        return 1;
    }

    if(buffer[4] == ELFCLASS32) {
        printf("32 bits             ");
    } else if(buffer[4] == ELFCLASS64) {
        printf("64 bits             ");
    } 

    if(buffer[6] == EV_CURRENT) {
        printf("true\n");
    } else {
        printf("false\n");
    }
}

int main(int argc, char *argv[])
{
    int fd, nread;
    
    struct linux_dirent64 *d;
    int bpos;
    char d_type;

   fd = open(argc > 1 ? argv[1] : ".", O_RDONLY | 00200000);
    if (fd == -1)
        handle_error("open");
    
    for ( ; ; ) {
        nread = getdents64(fd, d, BUF_SIZE);
        if (nread == -1)
            handle_error("getdents");

        if (nread == 0)
            break;

        printf("inode#    file type   d_name    is_elf    elf_architecture     EV_CURRENT\n");
        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);
            printf("%8ld  ", d->d_ino);
            d_type = d->d_type;
            printf("%-10s ", (d_type == DT_REG) ?  "regular" :
                             (d_type == DT_DIR) ?  "directory" :
                             (d_type == DT_FIFO) ? "FIFO" :
                             (d_type == DT_SOCK) ? "socket" :
                             (d_type == DT_LNK) ?  "symlink" :
                             (d_type == DT_BLK) ?  "block dev" :
                             (d_type == DT_CHR) ?  "char dev" : "???");
            
            printf(" %-10s", d->d_name);
            if(d_type == DT_REG) 
                readelfoffile(d->d_name);
            else {
                printf("------------Not a regular file------------\n");
            }
            
            bpos += d->d_reclen;
        }
    }
}
