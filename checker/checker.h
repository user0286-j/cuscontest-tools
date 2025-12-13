#pragma once

#define version 1.0

#define ANSWER_CORRECT 42
#define EXITCODE_AC 42
#define EXITCODE_WA 43
#define WRONG_ANSWER 43

#define FILENAME_AUTHOR_MESSAGE "teammessage.txt"
#define FILENAME_JUDGE_MESSAGE "judgemessage.txt"
#define FILENAME_SCORE "score.txt"

#define USAGE "%s: judge_in judge_ans feedback_dir < author_out\n"

#include <sys/stat.h>
#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include <limits>

typedef void (*feedback_function)(const std::string &, ...);

using namespace std;

#define max(a,b) (a> b? a: b)


std::ifstream judge_in, judge_ans;
std::istream author_out(std::cin.rdbuf());

char *feedbackdir = NULL;

void vreport_feedback(const std::string &category,
                      const std::string &msg,
                      va_list pvar) {
    std::ostringstream fname;
    if (feedbackdir)
        fname << feedbackdir << '/';
    fname << category;
    FILE *f = fopen(fname.str().c_str(), "a");
    assert(f);
    vfprintf(f, msg.c_str(), pvar);
    fclose(f);
}

void report_feedback(const std::string &category, const std::string &msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(category, msg, pvar);
}

void author_message(const std::string &msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(FILENAME_AUTHOR_MESSAGE, msg, pvar);
}

void judge_message(const std::string &msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(FILENAME_JUDGE_MESSAGE, msg, pvar);
}

void wrong_answer(const std::string &msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(FILENAME_JUDGE_MESSAGE, msg, pvar);
    exit(EXITCODE_WA);
}

void judge_error(const std::string &msg, ...) {
    va_list pvar;
    va_start(pvar, msg);
    vreport_feedback(FILENAME_JUDGE_MESSAGE, msg, pvar);
    assert(0);
}

void accept() {
    exit(EXITCODE_AC);
}

void accept_with_score(double scorevalue) {
    report_feedback(FILENAME_SCORE, "%.9le", scorevalue);
    exit(EXITCODE_AC);
}


bool is_directory(const char *path) {
    struct stat entry;
    return stat(path, &entry) == 0 && S_ISDIR(entry.st_mode);
}

void init_io(int argc, char **argv) {
    if(argc < 4) {
        fprintf(stderr, USAGE, argv[0]);
        judge_error("Usage: %s judgein judgeans feedbackdir [opts] < userout", argv[0]);
    }

    if (!is_directory(argv[3])) {
        judge_error("%s: %s is not a directory\n", argv[0], argv[3]);
    }
    feedbackdir = argv[3];

    judge_in.open(argv[1], std::ios_base::in);
    if (judge_in.fail()) {
        judge_error("%s: failed to open %s\n", argv[0], argv[1]);
    }

    judge_ans.open(argv[2], std::ios_base::in);
    if (judge_ans.fail()) {
        judge_error("%s: failed to open %s\n", argv[0], argv[2]);
    }

    author_out.rdbuf(std::cin.rdbuf());
}


// =================================

template<typename T>
bool can_read(const std::string & s, T& out){
    std::stringstream ss(s);
    ss >> out;
    return ss && ss.eof(); 
}

vector<string> split(string s, const char delimiter = ' ') {
    vector<string> tokens;
    int fin = (int) s.length();
    int pos = 0;
    string token = "";
    while (pos < fin){
        if (s[pos] == delimiter){
            if (token != ""){
                tokens.push_back(token);
                token = "";
            }
        }else{
            token += s[pos];
        }
        pos++;
    }
    if (token != ""){
        tokens.push_back(token);
    }

    return tokens;
}



string read_line(istream &state, bool EOL = true){
    if (state.eof()){
        wrong_answer("Estamos en el EOF");
    }

    //state.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    string linea;
    getline(state, linea);

    if (!EOL){
        state.putback('\n');
    }

    return linea;
}

