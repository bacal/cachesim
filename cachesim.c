#include <argp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
  { "associativity"  ,  'a', "[TYPE]", 0, "Associativity",0},
  { "LRU"  ,  'l', NULL, OPTION_ARG_OPTIONAL, "LRU policy",0},
  { "output"  ,  'o', "[FILE]", 0, "Output to a file instead of standard out",0},
  { "cache-size", 'c', "[CACHE_SIZE]", 0, "Set cache size",0},
  {0}
};

struct arguments {
  char* input_file;
  char* output_file;
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
      break;
    case 'n':
      arguments->block_amount = atoi(arg);
      break;
    case 'a':
      arguments->associativity = atoi(arg);
      break;
    case 'l':
      arguments->lru_policy = true;
      break;

    case 'c':
      arguments->cache_size = atoi(arg);
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


int main(int argc, char **argv)
{
  struct raw_data* addresses;
  struct arguments arguments;
  arguments.block_size = 256;
  arguments.block_amount = 16;
  arguments.input_file = NULL;
  arguments.output_file = NULL;
  arguments.cache_size = _640KiB;
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  FILE* fp = stdout;
  addresses = read_data(arguments.input_file);
  if(arguments.output_file){
    fp = fopen(arguments.output_file, "w");
  }

  if(!power_of_two(arguments.block_size)){
    fprintf(stderr, "%s: error: Block size must be a power of two\n",argv[0]);
    return 1;
  }

  cache* cache = create_cache(arguments.cache_size);
  
  fprintf(fp,"Cache size: %dk\n",arguments.cache_size/1024);
  fprintf(fp,"Block Size: %d\n", arguments.block_size);
  fprintf(fp,"Reads: %d\n",addresses->count);

  bit_sizes* bits = get_bit_sizes(arguments.block_amount,arguments.block_size);
  /*
  fprintf(fp, "#index bits: %d\n#tag bits: %d\n#offset bits: %d\n",bits->index_bits,bits->tag_bits,bits->offset_bits);
  

  //  index = block_address % arguments.block_amount;


  //fprintf(fp, "block_address = %d\n", block_address);
  //cache[index] = 0;
  //fprintf(fp, "address: 0x%x: %s\ntag: %dbits\ntag_value: %s\nindex: %d\n",addresses->addresses[0],int_to_binary_string(addresses->addresses[0],32),tag,int_to_binary_string(addresses->addresses[0]>>(32-tag),32),index);

  fprintf(fp, "--------------\nAddress 0x%x\n------------\n",addresses->addresses[0]);
  fprintf(fp, "Address\t%s\n",int_to_binary_string(addresses->addresses[0],32));
  //fprintf(fp, "Index Bits\t%s\n",int_to_binary_string());
  //fprintf(fp, "0x%x: %s",addresses->addresses[0],int_to_binary_string(addresses->addresses[0],32));
  fprintf(fp,"\n");

  cache_components* c = get_cache_components(addresses->addresses[0],bits);
  fprintf(fp,"tag bits: %s : %d\n",int_to_binary_string(c->tag,32),c->tag);
  fprintf(fp,"index bits: %s:  %d\n", int_to_binary_string(c->index,32),c->index);
  fprintf(fp,"offset bits: %s:  %d\n", int_to_binary_string(c->offset,32),c->offset);
  

  
  
  fprintf(fp,"Adding 0x%x to cache: %s\n",addresses->addresses[0],add_to_cache(cache,c,bits)?"hit":"miss");
  fprintf(fp,"Adding 0x%x to cache: %s\n",0xAAFFCDFF,add_to_cache(cache,0xAAFFCDFF,bits)?"hit":"miss");
  fprintf(fp,"ADding 0x%x to cache: %s\n",0xAAFFCDFF,add_to_cache(cache,0xAAFFCDFF,bits)?"hit":"miss");
  
  */

  int hit_rate =0;
  for(int i=0; i<addresses->count; i++){
    printf("Adding address 0x%x to cache\n",addresses->addresses[i]);
    if(add_to_cache(cache,addresses->addresses[i],bits)){
      hit_rate++;
    }
  }
  fprintf(fp,"hits: %d\n",hit_rate);
  
  //  fp==stdout?fclose(fp):0;
  
  return 0;
}
