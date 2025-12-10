#include "../../checker.h" // directory'file 

#include <iostream>
#include <string>

using namespace std;


int main(int argc, char * argv[]){


    init_io(argc, argv);

    vector<int> entrada;
    int casos = read_int(judge_in);
    
    for (int i = 0; i < casos; ++i){
        int x = read_int(judge_in);

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
            extra_contidional::is_permutation(resultado_participante);
            for (int i = 1; i < x; ++i){
                if (abs(resultado_participante[i] - resultado_participante[i-1]) == 1){
                    wrong_answer("En la posicion %d y %d, sus valores son %d, %d\n", i-1, i, resultado_participante[i-1], resultado_participante[i]);
                }
            }
        }
    }

    read_EOF(author_out);
}