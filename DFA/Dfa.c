//3_14 문제 p.88 DFA_Recognize 알고리즘을 사용하여 구현하기 
#include <stdio.h>

int state = 3;
int symbol = 2;


int state_table[3][2] = {
	{ 1, 0 },
	{ 2, 0 },
	{ 2, 2 }
}; //p=0,q=1,r=2라고 가정

int main() {
	char str;	
	int current_state = 0;	


	str = getchar(); // next symbol get

	while (str != '\n') {

		current_state = state_table[current_state][str-'0'];

		str= getchar();
		
	}
	if (current_state == 2)//현재 상태가 final상태일 때. 현재 이 식에서 final은 r, r=2
	{
		printf("Accept");
	}
	else printf("Reject");

	
}
