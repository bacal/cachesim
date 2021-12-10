#ifndef _CACHESIM_CACHE_H_
#define _CACHESIM_CACHE_H_


struct raw_data{
  unsigned int* addresses;
  unsigned int count;
};

typedef struct CACHE_STRUCT{
  char* valid;
  unsigned int* data;
}cache;

typedef struct BIT_SIZES{
  int tag_bits;
  int offset_bits;
  int index_bits;
}bit_sizes;

typedef struct CACHE_COMPONENTS_STRUCT{
  int tag;
  int index;
  int offset;
}cache_components;

cache* create_cache(int);

int add_to_cache(cache*,int,bit_sizes*);

char* int_to_binary_string(int,int);

bit_sizes* get_bit_sizes(int,int);

int power_of_two(int);

cache_components* get_cache_components(unsigned int addr, bit_sizes* bits);

struct raw_data* read_data(const char* file_name);

#endif /*_CACHESIM_CACHE_H_*/
