#include "array.h"
#include "math/math.h"
#include "../libs.h"
#include "../conf.h"

#define DE_ARRAY_GROW_FACTOR 4

uint Darr_grow_factor = DE_ARRAY_GROW_FACTOR;

darr* darr_new(size_t type, uint initial_capacity, void* initial_content){
	darr* a = malloc(sizeof(struct darr));

	if(initial_capacity==0){initial_capacity=1; initial_content=NULL;}
	a->data = malloc(type * initial_capacity);

	a->valid = DARR_VALID_KEY;

	a->type = type;
	a->len = 0;
	a->cap = initial_capacity;

	if(initial_content != NULL){
		uint i;
		for(i=0; i<initial_capacity; i++){
			darr_add(a, initial_content + i * type);
		}
	}

	return a;
}

void darr_free(darr* a){ DARR_VALIDATE(a);
	free(a->data); a->data = NULL;

	a->valid = a->type = a->len = a->cap = 0;
	a->valid = 0;

	free(a);
}

bool darr_valid(darr* a){
	if(a == NULL) return false;
	return a->valid == DARR_VALID_KEY;
}

uint darr_len(darr* a){DARR_VALIDATE(a); return a->len;}

size_t darr_type_size(darr* a){DARR_VALIDATE(a); return a->type;}

uint darr_set_len(darr* a, uint new_len){ DARR_VALIDATE(a);
	uint old_len = a->len;
	if(a->cap < new_len){
		int f = 2;
		while(f*a->cap < new_len)
			f++;
		darr_grow(a, f);
	}
	// while(new_len >= a->cap) darr_grow(a);
	a->len = new_len;
	return old_len;
}


void darr_set(darr* a, uint slot, void* val){ DARR_VALIDATE(a);
	// if(slot >= a->len) return;
	if(slot >= a->cap){
		int f = 2;
		while(f*a->cap < slot)
			f++;
		darr_grow(a, f);
	}
	memcpy(a->data + slot * a->type, val, a->type);
	a->len = MAX(a->len, slot);

}

// fixme ?
void darr_set_part(darr* a, uint start, uint end, void* vals){ DARR_VALIDATE(a);
	start = CLAMP(start, 0, a->len-1);
	end   = CLAMP(end,   0, a->len-1);
	if(start > end) return;

	uint i;
	for(i=start; i<end; i++){
		darr_set(a, i, vals + (i-start) * a->type );
	}
}

void* darr_get(darr* a, uint slot){ DARR_VALIDATE(a);
	if(slot >= a->len)
		slot = a->len-1;

	return a->data + slot * a->type;
}

void darr_add(darr* a, void* value){ DARR_VALIDATE(a);
	if(a->len >= a->cap) darr_grow(a, 0);

	memcpy(a->data + a->len * a->type, value, a->type);

	a->len++;
}

void darr_pop(darr* a){ DARR_VALIDATE(a);
	if(a->len == 0) return;

	a->len--;
}

void darr_zero(darr* a, uint slot){ DARR_VALIDATE(a);
	if(slot >= a->len) slot = a->len-1;
	memset(a->data + slot * a->type, 0, a->type);
}


void darr_grow(darr* a, int factor){ DARR_VALIDATE(a);
	if((factor==0) || (factor==1))
		factor = Darr_grow_factor;

	uint new_cap = factor *(a->cap+1);

	a->data = realloc(a->data, new_cap * a->type);

	a->cap = new_cap;
}


void darr_add_zero(darr* a){ DARR_VALIDATE(a);
	if(a->len >= a->cap) darr_grow(a, 0);

	memset(a->data + a->len * a->type, 0, a->type);

	a->len++;
}

darr* darr_dup(darr* src){ DARR_VALIDATE(src);
	return darr_new(src->type, src->len, src->data);
}

darr* darr_dup_part(darr* src, uint start, uint end){ DARR_VALIDATE(src);
	start = CLAMP(start, 0, src->len-1);
	end   = CLAMP(end,   0, src->len-1);

	if(start < end)
		return darr_new(src->type, end - start, src->data + start*src->type);
	else
		return darr_new(src->type, 0, NULL);
}

void darr_cat(darr* dst, darr* src){ DARR_VALIDATE(dst); DARR_VALIDATE(src);
	for (uint i = 0; i < src->len; ++i){
		darr_add(dst, darr_get(src,i));
	}
}


void darr_ins(darr* a, uint slot, void* value){ DARR_VALIDATE(a);
	darr_ins_part(a, slot, slot+1, &value);
}

