#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cache.h"

cache* create_cache(int cache_size){
  
  cache* cache = malloc(sizeof(struct CACHE_STRUCT));
  cache->valid = malloc(sizeof(char)*cache_size);
  cache->data =  malloc(sizeof(int)*cache_size);
  for(int i=0; i<cache_size; i++)
    cache->valid[i] = 0;
  return cache;
}

bit_sizes* get_bit_sizes(int block_amount,int block_size){
  
  bit_sizes* bits = malloc(sizeof(struct BIT_SIZES));
  bits->index_bits = (int) log2(block_amount);
  bits->offset_bits = (int) log2(block_size);
  bits->tag_bits = 32-bits->index_bits-bits->offset_bits;
  return bits;

}

int add_to_cache(cache* c, int address,bit_sizes* s){
  
  cache_components* cc = get_cache_components(address,s);
  //printf("Address: 0x%x\n",address);

  //printf("index_size: %d, tag_size: %d, offset_size: %d\n",s->index_bits,s->tag_bits,s->offset_bits);
  //printf("cc->index = %d\n",cc->index);
  printf("cc->index = %d\n",cc->index);
  if(!c->valid[cc->index]){
    printf("Invalid tag!\n");
    c->data[cc->index] = cc->tag;
    c->valid[cc->index] = 1;
    return 0;
  }
  else if(c->valid[cc->index]){
    if(c->data[cc->index] == cc->tag){
      printf("Valid tag!\n");
      return 1;
    }
    else{
      c->data[cc->index] = cc->tag;
      return 0;
    }
  }
  return 0;
}
  


char* int_to_binary_string(int a,int len){
  int count =0;
  char* data = malloc(sizeof(char)*len*8);
  data[0] = '\0';
  for(int i=0; i<len; i++){
    strcat(data, ((1<<(len-1-i)) &a)?"1":"0");
    count++;
     if(count == 4){
      count =0;
      strcat(data, " ");
    }
  }
    return data;
}
int power_of_two(int num){
  if(num==0)
    return 0;
  else
    return(ceil(log2(num)) == floor(log2(num)));
}

cache_components* get_cache_components(unsigned int addr, bit_sizes* bits){
  cache_components* c = malloc(sizeof(struct CACHE_COMPONENTS_STRUCT));
  c->tag =0;
  c->offset =0;
  c->index =0;
  
  c->tag = (addr>>(32-bits->tag_bits));

  unsigned int index_mask = 0;
  for(int i=0; i<bits->index_bits; i++){
    index_mask = index_mask | (1<<i);
  }
  index_mask = index_mask << bits->offset_bits;
  c->index = addr & index_mask;
  c->index = c->index >> bits->offset_bits;

  unsigned int offset_mask = 0;
  for(int i=0; i<bits->offset_bits; i++){
    offset_mask = offset_mask |(1<<i);
  }
  c->offset = addr&offset_mask;
  return c;
}

struct raw_data* read_data(const char* file_name){
  FILE* fp;
  unsigned int* addresses = NULL;
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
    unsigned int a;
    while(fscanf(fp,"0x%x\n",&a) != EOF){
      count++;
    }
    fseek(fp, 0, SEEK_SET);
    addresses = malloc(count*sizeof(unsigned int));
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
