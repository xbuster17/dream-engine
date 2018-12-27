#include "string.h"
#include "../libs.h"

dstr* dstr_new(char* str){
	if(str)
		return darr_new(sizeof(char), strlen(str)+1, str);
	else
		return darr_new(sizeof(char), 1, "\0");
}

dstr* dstr_new_len(char* str, uint length){
	dstr* ret = darr_new(sizeof(char), strnlen(str, length), str);
	dstr_add(ret, '\0');
	return ret;
}

void dstr_free(dstr* str){
	darr_free(str);
}

void dstr_set(dstr* s, char* str){
	if(!str) {
		darr_zero(s, 0);
		darr_set_len(s, 1);
		return;
	};
	uint i, len;
	len = strlen(str)+1;
	darr_set_len(s, len);
	for(i=0; i<len; i++){
		darr_set(s, i, &(str[i]));
	}
	darr_zero(s, len);

}

uint dstr_len(dstr* s){
	uint len = darr_len(s);
	if(len > 0) return len-1;
	else return 0;
}

char* dstr_get(dstr* s){return darr_get(s, 0);}

char dstr_read(dstr* s, uint slot){return darr_read(char,s,slot);}

void dstr_add(dstr* s, char c){
	// darr_pop(s);
	darr_set(s, dstr_len(s), &c);
	// darr_add(s, &c);
	darr_add(s, "\0");
}

void dstr_pop(dstr* s){
	darr_pop(s);
	// darr_pop(s);
	darr_set(s, dstr_len(s), "\0");
}

void dstr_cat(dstr* dst, dstr* src){
	uint i;
	uint tlen = dstr_len(src);
	for(i=0; i<tlen; i++)
		dstr_add(dst, darr_read(char, src, i));
}

void dstr_cat_raw(dstr* dst, char* src){
	if(!dst) return;
	if(!src) return;
	uint i;
	uint tlen = strlen(src);
	for(i=0; i<tlen; i++)
		dstr_add(dst, src[i]);
}

dstr* dstr_dup(dstr* src){return darr_dup(src);}

void dstr_rem(dstr* s, uint slot){
	dstr_rem_part(s, slot, slot+1);
}

void dstr_rem_part(dstr* s, uint start, uint end){
	if(start > dstr_len(s)) start = dstr_len(s);
	if(end > dstr_len(s)) end = dstr_len(s);
	darr_rem_part(s, start, end);
}

void dstr_pad(dstr* s, uint slot, int shift){
	darr_pad(s,slot,shift);
	if(shift>0){
		int i;
		for(i=0; i<shift; i++){
			darr_set(s, slot + i, " ");
		}
		darr_zero(s, darr_len(s));
	}
}

void dstr_ins(dstr* s, uint slot, char c){
	dstr_pad(s, slot, 1);
	if(slot > dstr_len(s)) slot = dstr_len(s);
	darr_set(s, slot, &c);
}

void dstr_inss(dstr* dst, uint dst_slot, dstr* src){
	dstr_inss_part(dst, dst_slot, src, 0, dstr_len(src));
}

void dstr_inss_part(dstr* dst, uint dst_slot, dstr* src, uint src_start, uint src_end){
	if(src_start >= dstr_len(src)) src_start = dstr_len(src)-1;
	if(src_end   >= dstr_len(src)) src_end   = dstr_len(src)-1;
	if(dst_slot  >= dstr_len(dst)) dst_slot  = dstr_len(dst)-1;
	dstr_inss_part(dst, dst_slot, src, src_start, src_end);
}


/* case */
void dstr_to_lower(dstr* s){
	uint i;for(i=0; i<dstr_len(s); i++){
		char val = tolower(darr_read(char, s, i));
		darr_set(s, i, &val);
	}
}
void dstr_to_upper(dstr* s){
	uint i;for(i=0; i<dstr_len(s); i++){
		char val = toupper(darr_read(char, s, i));
		darr_set(s, i, &val);
	}
}

void dstr_swap(dstr* s, uint a, uint b){
	darr_swap(s,a,b);
}


bool dstr_valid(dstr* s){
	return darr_read(char, s, dstr_len(s)) == 0;
}




void str_shuffle(dstr* s){
	uint i; srand(time(0));
	for(i=0; i<1024; i++){
		dstr_swap(s, rand()%(dstr_len(s)-1),rand()%(dstr_len(s)-1));
	}
}






















void dstr_test(void){
	dstr* s1 = dstr_new("");
	dstr* s2 = dstr_new("");
	dstr* s3 = dstr_new("");

	char i;
	for(i='a'; i<='z';i++){
		dstr_ins(s1, dstr_len(s1), i);
	}

	// printf("%s\n", dstr_get(s1));
	assert(strcmp(s1->data, "abcdefghijklmnopqrstuvwxyz" ) == 0);

	dstr_free(s1),dstr_free(s2);dstr_free(s3);


	// {while(dstr_len(s2) > dstr_len(s1)/2) dstr_pop(s2);}
	// dstr_rem_part(s2, dstr_len(s2)-2, dstr_len(s2));

}
