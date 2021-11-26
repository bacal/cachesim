#include <argp.h>
#include <stdio.h>
#include <stdbool.h>

const char *argp_program_version = "cachesim 1.0.0";
const char *argp_program_bug_address = "mesfinb2@vcu.edu";
static char doc[] = "Cache Simulator for EGRE-426";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = { 
	{ "bs",  'b', 0, 0, "Number of bytes in a block"},
	{ "bn",  'n', 0, 0, "Number of blocks in the cache"},
	{ "as",  'a', 0, 0, "Associativity"},
	{ "lru", 'l', 0, 0, "LRU policy"},
	{ 0 } 
};

struct arguments {
	enum { CHARACTER_MODE, WORD_MODE, LINE_MODE } mode;
	bool isCaseInsensitive;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;
	switch (key) {
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

	arguments.mode = CHARACTER_MODE;
	arguments.isCaseInsensitive = false;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

}
