#include <iostream>
#include <string>
#include <unordered_set>
#include <bitset>
#include <map>
#include <random>
#include <bit>

using namespace std;

int simulate_battle(string strategy, unordered_set<string> Strategies, int q_fights);
void targeting(string strategy, bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12]);
void targeting_drunken_captain(bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12]);
void targeting_two_squads(bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12]);
void targeting_three_squads(bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12]);
void targeting_four_squads(bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12]);

mt19937& get_random_generator();
bool is_hit();

int main()
{
    // 42%, 58%, 52%, 47%
    setlocale(LC_ALL, "Russian");
    const unordered_set<string> Strategies = {
        "drunken_captain", "two_squads", "three_squads", "four_squads"
    };
    string strategy;
    int q_fights = 0;
    int result = 0;
    cout << "программа симуляции боёв" << endl;
    cout << "введите стратегию для составления статистики как в примере ниже:" << endl;
    cout << "drunken_captain" << endl;
    cout << "two_squads" << endl;
    cout << "three_squads" << endl;
    cout << "four_squads" << endl;
    while (true) {
        cin >> strategy;
        if (Strategies.count(strategy)) {
            break;
        }
        cout << endl << "стратегия введена неверно, попробуйте еще раз" << endl;
    }

    cout << endl << "введите количество боёв для статистики(рекомендуется 100 и выше)" << endl;
    while (true) {
        if ((cin >> q_fights) && (q_fights > 0) && (cin.peek() == ' ' || cin.peek() == '\n')) {
            break;
        }
        else {
            cout << endl << "Количество боёв должны быть целым неотрицательным числом, попробуйте еще раз:" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Чистим буфер
        }
    }
    result = simulate_battle(strategy, Strategies, q_fights);
    cout << endl << "средний процент побед против других стратегий: " << result << '%';
}

// можно конечно учитывать и для противоположных стратегий проценты победы и запоминать их и считать все в одной программе
// это было бы эффективно если бы стратегий было бы много(очень), но их мало и задача несложная
// поэтому можно запускать программу для каждой чтобы проверить и не писать большой код там где не надо
int simulate_battle(string strategy_a, unordered_set<string> Strategies, int q_fights)
{
    Strategies.erase(strategy_a);
    bitset<12> team_a;
    bitset<12> team_b;
    int targets_a[12]{};
    int targets_b[12]{};
    // поскольку не считаем эффективность против конкретной стратегии, можно считать все победы
    // если нужно против каждой, можно сделать вектор resize под количество вражеских стратегий
    double wins = 0;
    
    for (const auto& strategy_b : Strategies) {
        for (int i = 0; i < q_fights; i++) {
            team_a.set();
            team_b.set();

            // симуляция боя
            while (true)
            {
                // если бой окончен
                if (team_a.none() || team_b.none()) {
                    if (team_a.none() && team_b.none()) {
                        // ничья
                        wins += 0.5;
                        break;
                    }
                    if (team_b.none()) {
                        wins++;
                        break;
                    }
                    if (team_a.none()) {
                        break;
                    }
                }

                // наведение
                targeting(strategy_a, team_a, team_b, targets_a);
                targeting(strategy_b, team_b, team_a, targets_b);
                // огонь
                for (int i = 0; i < 12; i++) {
                    if (targets_a[i] != -1 && is_hit()) team_b[targets_a[i]] = 0;
                    if (targets_b[i] != -1 && is_hit()) team_a[targets_b[i]] = 0;
                }
                // подбитые пушки не стреляют
                for (int i = 0; i < 12; i++) {
                    if (!team_a[i]) targets_a[i] = -1;
                    if (!team_b[i]) targets_b[i] = -1;
                }
            }
        }
    }
    int win_percentage = static_cast<int>(std::round((wins / (q_fights * 3)) * 100));
    return win_percentage;
}

void targeting(string strategy, bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12])
{
    if (strategy == "drunken_captain") {
        targeting_drunken_captain(current_command, enemy_team, targets);
    }
    else if (strategy == "two_squads") {
        targeting_two_squads(current_command, enemy_team, targets);
    }
    else if (strategy == "three_squads") {
        targeting_three_squads(current_command, enemy_team, targets);
    }
    else if (strategy == "four_squads") {
        targeting_four_squads(current_command, enemy_team, targets);
    }
}

void targeting_drunken_captain(bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12])
{
    int enemies = enemy_team.count();
    uniform_int_distribution<int> dist(0, enemies - 1);
    for (int i = 0; i < 12; i++) {
        // Орудие целое - нужна цель
        if (current_command[i]) {
            // выбираем случайную цель
            int random_target_index = dist(get_random_generator());
            int current_enemy = 0;

            // Ищем живого врага с номером random_target_index
            for (int j = 0; j < 12; ++j) {
                if (enemy_team[j]) { // Если враг жив
                    if (current_enemy == random_target_index) {
                        targets[i] = j; // Записываем номер цели
                        break;
                    }
                    ++current_enemy;
                }
            }
        }
    }
}

