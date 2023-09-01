
#include "../includes/verle.h"



void malloc_tv3(int size, t_arrv3* tv3_arr, char * error_msg){

	tv3_arr->len = 0;
	tv3_arr->size = size;

	if( (tv3_arr->addr = (t_v3*)malloc(tv3_arr->size * sizeof(t_v3))) == NULL){
		io_error(error_msg);
	}

}
