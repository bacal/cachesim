#include <argp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PROGRAM_NAME "cachesim"
#include "cache.h"

#define _64KiB 65536
#define _640KiB 655360

const char *argp_program_version = "cachesim 1.0.0";
const char *argp_program_bug_address = "mesfinb2@vcu.edu";
static char doc[] = "Cache Simulator for EGRE-426";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = {
  { "block-size"  ,  'b', "[BLOCK_SIZE]", 0, "Number of bytes in a block",0},
  { "num-blocks"  ,  'n', "[NUM_OF_BLOCKS]", 0, "Number of blocks in the cache",0},
  { "associativity"  ,  'a', "[N-WAY]", 0, "Associativity",0},
  { "output"  ,  'o', "[FILE]", 0, "Output to a file instead of standard out",0},
  { "cache-size", 'c', "[CACHE_SIZE]", 0, "Set cache size",0},
  {0}
};

struct arguments {
  char* input_file;
  char* output_file;
  char** other_input_files;
  unsigned int cache_size;
  unsigned int block_size;
  unsigned int block_amount;
  unsigned int  associativity;
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
      if(atoi(arg) < 0){
	fprintf(stderr,"%s: error: block size must be > 0\n",PROGRAM_NAME);
	exit(1);
      }
      break;
    case 'n':
      arguments->block_amount = atoi(arg);
      break;
    case 'a':
      if(!strcmp(arg,"fully")|| !strcmp(arg,"f")){
	arguments->associativity = 0;
      }
      else if(!strcmp(arg,"direct") || !strcmp(arg,"d")){
	arguments->associativity = 1;
      }
      else{
	arguments->associativity = atoi(arg);
	if(atoi(arg)< 0){
	  fprintf(stderr,"cahesim: error: associativity must be >0");
	  exit(1);
	}
      }
      break;
      
    case 'c':
      arguments->cache_size = atoi(arg);
      break;

    case ARGP_KEY_ARG:
      if(!arguments->input_file)
	arguments->input_file = arg;
      arguments->other_input_files = &state->argv[state->next];
      state->next = state->argc;
      break;
      
    case ARGP_KEY_NO_ARGS:
      argp_usage(state);
      break;

    case ARGP_KEY_END:
      if(arguments->input_file)
	break;
      break;

    default: return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };


int main(int argc, char **argv)
{
  struct raw_data* addresses;
  struct arguments arguments;
  arguments.block_size = 16;
  arguments.block_amount = 256;
  arguments.input_file = NULL;
  arguments.output_file = NULL;
  arguments.cache_size = _640KiB;
  arguments.associativity = 1;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  //for(int i=0; arguments.other_input_files[i] != NULL; i++)
  //fprintf(stdout, "%s ", arguments.other_input_files[i]);

  FILE* fp = stdout;
  addresses = read_data(arguments.input_file);
  if(arguments.output_file){
    fp = fopen(arguments.output_file, "w");
  }

  if(!power_of_two(arguments.block_size)){
    fprintf(stderr, "%s: error: Block size must be a power of two\n",argv[0]);
    return 1;
  }

  cache* cache = create_cache(arguments.cache_size,arguments.associativity);
  
  fprintf(fp,"Cache size: %dk\n",arguments.cache_size/1024);

  fprintf(fp,"Reads: %d\n",addresses->count);

  bit_sizes* bits = get_bit_sizes(arguments.block_amount,arguments.block_size);

  int hit_rate =0;
  for(int i=0; i<(int)addresses->count; i++){
    if(add_to_cache(cache,addresses->addresses[i],bits)){
      hit_rate++;
    }
  }
  fprintf(fp,"Hits: %d\n",hit_rate);
  fprintf(fp,"Misses: %d\n",addresses->count - hit_rate);
  fprintf(fp,"Hit Rate: %.2f%%\n",((float)hit_rate/addresses->count)*100);
  fprintf(fp,"Miss Rate: %.2f%%\n",(1-(float)hit_rate/addresses->count)*100);
  //  fp==stdout?fclose(fp):0;
  
  return 0;
}