bool is_number(string ss){
    if (ss.empty()) return false;

    for (int i = 0; i < (int) ss.length(); ++i){
        if (!std::isdigit(static_cast<unsigned char>(ss[i]))){
            return false;
        }
    }

    return true;
}

int read_int(istream &state, bool END = true){
    string linea = read_line(state, END);
    vector<string> lineas = split(linea, ' ');

    if ((int) lineas.size() == 1){
        if (is_number(lineas[0])){
            return std::stoi(lineas[0]);
        }else{
            wrong_answer("%s debe ser un numero\n", lineas[0]);
        }
    }else{
        wrong_answer("Solo debe tener un elemento pero tiene %d\n", (int) lineas.size());
    }

    return -1;
}

template <typename T>
T read_element(istream &state, bool END = true){
    string linea = read_line(state, END);
    vector<string> lineas = split(linea, ' ');

    if ((int) lineas.size() != 1){
        wrong_answer("Solo debe haber un elemento pero hay %d elementos\n", (int) lineas.size());
    }

    T element;
    if (can_read(linea, element)){
        return element;
    }else{
        wrong_answer("La conversion es incompatible en read_element\n");
    }

    return element;
}

vector<int> read_ints(istream &state, int n= 1, bool END = true){
    string linea = read_line(state, END);
    vector<string> lineas = split(linea, ' ');
    vector<int> ans;
    
    if ((int) lineas.size() == n){
        
        for (int i = 0; i < n; ++i){
            if (is_number(lineas[i])){
                ans.push_back(std::stoi(lineas[i]));
            }else{
                wrong_answer("La posicion %d debe ser un numero\n", i);
            }
        }
        return ans;
    }else{
        wrong_answer("Debe tener %d elemento pero tiene %d\n", n, (int) lineas.size());
    }
    return ans;
}


long read_long(istream &state, bool END = true){
    string linea = read_line(state, END);
    vector<string> lineas = split(linea, ' ');
    if ((int) lineas.size() == 1){
        if (is_number(lineas[0])){
            return std::stol(lineas[0]);
        }else{
            wrong_answer("%s debe ser un numero\n", lineas[0]);
        }
    }else{
        wrong_answer("Solo debe tener un elemento pero tiene %d\n", (int) lineas.size());
    }
    return -1;
}

vector<long> read_longs(istream &state, int n= 1, bool END = true){
    string linea = read_line(state, END);
    vector<string> lineas = split(linea, ' ');
    vector<long> ans;
    if ((int) lineas.size() == n){
        
        for (int i = 0; i < n; ++i){
            if (is_number(lineas[i])){
                ans.push_back(std::stol(lineas[i]));
            }else{
                wrong_answer("La posicion %d debe ser un numero\n", i);
            }
        }
        return ans;
    }else{
        wrong_answer("Debe tener %d elemento spero tiene %d\n", n, (int) lineas.size());
    }

    return ans;
}

void read_EOF(istream &state){
    char c;
    while (state.get(c)){
        if (!((int) c == 10 || (int) c == 32)){
            wrong_answer("Se esperaba terminar, pero hay más elementos");
        }
    }
    if (state.eof()){
        accept();
    }
}

void ensure(bool estado, string mensaje = "ERROR", ...){
    va_list pvar;
    va_start(pvar, mensaje);
    if (!estado){
        vreport_feedback(FILENAME_JUDGE_MESSAGE, mensaje, pvar);
    }
}

bool is_float(const std::string& s) {
    try {
        size_t pos;
        std::stod(s, &pos);
        return pos == s.size(); 
    } catch (...) {
        return false;            
    }
}

float read_float(istream &state, bool END = true){
    string linea = read_line(state, END);
    vector<string> lineas = split(linea, ' ');
    if ((int) lineas.size() == 1){
        if (is_float(lineas[0])){
            return stod(lineas[0]);
        }else{
            wrong_answer("%s debe ser un numero\n", lineas[0]);
        }
    }else{
        wrong_answer("Solo debe tener un elemento pero tiene %d\n", (int) lineas.size());
    }

    return -1;
}

