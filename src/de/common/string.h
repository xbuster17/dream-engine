#ifndef __DE_STR_H__
#define __DE_STR_H__
#include "array.h"

typedef darr dstr;

dstr* dstr_new(char* str);
dstr* dstr_new_len(char* str, uint length);
void dstr_free(dstr* str);
bool dstr_valid(dstr* s);

char* dstr_get(dstr* s);
char dstr_read(dstr* s, uint slot);
uint  dstr_len(dstr* s);

void dstr_set(dstr* s, char* str);

void dstr_add(dstr* s, char c);
void dstr_pop(dstr* s);
void dstr_cat(dstr* dst, dstr* src);
void dstr_cat_raw(dstr* dst, char* src);

/*util*/
dstr* dstr_dup(dstr* src);
void dstr_swap(dstr* s, uint a, uint b);

/*case*/
void dstr_to_lower(dstr* s);
void dstr_to_upper(dstr* s);

void dstr_ins(dstr* s, uint slot, char c);
void dstr_inss(dstr* dst, uint dst_slot, dstr* src);
void dstr_inss_part(dstr* dst, uint dst_slot, dstr* src, uint src_start, uint src_end);
void dstr_pad(dstr* a, uint slot, int shift);
void dstr_pad_part(dstr* a, uint start, uint end, int shift);

void dstr_rem(dstr* s, uint slot);
void dstr_rem_part(dstr* s, uint start, uint end);

void dstr_test(void);
#endif
