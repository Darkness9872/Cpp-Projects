#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <climits>
#include <stdexcept>

using namespace std;

// определяем возможные фигуры тетриса
enum class Tetromino { I, O, T, L, J, S, Z };

// структура для представления позиции фигуры
struct Position {
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    // оператор сравнения позиций на эквивалентность
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// класс для представления фигуры с поворотами
class Piece {
public:
    vector<vector<Position>> rotations;
    char symbol = ' ';

    Piece() = default;
    Piece(const vector<Position>& cells, char sym) : symbol(sym) {
        generateRotations(cells);
    }

private:
    // генерим все повороты - 90, 180 и 270 градусов
    void generateRotations(const vector<Position>& cells) {
        vector<vector<Position>> all_rotations;
        all_rotations.push_back(cells);

        for (int i = 1; i < 4; ++i) {
            vector<Position> rotated;
            // поворот на 90
            for (const auto& cell : all_rotations.back()) {
                rotated.emplace_back(cell.y, -cell.x);
            }
            // фигуру к началу координат
            normalize(rotated);

            // проверяем на уникальность 
            bool is_unique = true;
            for (const auto& existing : all_rotations) {
                if (rotated.size() != existing.size()) continue;

                // если хоть одна точка отличается - фигура уникальна
                bool match = true;
                for (size_t j = 0; j < rotated.size(); ++j) {
                    if (!(rotated[j] == existing[j])) {
                        match = false;
                        break;
                    }
                }
                // или не уникальна
                if (match) {
                    is_unique = false;
                    break;
                }
            }
            if (is_unique) {
                all_rotations.push_back(rotated);
            }
        }

        rotations = all_rotations;
    }

    // фигуры к началу координат
    // чтобы одна фигура в разных местах не была принята за несколько разных
    void normalize(vector<Position>& cells) {
        if (cells.empty()) return;

        int min_x = cells[0].x, min_y = cells[0].y;
        for (const auto& cell : cells) {
            min_x = min(min_x, cell.x);
            min_y = min(min_y, cell.y);
        }

        for (auto& cell : cells) {
            cell.x -= min_x;
            cell.y -= min_y;
        }
        // сортируем клетки фигуры для той же цели - сравнивать с другими фигурами
        sort(cells.begin(), cells.end(), [](const Position& a, const Position& b) {
            return a.x < b.x || (a.x == b.x && a.y < b.y);
        });
    }
};

class TetrisPacker {
private:
    vector<Piece> pieces;
    vector<vector<char>> grid;
    int min_area;
    vector<vector<char>> best_grid;
    unordered_map<Tetromino, Piece> pieces_map;
    bool perfect_fit_found = false;

public:
    TetrisPacker(const vector<char>& piece_symbols) {
        initPieces();

        for (char sym : piece_symbols) {
            Tetromino t;
            switch (toupper(sym)) {
            case 'I': t = Tetromino::I; break;
            case 'O': t = Tetromino::O; break;
            case 'T': t = Tetromino::T; break;
            case 'L': t = Tetromino::L; break;
            case 'J': t = Tetromino::J; break;
            case 'S': t = Tetromino::S; break;
            case 'Z': t = Tetromino::Z; break;
            default: throw invalid_argument("Unknown piece symbol");
            }
            pieces.push_back(pieces_map.at(t));
        }

        min_area = INT_MAX;
    }

    // основной метод, который ищет идеальную упаковку
    void solve() {
        int total_cells = static_cast<int>(pieces.size()) * 4;

        // Пробуем сначала идеальные упаковки (без пустот)
        vector<pair<int, int>> possible_dimensions;

        // Генерируем возможные размеры для идеальной упаковки
        for (int h = 1; h <= total_cells; ++h) {
            if (total_cells % h == 0) {
                int w = total_cells / h;
                possible_dimensions.emplace_back(w, h);
            }
        }

        // Добавляем самые часто используемые размеры 2x(2n) и 4xn
        possible_dimensions.emplace_back(2, 2 * static_cast<int>(pieces.size()));
        possible_dimensions.emplace_back(4, static_cast<int>(pieces.size()));

        // Удаляем дубликаты
        sort(possible_dimensions.begin(), possible_dimensions.end());
        possible_dimensions.erase(unique(possible_dimensions.begin(), possible_dimensions.end()), possible_dimensions.end());

        // Сначала пробуем идеальные упаковки
        for (const auto& dim : possible_dimensions) {
            int w = dim.first;
            int h = dim.second;

            // подготовка сетки (просто заполняем пробелами)
            grid.assign(h, vector<char>(w, ' '));

            // попытка упаковки
            if (backtrack(0)) {
                if (isPerfectFit()) {
                    min_area = w * h;
                    best_grid = grid;
                    perfect_fit_found = true;
                    return;
                }
                else if (w * h < min_area) {
                    min_area = w * h;
                    best_grid = grid;
                }
            }
        }

        // Если идеальная упаковка не найдена, пробуем другие размеры
        if (!perfect_fit_found) {
            for (int h = 1; h <= total_cells; ++h) {
                for (int w = 1; w <= total_cells; ++w) {
                    if (w * h < total_cells) continue;
                    if (w * h > min_area) continue;

                    // Пропускаем уже проверенные размеры
                    bool already_checked = false;
                    for (const auto& dim : possible_dimensions) {
                        if (dim.first == w && dim.second == h) {
                            already_checked = true;
                            break;
                        }
                    }
                    if (already_checked) continue;

                    grid.assign(h, vector<char>(w, ' '));
                    if (backtrack(0)) {
                        if (w * h < min_area) {
                            min_area = w * h;
                            best_grid = grid;
                        }
                    }
                }
            }
        }
    }

