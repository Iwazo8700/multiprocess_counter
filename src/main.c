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
    /*funcao que verifica se um numero eh primo ou nao
      funcao nao esta otimizada, portanto ele faz o calculo
      verifica a divisao inteira de todos os numeros inteiros
       menores que n e maiores que 0.*/

    if(n < 2) return 0;
    for(int i=2; i<n; i++){
        if(n % i == 0) return 0;
        }
    
    return 1;
    }


void sigquit(){
    /*funcao que recebe um sinal e encerra o processo*/
    exit(0);
}


int all_proc(int *proc){
    /*funcao que verifica se todos os valores dentro de um vetor
      sao verdadeiros. No caso desse codigo sele serve para verificar
      se ja iniciei a quantidade de processos desejada*/

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
    /*variaveis mmap para q todos os processos tenham acesso*/
    int *count, *numero, *init, *processo;
    count = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
    numero = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
    processo = (int*) mmap(NULL, sizeof(int)*NUM_PROC, protection, visibility, 0, 0);
    init = (int*) mmap(NULL, sizeof(int)*NUM_PROC, protection, visibility, 0, 0);
    (*count) = 0;
    (*numero) = 0;

    /*incializando os vetores com 0*/
    for(int i=0; i<NUM_PROC; i++){
        processo[i]=0;
        init[i] = 0;
    }

    /*loop principal para pegar varios numeros*/
    while(scanf("%d%c", numero, &c)){
        
        tam++;
        
        for(int i=NUM_PROC-1; i>=0; i--){

            //if para mudar entre qual processos e decidir qual sera o processo a executar a conta 
            if(!((tam%NUM_PROC)%(i+1))){
                //caso o processo ja tenha sido iniciado so preciso mandar um 'sinal' para que ele 
                //execute a conta necessaria
                if(!init[i]){
                    init[i] = 1;
                    filho[i] = fork();
                    //se for o processo filho o sinal entra num loop infinito e encerra qdo receber
                    //um sinal de saida, 
                    if(!filho[i]){
                        signal(SIGQUIT, sigquit);
                        while(1){
                            //verificador do 'sinal' para fazer a conta
                            if(processo[i]){
                                processo[i] = 0;
                                (*count)+=primo(*numero);
                                sleep(0.5);
                            }
                        }
                    //se for o processo pai so envio o sinal para fazer a conta
                    }else{
                    
                        processo[i] = 1;
                        sleep(1);
                    }
                //se todos ja foram iniciados so enviar um 'sinal' para fazer a conta no processo i
                }else if (all_proc(init)){
                    processo[i] = 1;
                    sleep(1);
                }
            break;
            }
        }
        //if de saida para \n
       if(c == '\n') break; 
    }
    //loop para encerrar todos os processos
    for(int j=0; j<4; j++){
        if(init[j]) kill(filho[j], SIGQUIT);
        waitpid(filho[j], NULL, 0);}
  //print do resultado final
  printf("%d\n", (*count));
  return 0;
}
