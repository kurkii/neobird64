#include <stdio.h>
void print_int(int num){
    int arr[64];
    int j = 0;
    int digitscount = 0;

    while(num != 0){
        int mod = num % 10;
        arr[j] = mod;
        num /= 10;
        j++;

        if(j == 64){            // ub handling like any good programmer
                return;
        }

        digitscount = j;
        if(num == 0){
                arr[j] = mod;
        }
    }
    for(int i = digitscount-1; i != -1; i--){
        switch(arr[i]){
                case 1:
                        printf("1");
                        break;
                case 2:
                        printf( "2");
                        break;
                case 3:
                        printf( "3");
                        break;
                case 4:
                        printf( "4");
                        break;
                case 5:
                        printf( "5");
                        break;
                case 6:
                        printf( "6");
                        break;
                case 7:
                        printf( "7");
                        break;
                case 8:
                        printf( "8");
                        break;
                case 9:
                        printf( "9");
                        break;
                case 0:
                        printf( "0");
                        break;
        }
                
    }
    if(digitscount == 0){
        printf( "0");
    }
}

void print_hex(int num){
    int arr[64];
    int j = 0;
    int digitscount = 0;
    int digitiszero = 1;

    while(num != 0){
        int mod = num % 16;
        arr[j] = mod;
        num /= 16;
        j++;
        digitscount = j;
    }
    for(int i = digitscount-1; i != -1; i--){
                switch(arr[i]){
                        case 1:
                                printf( "1");
                                break;
                        case 2:
                                printf( "2");
                                break;
                        case 3:
                                printf( "3");
                                break;
                        case 4:
                                printf( "4");
                                break;
                        case 5:
                                printf( "5");
                                break;
                        case 6:
                                printf( "6");
                                break;
                        case 7:
                                printf( "7");
                                break;
                        case 8:
                                printf( "8");
                                break;
                        case 9:
                                printf( "9");
                                break;
                        case 10:
                                printf( "A");
                                break;
                        case 11:
                                printf( "B");
                                break;
                        case 12:
                                printf( "C");
                                break;
                        case 13:
                                printf( "D");
                                break;
                        case 14:
                                printf( "E");
                                break;
                        case 15:
                                printf( "F");
                                break;
                }
    }
        if(digitiszero == 0){
                printf( "0");           // hacky solution but whatever
        }
}