    // вывод рисуем
    void printSolution() const {
        if (best_grid.empty()) {
            cout << "Невозможно уложить фигуры без пустот." << endl;
            return;
        }

        if (!perfect_fit_found) {
            cout << "Предупреждение: найдено решение с пустотами." << endl;
        }

        cout << "Минимальная площадь: " << min_area << endl;
        cout << "Размер: " << best_grid[0].size() << "x" << best_grid.size() << endl;

        cout << "+";
        for (size_t i = 0; i < best_grid[0].size(); ++i) cout << "-";
        cout << "+" << endl;

        for (const auto& row : best_grid) {
            cout << "|";
            for (char c : row) {
                cout << (c == ' ' ? '.' : c);
            }
            cout << "|" << endl;
        }

        cout << "+";
        for (size_t i = 0; i < best_grid[0].size(); ++i) cout << "-";
        cout << "+" << endl;
    }

private:
    // Рекурсивно пытается разместить все фигуры, начиная с указанной
    bool backtrack(int piece_index) {
        if (piece_index == static_cast<int>(pieces.size())) {
            return true;
        }

        const Piece& piece = pieces[piece_index];

        for (const auto& rotation : piece.rotations) {
            // все позиции
            for (int y = 0; y < static_cast<int>(grid.size()); ++y) {
                for (int x = 0; x < static_cast<int>(grid[0].size()); ++x) {
                    if (canPlace(rotation, x, y)) {
                        placePiece(rotation, x, y, piece.symbol);

                        // рекурсия на размещение оставшихся фигур
                        if (backtrack(piece_index + 1)) {
                            return true;
                        }
                        // если не вышло - откат
                        removePiece(rotation, x, y);
                    }
                }
            }
        }

        return false;
    }

    bool canPlace(const vector<Position>& cells, int x, int y) const {
        for (const auto& cell : cells) {
            int nx = x + cell.x;
            int ny = y + cell.y;

            if (nx < 0 || ny < 0 || nx >= static_cast<int>(grid[0].size()) ||
                ny >= static_cast<int>(grid.size()) || grid[ny][nx] != ' ') {
                return false;
            }
        }
        return true;
    }

    void placePiece(const vector<Position>& cells, int x, int y, char sym) {
        for (const auto& cell : cells) {
            grid[y + cell.y][x + cell.x] = sym;
        }
    }

    void removePiece(const vector<Position>& cells, int x, int y) {
        for (const auto& cell : cells) {
            grid[y + cell.y][x + cell.x] = ' ';
        }
    }

    bool isPerfectFit() const {
        for (const auto& row : grid) {
            for (char c : row) {
                if (c == ' ') return false;
            }
        }
        return true;
    }

    void initPieces() {
        pieces_map[Tetromino::I] = Piece({ {0,0}, {0,1}, {0,2}, {0,3} }, 'I');
        pieces_map[Tetromino::O] = Piece({ {0,0}, {1,0}, {0,1}, {1,1} }, 'O');
        pieces_map[Tetromino::T] = Piece({ {0,0}, {1,0}, {2,0}, {1,1} }, 'T');
        pieces_map[Tetromino::L] = Piece({ {0,0}, {0,1}, {0,2}, {1,2} }, 'L');
        pieces_map[Tetromino::J] = Piece({ {1,0}, {1,1}, {1,2}, {0,2} }, 'J');
        pieces_map[Tetromino::S] = Piece({ {1,0}, {2,0}, {0,1}, {1,1} }, 'S');
        pieces_map[Tetromino::Z] = Piece({ {0,0}, {1,0}, {1,1}, {2,1} }, 'Z');
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "Программа упаковки тетрамино" << endl;
    cout << "Введите символы тетрамино (I, O, T, L, J, S, Z) разделенные пробелами: ";

    vector<char> pieces;
    char c;
    while (cin >> c) {
        pieces.push_back(toupper(c));
        if (cin.peek() == '\n') break;
    }

    try {
        TetrisPacker packer(pieces);
        packer.solve();
        packer.printSolution();
    }
    catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    catch (const out_of_range&) {
        cerr << "Error: Invalid piece symbol" << endl;
        return 1;
    }
}