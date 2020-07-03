#include <stdio.h>

int primo(int n){
    //printf("entrei no primo %d\n", n);    
    if(n < 2) return 0;
    for(int i=2; i<n; i++){
        if(n % i == 0) return 0;
        }
    //printf("1\n");
    return 1;
    }


int main(){
    char c;
    int i;
    int tam = 0;
    while(scanf("%d%c", &i, &c)){
        tam += primo(i);
        if(c=='\n') break;
    }
    printf("%d\n", tam);
    return 0;
}
