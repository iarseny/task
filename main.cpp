#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>


using namespace std;

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

double rnd() {
    return double(rand()) / RAND_MAX;
}

int n, k;

int f(vector<pii> coords) {
    vector<vector<int>> mat(n + 1, vector<int>(n + 1));
    for (auto i : coords) {
        mat[i.first][i.second] = 1;
    }

    vector<vector<int>> rst(n + 1, vector<int>(n + 1));

    for (int i = 1; i <= n; i++) {
        bool has = false;

        for (int j = 1; j <= n; j++) {
            if (mat[i][j] == 1) {
                has = true;
            }
        }

        if (!has) continue;

        for (int j = 1; j <= n; j++) {
            rst[i][j] = 1;
        }
    }

    for (int i = 1; i <= n; i++) {
        bool has = false;

        for (int j = 1; j <= n; j++) {
            if (mat[j][i] == 1) {
                has = true;
            }
        }

        if (!has) continue;

        for (int j = 1; j <= n; j++) {
            rst[j][i] = 1;
        }
    }

    for (int i = 2; i <= 2 * n; i++) {
        bool has = false;
        for (int x = 1; x <= n; x++) {
            if (x >= i || i - x > n) continue;

            int y = i - x;

            if (mat[x][y] == 1) {
                has = true;
            }
        }

        if (!has) continue;

        for (int x = 1; x <= n; x++) {
            if (x > i || i - x > n) continue;

            int y = i - x;

            rst[x][y] = 1;
        }
    }

    for (int i = -n + 1; i <= n - 1; i++) {
        bool has = false;
        for (int x = 1; x <= n; x++) {
            if (x + i >= 1 && x + i <= n) {
                int y = i + x;

                if (mat[y][x] == 1) {
                    has = true;
                }
            }
        }

        if (!has) continue;

        for (int x = 1; x <= n; x++) {
            if (x + i >= 1 && x + i <= n) {
                int y = i + x;
                rst[y][x] = 1;
            }
        }
    }

    int sum = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            sum += rst[i][j];
        }
    }

    return sum;
}

int main() {
    ios_base::sync_with_stdio(false);
    cout.tie(0);
    cin.tie(0);

    srand(time(NULL));

    cout << "Started" << endl;

    for (int q = 1; q <= 16; q++) {
        for (int w = 1; 2 * w <= q + 1; w++) {
            n = q;
            k = w;

            double t = 1;
            int mx = 2e7;


            vector<pii> coords;

            for (int i = 0; i < k; i++) {
                coords.push_back({(int)rand() % n + 1, (int)rand() % n + 1});
            }

            ll res = f(coords);
            vector<pii> tmp = coords;

            ll ans = res;

            for (int i = 0; i < mx; i++) {
                t *= 0.99;

                int ttmp = (int)rand() % k;
                vector<pii> know = coords;

                know[ttmp] = {(rand() % n) + 1, (rand() % n) + 1};

                int rs = f(know);

                if (rs > res) {
                    res = rs;
                    tmp = know;
                }

                if (rs > ans || (rnd() < exp((double)(rs - ans) / t))) {
                    ans = rs;
                    coords = know;
                }
            }

            cout << res << endl;

            cout << "ans[" << n << ']' << '['<<k<<']' << '=' << '{';
            for (auto i : tmp) {
                cout << '{'<<i.first << "," << i.second<<'}' << ',';
            }

            cout << '}';

            cout << endl;
        }
    }

    return 0;
}