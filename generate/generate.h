#pragma once

#define VERSION "0.1"

#include <iostream>
#include <random>
#include <ctime>

std::mt19937 generator(0);

namespace gen{

    void init_generate(int argc, char * argv[], int seed = 0){
        generator.seed(seed);
    }

    void write_int(int mi = 0, int ma = 2147483647){
        int a = (generator()%(ma+1)) + mi;
        cout<<a<<endl;
    }

    void write_ints(int n, int mi = 0, int ma = 2147483647){
        for (int i = 0;  i < n ; ++i){
            int a = (generator()%(ma+1)) + mi;
            cout<<a<<endl;
        }
    }

}

