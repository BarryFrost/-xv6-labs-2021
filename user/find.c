#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *getFileName(char *s) {
    char *idx = s;
    while((strchr(idx, '/')) != 0)
    {
        idx = strchr(idx, '/');
        idx++;
    }
    return idx;
}

void 
find(char *path, char *target_file) {
    // open target
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            // is file, check if file name is target
            char *name = getFileName(path);
            // printf("get name: %s\n", name);
            if(!strcmp(name, target_file))
                fprintf(1, "%s\n", path);
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf); // end of directory name
            *p++ = '/';          // append with /
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                // skip . and ..
                if(!strcmp(de.name, ".") || !strcmp(de.name, ".."))
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0; // full path name
                find(buf, target_file);
            }
            break;
    }
    close(fd);
    return;
}

int 
main(int argc, char *argv[]) {
    if(argc < 3)
    {
        fprintf(1, "Usage: find [target] [path]\n");
        exit(0);
    } else if(argc > 3) {
        fprintf(1, "Too many argument.\n");
        exit(0);    
    } else {
        if(argv[1][0] == '.' && argv[1][1] == '\0')
            find(".", argv[2]);
        else
            find(argv[1], argv[2]);
    }
    exit(0);
}