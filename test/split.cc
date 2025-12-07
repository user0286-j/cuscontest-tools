#include <bits/stdc++.h>

using namespace std;

vector<string> split(string s, const string& delimiter) {
    vector<string> tokens;
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        if ((int) token.length() != 0) tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}


int main(){
    string s; 
    getline(cin, s);
    cout<<s<<endl;
    cout<<endl;
    for (auto x: split(s, " ")) cout<<"->"<<x<<endl;
}