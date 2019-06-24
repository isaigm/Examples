#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX 100
int comp(char const *);
int balanceados(const char *);
int main(){
    char cad[MAX], aux[MAX];
    int i, j;
    scanf("%[^\n]", cad);
    if(balanceados(cad)){
        for(i = 0, j = 0; i < (int) strlen(cad); i++){
            if(cad[i] != ' '){
                aux[j] = cad[i];
                j++;
            }
        }
        aux[j] = '\0';
        if(aux[0] == 'i' && aux[1] == 'f'){
            int flag = 1;
            for(int i = 0; i < (int) strlen(aux); i++){
                if(aux[i] == '(' && aux[i-1] == ')'){
                    flag = 0;
                    break;
                }
            }
            if(flag && comp(aux)){
               printf("It's ok\n");
            }else printf("Invalid sentence\n");
        }else printf("Invalid sentence\n");
    }
    if(3 > 0 && ( 6 > (5 < 0))){
        printf("Hey\n");
    }

    return 0;
}
int balanceados(const char *cad){
    int flag = 1, var = 0;
    for(int i = 0; i < (int)strlen(cad) && flag; i++){
        if(cad[i] == '(') var++;
        else if(cad[i] == ')'){
            if(!(var == 0)) var--;
            else flag = 0;
        }
    }
    if(var == 0 && flag) return 1;
    else return 0;

}
int comp(const char *cad){
    int flag = 1;
    for(size_t i = 0; i < strlen(cad) && flag; i++){
        switch (cad[i]) {
        case '&':
            if(cad[i-1] == '&' && (isalpha(cad[i-2]) != 0 || isdigit(cad[i-2]) != 0 || cad[i-2] == ')')){
                if(i+1 < strlen(cad)){
                    if(isalpha(cad[i+1]) != 0 || cad[i+1] == '('){}
                    else flag = 0;
                }else flag = 0;
            }else flag = 0;
            break;
        case '|':
            if(cad[i-1] == '|' && (isalpha(cad[i-2]) != 0 || isdigit(cad[i-2]) != 0 || cad[i-2] == ')')){
                if(i+1 < strlen(cad)){
                    if(isalpha(cad[i+1]) != 0 || cad[i+1] == '('){}
                    else flag = 0;
                }else flag = 0;
            }else flag = 0;
            break;
        case '>':
            if(isalpha(cad[i-1]) != 0 || isdigit(cad[i-1]) != 0 || cad[i-1] == ')'){
                if(i+1 < strlen(cad)){
                    if(isalpha(cad[i+1]) != 0 || cad[i+1] == '(' || isdigit(cad[i+1]) != 0){}
                    else flag = 0;
                }else flag = 0;
            }else flag = 0;
            break;
        case '<':
            if(isalpha(cad[i-1]) != 0 || isdigit(cad[i-1]) != 0 || cad[i-1] == ')'){
                if(i+1 < strlen(cad)){
                    if(isalpha(cad[i+1]) != 0 || cad[i+1] == '(' || isdigit(cad[i+1]) != 0){}
                    else flag = 0;
                }else flag = 0;
            }else flag = 0;
            break;
        case '=':
            if(cad[i-1] == '=' && (isalpha(cad[i-2]) != 0 || isdigit(cad[i-2]) != 0 || cad[i-2] == ')')){
                if(i+1 < strlen(cad)){
                    if(isalpha(cad[i+1]) != 0 || cad[i+1] == '(' || isdigit(cad[i+1]) != 0){}
                    else flag = 0;
                }else flag = 0;
            }else flag = 0;
            break;
        default:
            break;
        }
    }
    if(flag) return 1;
    else return 0;
}