void darr_ins_arr(darr* dst, uint slot, darr* src){ DARR_VALIDATE(dst); DARR_VALIDATE(src);
	// darr_insa_part(dst,slot,src,0,darr_len(src));
	uint i, len;
	len = darr_len(src);
	darr_pad(dst, slot, len);
	for(i=0; i<len; i++){
		darr_set(dst, slot+i, darr_get(src, i));
	}
}

void darr_insa_part(darr* dst, uint slot, darr* src, uint src_start, uint src_end){ DARR_VALIDATE(src); DARR_VALIDATE(dst);
	// void* vals = darr_get(src, src_start);
	// darr_ins_part(dst, slot, src_end - src_start, &vals);
	uint i, j, len;
	len = src_end - src_start;
	j = 0;
	darr_pad(dst, slot, len);
	for(i=src_start; i<src_end; i++, j++){
		darr_set(dst, slot + j, darr_get(src, i));
	}
}

void darr_ins_part(darr* a, uint start, uint end, void** values){ DARR_VALIDATE(a);
	start = CLAMP(start, 0, a->len);
	end   = CLAMP(end,   0, a->len);

	if(start > end) return;

	darr_pad(a, start, end - start);

	uint i;
	for(i=start; i<end; i++)
		darr_set(a, i, (*values) + i * a->type);
		// darr_set(a, i, (*values)+ start+i *a->type);
}

void darr_pad(darr* a, uint slot, int shift){ DARR_VALIDATE(a);
	if(shift==0) return;
	if(slot > a->len) slot = a->len;
	if(shift>0){
		int f = 1;
		while(a->len + shift > a->cap*f)
			f++;
		darr_grow(a, f);

		memmove(
			a->data + (slot+shift)*a->type,
			a->data + slot * a->type,
			(a->len - slot)*a->type );

		// printf("%i\n", a->len);
		a->len+=abs(shift);
		// printf("%i\n", a->len);
	} else if(shift<0){
		int islot = slot;
		if(islot + shift < 0) shift = -islot;
		memmove(
			a->data + (islot+shift)*a->type,
			a->data + slot * a->type,
			(a->len - slot)* a->type);
		// printf("%i\n", a->len);
		a->len-=abs(shift);
		// printf("%i\n", a->len);

	}
}
// void arrayPadPart(darr* a, uint start, uint end, int shift);

void darr_rem(darr* a, uint slot){ DARR_VALIDATE(a);
	darr_rem_part(a, slot, slot + 1);
}

void darr_rem_part(darr* a, uint start, uint end){ DARR_VALIDATE(a);
	int shift = end-start;
	darr_pad(a, end, -shift);
}


uint darr_index_of(darr* arr, void* address);



void darr_swap(darr* a, uint slota, uint slotb){ DARR_VALIDATE(a);
	if(slota == slotb) return;
	uint len = a->len;
	if(slota > len) return;
	if(slotb > len) return;
	size_t type = a->type;
	void* temp = alloca(type);
	void* ap = darr_get(a, slota);
	void* bp = darr_get(a, slotb);
	memcpy(temp, ap, type);
	memcpy(ap,   bp, type);
	memcpy(bp, temp, type);
}




































// --------------------------------------------------------







void darr_test(void){
	darr* a = darr_newb(darr*);
	darr* a2;
	uint i,j; uint si = 32, sj = 16;
	int v;
	for(i=0; i<si; i++){
		a2 = darr_newb(int);
		darr_add(a, &a2);
	}
	for(i=0; i<si; i++){
		for(j=0; j<sj; j++){
			v = i+j;
			// v = deVec4i(i, j, i+j, rand());
			darr_add(darr_read(darr*, a, i), &v);
			// darr_add(*a2p, &v);
		}
	}
	for(i=0; i<si; i++){
		for(j=0; j<sj; j++){
			printf("%i\t",darr_read(int, darr_read(darr*, a, i), j));
			// deV4iPrint(darr_read(int, darr_read(array, a, i), j));
			// printf("%i\t",*(int*)darr_get(*(array*)darr_get(a,i),j));
		}
		printf("\n");
	}

	printf("deleting\n");

	for(i=0; i<si; i++){
		darr_free(darr_read(darr*, a,i));
	}
	darr_free(a);



	// printf("%s\n", "darr* test cat");
	darr* ars[2];
	char hola[] = "hola ";
	ars[0] = darr_new(sizeof(char), sizeof(hola), hola);
	ars[1] = darr_new(sizeof(char), 7, "mundo\n");
	darr_cat(ars[0], ars[1]);

	for (uint i = 0; i < ars[0]->len; ++i){
		printf("%c", darr_read(char, ars[0], i));
	}

}