vector<string> read_strings(istream & state, int n = 1, bool END = true){
    string linea = read_line(state, END);
    vector<string> lineas = split(linea, ' ');

    if ((int) lineas.size() == n){
        return split(linea, ' ');
    }
    wrong_answer("Tamano incorrecto\n");

    return vector<string> (1,"ERROR");
}

template <typename T>
vector<T> read_elements(istream & state, int n = 1, bool END = true){
    string linea = read_line(state, END);
    vector<string> lineas = split(linea, ' ');

    vector<T> ans;

    if ((int) lineas.size() != n){
        wrong_answer("La lista tiene %d elementos, pero solo debe haber %d elementos\n", (int) lineas.size(), n);
        return ans;
    }

    for (int i = 0; i < n; ++i){
        T element;
        if (can_read<T>(lineas[i], element)){
            ans.push_back(element);
        }else{
            wrong_answer("Elemento leido no es compatible en read_elements\n");
        }
    }

    return ans;
}

bool is_range(int element, int mi, int ma){
    return ((mi <= element) && (element <= ma));
}

vector<vector<int>> read_graph(istream & state, bool dirigido = false){
    vector<int> datos = read_ints(state, 2);
    int n = datos[0];
    int m = datos[1];
    vector<vector<int>> grafo(n+1);
    for (int i = 0; i < m; ++i){
        vector<int> edge = read_ints(state, 2);
        int x = edge[0];
        int y = edge[1];

        if (!is_range(x, 1, n)){
            wrong_answer("el vertice %d esta fuera del rango", x);
        }

        if (!is_range(y, 1, n)){
            wrong_answer("el vertice %d esta fuera del rango", y);
        }

        grafo[x].push_back(y);
        if (!dirigido){
            grafo[y].push_back(x);
        }
    }

    return grafo;
}



void absolute_error(double ans, double out, int precision = 6){
    string s_ans = std::to_string(ans);
    string s_out = std::to_string(out);

    int pos = 0;
    int ma = max((int) s_ans.length(), (int) s_out.length());

    bool dot = false;
    int count = 0;
    while ((pos < ma) && (count < precision)){
        if (!dot){
            if (s_ans[pos] != s_out[pos]){
                wrong_answer("Son números distintos");
            }

            if (s_ans[pos] == '.'){
                dot |= true;
            }
            pos++;
        }else{
            if (s_ans[pos] != s_out[pos]){
                wrong_answer("En el decimal %d, los valores son diferentes %s, %s", count+1, s_ans[pos], s_out[pos]);
            }
            pos++;
            count++;
        }
    }
}

void absolute_error(string ans, string out, int precision = 6){
    string s_ans = ans;
    string s_out = out;

    int pos = 0;
    int ma = max((int) s_ans.length(), (int) s_out.length());

    bool dot = false;
    int count = 0;
    while ((pos < ma) && (count < precision)){
        if (!dot){
            if (s_ans[pos] != s_out[pos]){
                wrong_answer("Son números distintos");
            }

            if (s_ans[pos] == '.'){
                dot |= true;
            }
            pos++;
        }else{
            if (s_ans[pos] != s_out[pos]){
                wrong_answer("En el decimal %d, los valores son diferentes %s, %s", count+1, s_ans[pos], s_out[pos]);
            }
            pos++;
            count++;
        }
    }
}

template <typename T>
vector<vector<T>> read_matrix(istream & state, int rows, int columns){
    vector<vector<T>> ans;
    for (int i = 0; i < rows; ++i){
        vector<T> elements = read_elements<T>(state, columns);
        ans.push_back(elements);
    }

    return ans;
}

namespace extra_contidional{
    void is_permutation(vector<int> v){
        int n = (int) v.size();
        vector<int> numeros(n+1,0);
        for (int i = 0; i < n; ++i){

            if (!((v[i] >= 1) && (v[i] <= n))){
                wrong_answer("El número de la posición %d es %d, por lo cual, no pertenece a una permutación de %d\n", i, v[i], n);
            }

            if (numeros[v[i]] != 0){
                wrong_answer("El número %d se repite más de una vez\n", v[i]);
            }

            numeros[v[i]] = 1;
        }
    }
}

// READSPACE, READEOLN
// READDOUBLE
