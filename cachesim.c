#include <argp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

const char *argp_program_version = "cachesim 1.0.0";
const char *argp_program_bug_address = "mesfinb2@vcu.edu";
static char doc[] = "Cache Simulator for EGRE-426";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = { 
	{ 0 ,  'b', "[BLOCK_SIZE]", 0, "Number of bytes in a block"},
	{ 0 ,  'n', "[NUM_OF_BLOCKS]", 0, "Number of blocks in the cache"},
	{ 0 ,  'a', 0, 0, "Associativity"},
	{ 0 , 'l', 0, 0, "LRU policy"},
	{ 0 , 'o', "[FILE]", 0, "Output to a file instead of standard out"},
	{ 0 } 
};

struct arguments {
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
    
    /*
      case 'l': arguments->mode = LINE_MODE; break;
      case 'w': arguments->mode = WORD_MODE; break;
      case 'i': arguments->isCaseInsensitive = true; break;
      case ARGP_KEY_ARG: return 0;
    */
  default: return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char **argv)
{
	struct arguments arguments;
	arguments.block_size = 0;
	arguments.block_amount = 0;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	if(!arguments.output_file){
	  FILE* fp = fopen(arguments.output_file, "w");
	  fprintf(fp, "Hello World!\n");
	  fclose(fp);
	}
	if(arguments.block_size)
	  printf("Block Size: %d\n", arguments.block_size);
	if(arguments.block_amount)
	  printf("Amount of Blocks: %d\n",arguments.block_amount);
	return 0;
}
