#include "../../checker.h" // directory'file 

#include <iostream>
#include <string>

using namespace std;

void isPermutation(vector<int> & vec){
    int n = (int) vec.size();
    vector<int> number(n+1,1);
    for (int i = 0; i < n; ++i){
        if (vec[i] > n || vec[i] <= 0){
            wrong_answer("En la posicion %d, el numero es %d, el cual, no pertence al rango de una permutacion de size %d\n", i, vec[i], n);
        }
        number[vec[i]]--;
        if (number[vec[i]] < 0){
            wrong_answer("El elemento %d se encuentra repetido\n", vec[i]);
        }
    }

    for (int i = 1; i <= n; ++i){
        if (number[i] == 1){
            wrong_answer("El numero %d falta\n", i);
        }
    }
}


int main(int argc, char * argv[]){


    init_io(argc, argv);

  

    vector<int> entrada;
    int casos = read_int(judge_in);


    //cout<<casos<<endl;

    
    for (int i = 0; i < casos; ++i){
        int x = read_int(judge_in);
        //cout<<"CO: "<<x<<endl;
        
        // Agarramos cada caso de prueba del jurado y del usuario
        if (x == 2 || x == 3){
            vector<string> resultado_original = read_strings(judge_ans,2);
            vector<string> resultado_participante= read_strings(author_out,2);

            if (!((resultado_original[0] == resultado_participante[0]) && 
                (resultado_original[1] == resultado_participante[1]))){
                    wrong_answer("No coincide con las opciones\n");
                }
        }else{
            vector<int> resultado_participante = read_ints(author_out, x);
            read_ints(judge_ans, x);
            isPermutation(resultado_participante);
            for (int i = 1; i < x; ++i){
                if (abs(resultado_participante[i] - resultado_participante[i-1]) == 1){
                    wrong_answer("En la posicion %d y %d, sus valores son %d, %d\n", i-1, i, resultado_participante[i-1], resultado_participante[i]);
                }
            }
        }
    }

    read_EOF(author_out);
}