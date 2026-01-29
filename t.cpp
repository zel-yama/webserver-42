#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> c(n);
    for (int i = 0; i < n; i++) {
        cin >> c[i];
    }
    int a = 0, b =0;
    for (int i = 0; i < n; i++) {
        if (c[n -1] % 2 == 0) {
            if (i % 2 != 0) a += c[i];
            else b+= c[i];
        } else {
            if (i % 2 != 0) b += c[i];
            else a += c[i];
        }
    }
    cout << a << " " << b << endl;
}