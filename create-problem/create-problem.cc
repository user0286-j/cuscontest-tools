#define STATE 0
#define VERSION 0.1

#define CORRECT 0
#define ERROR 1

#if defined(__linux__)
#define mkdir "mkdir"
#define ls "ls"
#define cat "cat"
#define output ">"
#define PYTHON "python3"
#elif defined(_WIN32) || defined(_WIN64)
#define mkdir "New-Item -Path \".\" -ItemType Directory -name"
#define ls "ls"
#define cat "Get-Content"
#define output "Set-Content"
#define PYTHON "py"
#else 
#undef STATE
#define STATE 1

#include<iostream>
#include<cstdlib>
#include<string>

using namespace std;

int TIMELIMIT = 1;
int MEMORYLIMIT = 64;
int AUTHOR = "UNKNOW";
int COLOR = "#ffffff";

const char *LOG_STATE[] = {
    "Correcto",
    "Sistema Operativo no reconocido"
};

string STATE_MEANING(){
    return LOG_STATE[STATE];
}

/*
    Parámetros fundamentales
    ./run <ruta_del_archivo>
    Dentro de <ruta_del_archivo> debe estar los siguientes archivos, ademas, la ruta del archivo debe contener 
        - content.tex 
        - input.tex
        - output.tex
        - Test
        - Solution 
        - Solution.*
    Parámetros especiales
    - timelimit (segundos): Por defecto 1
    - memorylimit (megabyte): Por defecto 64
    - author: Por defecto "Unknow"
    - color: Por defecto "#ffffff"
    
    * Generate new file as main.tex
*/

void process_arguments(int argc, char * argv[]);


signed main(int argc, char * argv[]){
    // Verificar si está bien el sistema operativo
    if (STATE != 0){
        exit(ERROR);
    }

    // Primer paso: Verificar los parámetros //
    process_arguments(argc, argv);

    // Segundo Paso: Recolectar todos los archivos  //
    


    return false;
}

void process_arguments(int argc, char * argv[]){
    if (argc < 2){
        cerr << "Falta direccion del archivo" << endl;
        exit(ERROR);
    }

    if (argc % 2 != 0){
        cerr << "Cantidad de parametros inexacta" << endl;
        exit(ERROR);
    }

    // Agarrar parametros expeciales 
    for (int parameters = 2; parameters < argc; ++parameters){
        string nombre = argv[parameters];
        if (nombre == "timelimit"){
            try{
                TIMELIMIT = stoi(argv[parameters + 1]);
            } catch (){
                cerr << "NO ES UN NUMERO" << endl;
                exit(ERROR);
            }
        } else if (nombre == "memorylimit"){
            try{
                MEMORYLIMIT = stoi(argv[parameters + 1]);
            } catch (){
                cerr << "NO ES UN NUMERO" << endl;
                exit(ERROR);
            }
        } else if (nombre == "author"){
            try{
                AUTHOR = argv[parameters + 1];
            } catch (){
                cerr << "ERROR INESPERADO EN EL NOMBRE DE AUTHOR" << endl;
                exit(ERROR);
            }
        } else if (nombre == "color"){
            try{
                COLOR = argv[parameters + 1];
            } catch (){
                cerr << "ERROR INESPERADO EN EL NOMBRE COLOR" << endl;
                exit(ERROR);
            }
        } else {
            cerr << "PARAMETRO DESCONOCIDO" << endl;
            exit(ERROR);
        }
    }
}

