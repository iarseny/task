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

// Forward declaration
int f(vector<pii> coords);

// Функция для локального поиска (hill climbing)
int hillClimbing(vector<pii>& coords, vector<pii>& best_coords) {
    int best_val = f(coords);
    best_coords = coords;
    bool improved = true;
    
    while (improved) {
        improved = false;
        for (int i = 0; i < k; i++) {
            for (int x = 1; x <= n; x++) {
                for (int y = 1; y <= n; y++) {
                    // Пропускаем текущую позицию
                    if (coords[i].first == x && coords[i].second == y) continue;
                    
                    // Проверяем, не занята ли клетка другим ферзём
                    bool occupied = false;
                    for (int j = 0; j < k; j++) {
                        if (i != j && coords[j].first == x && coords[j].second == y) {
                            occupied = true;
                            break;
                        }
                    }
                    if (occupied) continue;
                    
                    pii old_pos = coords[i];
                    coords[i] = {x, y};
                    int val = f(coords);
                    
                    if (val > best_val) {
                        best_val = val;
                        best_coords = coords;
                        improved = true;
                    } else {
                        coords[i] = old_pos;
                    }
                }
            }
        }
    }
    coords = best_coords;
    return best_val;
}

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

            ll best_global = 0;
            vector<pii> best_global_coords;

            // Несколько запусков отжига с разными параметрами
            int num_runs = (n <= 8) ? 5 : 3;
            
            for (int run = 0; run < num_runs; run++) {
                // Инициализация: хорошее начальное размещение
                vector<pii> coords;
                if (run == 0) {
                    // Первый запуск: диагональное размещение
                    for (int i = 0; i < k; i++) {
                        coords.push_back({i + 1, i + 1});
                    }
                } else {
                    // Остальные: случайное размещение без повторений
                    for (int i = 0; i < k; i++) {
                        int x, y;
                        bool valid;
                        do {
                            valid = true;
                            x = (rand() % n) + 1;
                            y = (rand() % n) + 1;
                            for (auto& c : coords) {
                                if (c.first == x && c.second == y) {
                                    valid = false;
                                    break;
                                }
                            }
                        } while (!valid);
                        coords.push_back({x, y});
                    }
                }

                ll curr_res = f(coords);
                vector<pii> best_coords = coords;
                ll best_res = curr_res;

                // Параметры отжига
                double t = (run == 0) ? 10.0 : (5.0 + run * 2.0);
                int mx = 4e7 / num_runs;
                double cooling = 0.99995;

                for (int i = 0; i < mx; i++) {
                    t *= cooling;
                    if (t < 1e-6) t = 1e-6;  // Минимальная температура

                    // Выбираем случайного ферзя
                    int idx = (int)rand() % k;
                    vector<pii> new_coords = coords;

                    // Генерируем новую позицию, гарантируя её уникальность
                    int new_x, new_y;
                    int attempts = 0;
                    do {
                        new_x = (rand() % n) + 1;
                        new_y = (rand() % n) + 1;
                        attempts++;
                    } while (attempts < 10 && new_x == coords[idx].first && new_y == coords[idx].second);

                    // Проверяем уникальность (избегаем совпадений с другими ферзями)
                    bool valid = true;
                    for (int j = 0; j < k; j++) {
                        if (j != idx && coords[j].first == new_x && coords[j].second == new_y) {
                            valid = false;
                            break;
                        }
                    }
                    
                    if (!valid) continue;

                    new_coords[idx] = {new_x, new_y};
                    ll new_res = f(new_coords);

                    // Принимаем движение, если оно лучше или с вероятностью exp(delta/T)
                    if (new_res > curr_res || (t > 1e-7 && rnd() < exp((double)(new_res - curr_res) / t))) {
                        coords = new_coords;
                        curr_res = new_res;
                    }

                    if (new_res > best_res) {
                        best_res = new_res;
                        best_coords = new_coords;
                    }
                }

                // Локальный поиск в конце
                vector<pii> hc_coords;
                int hc_res = hillClimbing(coords, hc_coords);
                
                if (hc_res > best_res) {
                    best_res = hc_res;
                    best_coords = hc_coords;
                }

                // Обновляем глобальный лучший результат
                if (best_res > best_global) {
                    best_global = best_res;
                    best_global_coords = best_coords;
                }
            }

            cout << best_global << endl;
            cout << "ans[" << n << "][" << k << "]={";
            for (auto i : best_global_coords) {
                cout << '{' << i.first << "," << i.second << "},";
            }
            cout << "}" << endl;
        }
    }

    return 0;
}