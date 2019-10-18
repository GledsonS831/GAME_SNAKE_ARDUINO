#include <bits/stdc++.h>
#include <stack>

using namespace std;

int main(){
    int n;

    while (cin >> n && n != 0){
        bool eh = true;
        while (eh){
            stack <int> valores;
            int lista[n];
            for (int i = 0; i < n; i++){
                cin >> lista[i];
                if (lista[i] == 0){
                    eh = false;
                    cout << endl;
                    break;
                }
            }
            if (!eh) break;

            int k = 1;
            for (int i = 1; i <= n; i++){
                valores.push(i);
                while (!valores.empty() && lista[k-1] == valores.top()){
                    k++;
                    valores.pop();
                }
            }
            if (valores.empty()) cout << "Yes" << endl;
            else cout << "No" << endl;
        }
    }
    return 0;
}