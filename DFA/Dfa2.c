#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int state = 5;
int symbol = 2;


int state_table[5][2] = {
   {1,0},
   {3,2},
   {4,-1},
   {3,4},
   {3,0}

};

/*
   0: A=[q0]
   1: B=[q1,q2]
   2: C=[q1]
   3: D=[q0,q1,q2]
   4: E=[q0,q1] 라고 가정
*/
#define max 100
int main() {
    char str[max];
    int current_state = 0;
    int final_state = 0;
    int i = 0;
    scanf("%s", str);
    int len = strlen(str);
    while (i != len) {
        current_state = state_table[current_state][str[i++] - 'a'];
        printf("%d\n", current_state);
    }
    if (current_state == 1 || current_state == 3)//현재 상태가 final상태일 때. 현재 이 식에서 final은 1, 3
    {
        printf("Accept");
    }
    else printf("Reject");

}
