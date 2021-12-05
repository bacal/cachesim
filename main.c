#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

void print_usage(){
  fprintf(stderr,"cachesim: usage:\n");
}

int main(int argc, char** argv){
  int option;
  if(argc == 1){
    print_usage();
    return 1;
  }
  while((option = getopt(argc,argv,":bl:r")) != -1){
    switch(option){
    case 'b':
      break;
    case 'l':
      printf("optarg: %s\n", optarg);
      break;
    case ':':
      printf("missing arg for: %c\n",optopt);
      break;
    case '?':
      printf("unknown option: %c\n",optopt);
      break;
    }
  }
    return 0;
}
