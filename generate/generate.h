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

    int gen_int(int mi = 0, int ma = 2147483647){
        int a = (generator()%(ma+1ll)) + 1;
        return a;
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

    void write_graph(int n, int m, bool direct = false, bool connect = true){
        cout<<n<<" "<<m<<endl;
        vector<vector<int>> g(n+1);
        int ver = 1;
        int x = 0;
        while (x <= m){
            int a = gen_int(1, n);
            while (a != ver){
                a = gen_int(1,n);
            }

            g[ver].push_back(a);
            cout<<ver<<" "<<a<<endl;

            if (!direct){
                g[a].push_back(ver);
            }

            x++;
            ver = ver%n + 1;
        }

    }

}

