#ifndef __DE_ARRAY_H__
#define __DE_ARRAY_H__
#include <stdlib.h>
#include <stdbool.h>
typedef unsigned int uint;

#define DARR_VALID_KEY 0xAbEdFeCa

typedef struct darr{
	uint valid; // this shall remain as first member and == VALID_KEY
	union{void* data, *v;};
	uint len;
	uint cap;
	size_t type;
} darr;

#ifdef DE_DEBUG
#	define DARR_VALIDATE(arr) assert(darr_valid(arr));
#else
#	define DARR_VALIDATE(arr)
#endif

#define darr_newb(type) darr_new(sizeof(type), 0, NULL)
#define darr_newt(type, init_cap, init_cont) darr_new(sizeof(type), (init_cap), (init_cont))

darr* darr_new(size_t type_size, uint initial_capacity, void* initial_content);
void  darr_free(darr* a);

bool    darr_valid(darr* arr);// (((darr)darr)->valid == DARR_VALID_KEY)

uint   darr_len(darr* a);
size_t darr_type_size(darr* a);
uint   darr_set_len(darr* a, uint new_len); //return old len

void  darr_set(darr* a, uint slot, void* val); // will alloc memory to fit
void  darr_set_part(darr* a, uint start, uint end, void* values);

void* darr_get(darr* a, uint slot);
#define darr_read(type,a,slot) ( *(type*)darr_get(a,slot) )

void darr_zero(darr* a, uint slot);
void darr_swap(darr* a, uint slota, uint slotb);

void darr_add(darr* a, void* value); /* aka push */
void darr_cat(darr* dst, darr* src);
void darr_cat_part(darr* dst, darr* src, uint src_start, uint src_end);
// void darr_add_zero(darr* a);
void darr_pop(darr* a); /* delete last element */


darr* darr_dup(darr* src);
darr* darr_dup_part(darr* src, uint start, uint end);

// void darrCpy
// void darrCpyPart

void darr_pad(darr* a, uint slot, int shift);
void darr_pad_part(darr* a, uint start, uint end, int shift);

void darr_ins(darr* a, uint slot, void* val);
void darr_insa(darr* dst, uint slot, darr* src);
void darr_ins_part(darr* dst, uint start, uint end, void** values);
void darr_insa_part(darr* dst, uint dst_slot, darr* src, uint src_start, uint src_end);
void darr_rem(darr* a, uint slot);
void darr_rem_part(darr* a, uint start, uint end);
#define darr_splice(a, i, len) darr_rem_part( (a), (i), (i)+(len) )

// uint darrIndexOf(darr* arr, void* address);// ?
/* i == index_of(a, get(a, i)) */

// extern uint Darr_grow_factor;
void darr_grow(darr* a, int factor);


// TODO
// void darrAdda(darr* dst, darr* src){}
// void darrAddv(darr* dst, void* val, uint num_val){}
/* arr iterators */




void darr_test(void);
#endif
