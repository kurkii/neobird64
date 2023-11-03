#include <stdio.h>

int main(void){
	for(int i = 0; i != 32; i++){
		printf("extern void s_isr%d();\n", i);
	}
	return 0;
}