void targeting_two_squads(bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12])
{
    int target_1 = -1, target_2 = -1;
    //куда стреляем?
    for (int i = 0; i < 6; i++) {
        if (current_command[i]) {
            target_1 = targets[i];
            break;
        }
    }
    for (int i = 6; i < 12; i++) {
        if (current_command[i]) {
            target_2 = targets[i];
            break;
        }
    }
    // цель одна
    if (enemy_team.count() == 1) {
        target_1 = countr_zero(enemy_team.to_ulong());
        target_2 = target_1;
    }
    // если нужно меняем цели
    if (target_1 == -1 || !enemy_team[target_1]) {
        for (int i = 0; i < 12; i++) {
            if (enemy_team[i]) {
                target_1 = i;
                break;
            }
        }
    }
    if (target_2 == -1 || !enemy_team[target_2]) {
        for (int i = 11; i >= 0; i--) {
            if (enemy_team[i]) {
                target_2 = i;
                break;
            }
        }
    }
    // наводимся
    for (int i = 0; i < 6; i++)
        if (current_command[i]) targets[i] = target_1;
    for (int i = 6; i < 12; i++)
        if (current_command[i]) targets[i] = target_2;
}

void targeting_three_squads(bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12])
{
    int target_1 = -1, target_2 = -1, target_3 = -1;

    // Определяем текущие цели для каждой группы
    for (int i = 0; i < 4; i++) {
        if (current_command[i] && target_1 == -1) {
            target_1 = targets[i];
        }
    }
    for (int i = 4; i < 8; i++) {
        if (current_command[i] && target_2 == -1) {
            target_2 = targets[i];
        }
    }
    for (int i = 8; i < 12; i++) {
        if (current_command[i] && target_3 == -1) {
            target_3 = targets[i];
        }
    }

    // Если целей меньше 3, все группы стреляют по одной цели
    if (enemy_team.count() < 3) {
        target_1 = target_2 = target_3 = countr_zero(enemy_team.to_ulong());
    }
    else {
        // Проверяем и обновляем цели для каждой группы
        if (target_1 == -1 || !enemy_team[target_1]) {
            for (int i = 0; i < 12; i++) {
                if (enemy_team[i] && i != target_2 && i != target_3) {
                    target_1 = i;
                    break;
                }
            }
        }
        if (target_2 == -1 || !enemy_team[target_2]) {
            for (int i = 11; i >= 0; i--) {
                if (enemy_team[i] && i != target_1 && i != target_3) {
                    target_2 = i;
                    break;
                }
            }
        }
        if (target_3 == -1 || !enemy_team[target_3]) {
            // Ищем цель между крайними (для разнообразия)
            for (int i = 0; i < 12; i++) {
                if (enemy_team[i] && i != target_1 && i != target_2) {
                    target_3 = i;
                    break;
                }
            }
        }
    }

    // назначаем цели
    for (int i = 0; i < 12; i++) {
        if (!current_command[i]) continue;
        // Выбираем цель в зависимости от группы (0-3, 4-7, 8-11)
        if (i < 4)       targets[i] = target_1;
        else if (i < 8)  targets[i] = target_2;
        else             targets[i] = target_3;
    }
}

void targeting_four_squads(bitset<12> current_command, bitset<12> enemy_team, int(&targets)[12])
{
    int target[4] = { -1, -1, -1, -1 };

    // Определяем текущие цели
    for (int i = 0; i < 3; i++) {
        for (int squad = 0; squad < 4; squad++) {
            int id = squad * 3 + i;
            if (current_command[id] && target[squad] == -1) {
                target[squad] = targets[id];
            }
        }
    }

    // Проверяем, живы ли текущие цели, иначе ищем новые
    for (int squad = 0; squad < 4; squad++) {
        if (target[squad] == -1 || !enemy_team[target[squad]]) {
            for (int i = 0; i < 12; i++) {
                if (enemy_team[i]) {
                    // проверка что это не цель другого взвода
                    bool is_used = false;
                    for (int other_squad = 0; other_squad < 4; other_squad++) {
                        if (other_squad != squad && target[other_squad] == i) {
                            is_used = true;
                            break;
                        }
                    }
                    if (is_used) continue;
                    target[squad] = i;
                    break;
                }
            }
            // Если не нашли уникальную, берём любую
            if (target[squad] == -1 || !enemy_team[target[squad]]) 
                target[squad] = countr_zero(enemy_team.to_ulong());
        }
    }
    // назначение целей
    for (int i = 0; i < 12; i++) {
        if (current_command[i]) {
            targets[i] = target[i / 3];
        }
    }
}

// генератор случайных чисел, static чтобы не пересоздавался и не ломал нормальное распределение
mt19937& get_random_generator() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen;
}

// Функция для проверки попадания (шанс 0.1)
bool is_hit() {
    static uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(get_random_generator()) < 0.1;
}
