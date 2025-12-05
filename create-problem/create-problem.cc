#define STATE 0
#define VERSION 0.1

#define CORRECT 0
#define ERROR 1

#if defined(__linux__)
#define mkdir "mkdir"
#define ls "ls"
#define cat "cat"
#define PYTHON "python3"
#elif defined(_WIN32) || defined(_WIN64)
#define mkdir "New-Item -Path \".\" -ItemType Directory -name"
#define ls "ls"
#define cat "Get-Content"
#define PYTHON "py"
#else 
#undef STATE
#define STATE 1
#endif

#include<iostream>
#include<cstdlib>
#include<string>
#include<fstream>
#include<filesystem>
#include<utility>

using namespace std;

int TIMELIMIT = 1;
int MEMORYLIMIT = 64;
string AUTHOR = "UNKNOW";
string COLOR = "#ffffff";

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

void verify_files(string dir);

void run_solution(string dir);

signed main(int argc, char * argv[]){
    // Verificar si está bien el sistema operativo
    if (STATE != 0){
        exit(ERROR);
    }

    if (__cplusplus < 201703L){
        cerr << "VERSION NO SOPORTADA POR ESTE CODIGO, DEBE SER DE 17 PARA ARRIBA" << endl;
        exit(1);
    }

    process_arguments(argc, argv);

    verify_files(argv[1]);

    run_solution(argv[1]);


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
            } catch (int e){
                cerr << "NO ES UN NUMERO" << endl;
                exit(ERROR);
            }
        } else if (nombre == "memorylimit"){
            try{
                MEMORYLIMIT = stoi(argv[parameters + 1]);
            } catch (int e){
                cerr << "NO ES UN NUMERO" << endl;
                exit(ERROR);
            }
        } else if (nombre == "author"){
            try{
                AUTHOR = argv[parameters + 1];
            } catch (int e){
                cerr << "ERROR INESPERADO EN EL NOMBRE DE AUTHOR" << endl;
                exit(ERROR);
            }
        } else if (nombre == "color"){
            try{
                COLOR = argv[parameters + 1];
            } catch (int e){
                cerr << "ERROR INESPERADO EN EL NOMBRE COLOR" << endl;
                exit(ERROR);
            }
        } else {
            cerr << "PARAMETRO DESCONOCIDO" << endl;
            exit(ERROR);
        }
    }
}

bool there_is_file(string file){
    string nombre_archivo = file;
    ifstream archivo(nombre_archivo);

    if (archivo.is_open()){
        return true;
    }

    return false;
}

bool there_is_namefile(string dir, string name){
    namespace fs = filesystem;

    for (const auto & entry: fs::directory_iterator(dir)){
        if (entry.is_regular_file() && entry.path().stem() == name){
            return true;
        }
    }

    return false;
}



int count_extensions(const string & ruta, const string ext = ".in"){
    namespace fs = filesystem;
    int count = 0;

    for (const auto & entry: fs::directory_iterator(ruta)) {
        if (entry.is_regular_file() && entry.path().extension() == ext){
            count++;
        }
    }

    return count;
}

bool there_is_dir(const string& ruta) {
    namespace fs = filesystem;
    return fs::exists(ruta) && fs::is_directory(ruta);
}


void verify_files(string dir){
    string ruta = dir;

    string content = dir + "/content.tex";
    string input = dir + "/input.tex";
    string output = dir + "/output.tex";

    if (!there_is_file(content)){
        cerr << "NO SE ENCUENTRA EL ARCHIVO content.tex" << endl;
        exit(1);
    }

    if (!there_is_file(input)){
        cerr << "NO SE ENCUENTRA EL ARCHIVO input.tex" << endl;
        exit(1);
    }

    if (!there_is_file(output)){
        cerr << "NO SE ENCUENTRA EL ARCHIVO output.tex" << endl;
        exit(1);
    }

    string dir_test_public = dir + "/test/public/";
    string dir_test_private = dir + "/test/private/";

    if (!there_is_dir(dir_test_public)){
        cerr << "NO SE ENCUENTRA LA CARPETA public" << endl;
        exit(ERROR);
    }

    if (!there_is_dir(dir_test_private)){
        cerr << "NO SE ENCUENTRA LA CARPETA private" << endl;
        exit(ERROR);
    }


    int tests_public = 0;
    int tests_private = 0;

    cout << count_extensions(dir_test_public) << endl;
    cout << count_extensions(dir_test_private) << endl;

    if (!there_is_namefile(dir, "solution")){
        cerr << "NO SE ENCUENTRA ARCHIVO solution" << endl;
        exit(ERROR);
    }

}

pair<string, string> get_solution_dir(string & dir){
    namespace fs = filesystem;
    for (const auto & entry: fs::directory_iterator(dir)){
        if (entry.is_regular_file() && entry.path().stem() == "solution"){
            return {entry.path(), entry.path().extension()};
        }
    }

    cerr << "SE PRODUJO UN ERROR INESPERADO" << endl;
    exit(ERROR);
    return {0,0};
}


void run_solution(string dir){
    string cadena = "";
    #if defined(_WIN32) || defined(_WIN64)
        cadena = "powershell";
    #endif

    pair<string, string> solution_data = get_solution_dir(dir);
    string ext = solution_data.second;
    string dir_solution = solution_data.first;

    string lenguaje = "";

    if (ext == ".py"){
        lenguaje = PYTHON;
        lenguaje += " ";
    }else if (ext == ".c++" || ext == ".cc" || ext == ".cpp"){
        string compilar = "g++ " + dir_solution + " -o solution";
        system(compilar.c_str());

        lenguaje = "./solution ";
    }else{
        cerr << "Lenguaje de programacion no soportado" << endl;
        exit(ERROR);
    }


    namespace fs = filesystem;

    string dir_test_public = dir + "/test/public/";
    string dir_test_private = dir + "/test/private/";
    
    for (const auto & entry: fs::directory_iterator(dir_test_public)){
        if (entry.is_regular_file() && entry.path().extension() == ".in"){
            string ready = cadena;
            filesystem::path salida = entry.path();
            salida.replace_extension(".ans");
            #if defined(_WIN32) || defined(_WIN64)
                ready = "Get-Content " + entry.path().string() 
                + " | " + lenguaje + dir_solution 
                + "| Out-File" + entry.path().filename().string() + ".ans";
            #else
                ready = lenguaje + dir_solution 
                + " < " + entry.path().string() 
                + " > " + salida.string();
            #endif

            cout << ready << endl;
            system(ready.c_str());
        }
    }

    for (const auto & entry: fs::directory_iterator(dir_test_private)){
        if (entry.is_regular_file() && entry.path().extension() == ".in"){
            string ready = cadena;
            filesystem::path salida = entry.path();
            salida.replace_extension(".ans");
            #if defined(_WIN32) || defined(_WIN64)
                ready = "Get-Content " + entry.path().string() 
                + " | " + lenguaje + dir_solution 
                + "| Out-File" + entry.path().filename().string() + ".ans";
            #else
                ready = lenguaje + dir_solution 
                + " < " + entry.path().string() 
                + " > " + salida.string();
            #endif

            cout << ready << endl;
            system(ready.c_str());
        }
    }

}