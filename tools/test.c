#include <stdio.h>
void print_int(long long num){
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

void print_hex(long long num){
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
                        case 0:
                                printf("0");
                                break;
                }
    }
        if(digitiszero == 0){
                printf( "0");           // hacky solution but whatever
        }
}
/*            int arr[MAX_INTERGER_SIZE];
    int j = 0;
    int digitscount = 0;

    while(num != 0){
        int mod = num % 16;
        arr[j] = mod;
        num /= 16;
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
                        print_char(ft_ctx, '1');
                        break;
                case 2:
                        print_char(ft_ctx, '2');
                        break;
                case 3:
                        print_char(ft_ctx, '3');
                        break;
                case 4:
                        print_char(ft_ctx, '4');
                        break;
                case 5:
                        print_char(ft_ctx, '5');
                        break;
                case 6:
                        print_char(ft_ctx, '6');
                        break;
                case 7:
                        print_char(ft_ctx, '7');
                        break;
                case 8:
                        print_char(ft_ctx, '8');
                        break;
                case 9:
                        print_char(ft_ctx, '9');
                        break;
                case 10:
                        print_char(ft_ctx, 'A');
                        break;
                case 11:
                        print_char(ft_ctx, 'B');
                        break;
                case 12:
                        print_char(ft_ctx, 'C');
                        break;
                case 13:
                        print_char(ft_ctx, 'D');
                        break;
                case 14:
                        print_char(ft_ctx, 'E');
                        break;
                case 15:
                        print_char(ft_ctx, 'F');
                        break;
                case 0:
                        print_char(ft_ctx, '0');
                        break;
        }
                
    }
    if(digitscount == 0){
        print_char(ft_ctx, '0');
    }
} */

