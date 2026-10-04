#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>


# define BUF_SIZE 4096


int main(int argc, char* argv[]){




    if(argc!=2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);  
        exit(EXIT_FAILURE);
    }

    int inputFd = open(argv[1] , O_RDONLY);

    if(inputFd == -1){
        perror("Open");
        exit(EXIT_FAILURE);
    }

    unsigned char buf[BUF_SIZE];
    ssize_t numRead;

    unsigned long freq[256] = {0};

    while((numRead = read(inputFd, buf, BUF_SIZE)) > 0){


        for(ssize_t i = 0; i < numRead ; i++){
            freq[buf[i]]++;
        }

    }

    if (numRead == -1) {
        perror("read");
        close(inputFd);
        exit(EXIT_FAILURE);
    }

    for(int i = 0 ; i< 256 ; i++){
        if(freq[i] > 0){
            printf("%c :  %lu\n", (char)i , freq[i]);
        }
    }

    if (close(inputFd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;

}
