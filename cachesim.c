#include <argp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const char *argp_program_version = "cachesim 1.0.0";
const char *argp_program_bug_address = "mesfinb2@vcu.edu";
static char doc[] = "Cache Simulator for EGRE-426";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = { 
  { "bs"  ,  'b', "[BLOCK_SIZE]", 0, "Number of bytes in a block",0},
  { "nb"  ,  'n', "[NUM_OF_BLOCKS]", 0, "Number of blocks in the cache",0},
  { "associativity"  ,  'a', NULL, OPTION_ARG_OPTIONAL, "Associativity",0},
  { "LRU"  ,  'l', NULL, OPTION_ARG_OPTIONAL, "LRU policy",0},
  { "output"  ,  'o', "[FILE]", 0, "Output to a file instead of standard out",0},
  {0}
};

struct raw_data{
  int* addresses;
  int count;
};

struct arguments {
  char* input_file;
  char* output_file;
  int block_size;
  int block_amount;
  bool associativity;
  bool lru_policy;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
    case 'o':
      arguments->output_file = arg;
      break;
    case 'b':
      arguments->block_size = atoi(arg);
      break;
    case 'n':
      arguments->block_amount = atoi(arg);
      break;
    case 'a':
      arguments->associativity = true;
      break;
    case 'l':
      arguments->lru_policy = true;
      break;

    case ARGP_KEY_ARG:
      arguments->input_file = arg;
      break;


    case ARGP_KEY_NO_ARGS:
      argp_usage(state);
      break;

    case ARGP_KEY_END:
      break;

    default: return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int power_of_two(int num){
  if(num==0)
    return 0;
  else
    return(ceil(log2(num)) == floor(log2(num)));
}


struct raw_data* read_data(const char* file_name){
  FILE* fp;
  int* addresses = NULL;
  struct raw_data* data = malloc(sizeof(struct raw_data));
  if(!file_name){
    fprintf(stderr, "cachesim: fatal error: must include a file name\n");
    exit(1);
  }
  else if(file_name){
    fp = fopen(file_name, "r");
    if(!fp){
      fprintf(stderr, "cachesim: fatal error: failed to open file\n");
      exit(1);
    }

    int count=0;
    int a;
    while(fscanf(fp,"0x%x\n",&a) != EOF){
      count++;
    }
    fseek(fp, 0, SEEK_SET);
    addresses = malloc(count*sizeof(int));
    int pos=0;
    while(fscanf(fp, "0x%x\n",&addresses[pos])!=EOF){
      pos++;
    }
    data->addresses = addresses;
    data->count = count;
    return data;
  }
  else{
    fprintf(stderr, "cachesim: error: unknown error");
    exit(1);
  }
}

int main(int argc, char **argv)
{

  struct raw_data* addresses;
  struct arguments arguments;
  arguments.block_size = 0;
  arguments.block_amount = 0;
  arguments.input_file = NULL;
  arguments.output_file = NULL;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  FILE* fp = stdout;
  addresses = read_data(arguments.input_file);
  if(arguments.output_file){
    fp = fopen(arguments.output_file, "w");
  }
  if(arguments.block_size)
    printf("Block Size: %d\n", arguments.block_size);
  if(arguments.block_amount <= 0){
    fprintf(stderr,"Error: block size must be > 0\n");
    return 1;
  }
  else if(!power_of_two(arguments.block_size)){
    fprintf(stderr, "%s: error: Block size must be a power of two\n",argv[0]);
    return 1;
  }
  else
    printf("Cache size: %d\n",arguments.block_amount);
  printf("Reads: %d\n",addresses->count);

  fclose(fp);

  return 0;
}
