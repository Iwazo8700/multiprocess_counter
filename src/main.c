/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#define NUM_PROC 4




int primo(int n){
    //printf("entrei no primo %d\n", n);    
    if(n < 2) return 0;
    for(int i=2; i<n; i++){
        if(n % i == 0) return 0;
        }
    //printf("1\n");
    return 1;
    }

void sigquit(){
        //printf("sigquit\n");
        exit(0);
    }
int all_proc(int *proc){
    
        for(int i=0; i<NUM_PROC; i++)
            if(!proc[i]) return 0;
        return 1;
    }


int main() {
    pid_t filho[NUM_PROC];
    int tam=0, aux = 0;
    char c;
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANON;
    int *count, *numero, *init, *processo;
    count = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
    numero = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
    processo = (int*) mmap(NULL, sizeof(int)*NUM_PROC, protection, visibility, 0, 0);
    init = (int*) mmap(NULL, sizeof(int)*NUM_PROC, protection, visibility, 0, 0);
    (*count) = 0;
    (*numero) = 0;
    for(int i=0; i<NUM_PROC; i++){
        processo[i]=0;
        init[i] = 0;
    }
    while(scanf("%d%c", numero, &c)){
        
        tam++;
        
        for(int i=NUM_PROC-1; i>=0; i--){
            if(!((tam%NUM_PROC)%(i+1))){
                //printf("\ntam = %d, filho = %d\n", tam, i);
                //printf("init = %d\n", init[i]);
                if(!init[i]){
                    init[i] = 1;
                    filho[i] = fork();
                    //printf("iniciei filho %d\n", i);
                    if(!filho[i]){
                        signal(SIGQUIT, sigquit);
                        //printf("entrei no filho %d\n", i);
                        while(1){
                            if(processo[i]){
                                processo[i] = 0;
                                (*count)+=primo(*numero);
                                sleep(1);
                            }
                        }
                    }else{
                        processo[i] = 1;
                        sleep(2);
                    }
                    
                }else if (all_proc(init)){
                    processo[i] = 1;
                    sleep(2);
                }
            break;
            }
        }
        //printf("oioi\n");
       if(c == '\n') break; 
    }

    for(int j=0; j<4; j++){
        if(init[j]) kill(filho[j], SIGQUIT);
        waitpid(filho[j], NULL, 0);}
  printf("%d\n", (*count));
  return 0;
}
