#pragma once
#ifndef LR_TG_FUNCTIONS
#define LR_TG_FUNCTIONS

#include "Graph.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <limits>
#include <cmath>
#include <chrono>

using namespace std::chrono;

struct Edge {
	int source;
	int destination;
	int weight;
};

struct Subset {
	int parent, rank;
};
vector<Edge> kruskal(vector<int> matrix, int quantity);
vector<Edge> prim(vector<int> matrix, int quantity);
vector<Edge> boruvka(vector<int> matrix, int quantity);
bool compareEdges(Edge a, Edge b);
void Union(Subset subsets[], int x, int y); 
int find(Subset subsets[], int i);
void result_cout(vector<Edge> result);

// основные функции //
Graph::Graph(bool key_lab8)
{
	if (key_lab8 == 1) { read_matrix(); return; }
	read_data();
}
int Graph::weight(int vi, int vj)
{
	int k = (vi - 1) * quantity + vj - 1;
	return matrix[k];
}
bool Graph::is_edge(int vi, int vj)
{
	int k = (vi - 1) * quantity + vj - 1;
	if (matrix[k]) return true;
	return false;
}
void Graph::adjacency_matrix()
{
	int y = quantity * quantity;
	if (key_file_output) goto out_in_file;
	cout << endl;
	for (int i = 0, k = 1; i < y; i++, k++) {
		cout << matrix[i] << ' ';
		if (k == quantity) {
			k = 0;
			cout << endl;
		}
	}
	return;
out_in_file:
	ofstream file(file_output_address);
	file << endl;
	for (int i = 0, k = 1; i < y; i++, k++) {
		file << matrix[i] << ' ';
		if (k == quantity) {
			k = 0;
			file << endl;
		}
	}
	file.close();
	return;
}
void Graph::adjacency_list(int v)
{
	if (key_file_output) goto fo;
	// вывод в консоль
	cout << endl;
	for (int i = (v - 1) * quantity + 1; i <= ((v - 1) * quantity + quantity); i++)
	{
		if (matrix[i]) cout << i - (v - 1) * quantity << ' ';
	}
	cout << endl;
	return;
	// вывод в файл
fo:
	ofstream file(file_output_address);
	file << endl;
	for (int i = (v - 1) * quantity + 1; i <= ((v - 1) * quantity + quantity); i++)
	{
		if (matrix[i]) file << i - (v - 1) * quantity << ' ';
	}
	file << endl;
	file.close();
	return;
}
void Graph::list_of_edges()
{
	int y = quantity * quantity;
	if (key_file_output) goto fo1;
	// вывод в консоль
	cout << endl;
	for (int i = 0; i < y; i++)
	{
		if (matrix[i])
		{
			cout << i / quantity + 1 << ' ' << i % quantity + 1 << endl;
		}
	}
	cout << endl;
	return;
	// вывод в файл
fo1:
	ofstream file(file_output_address);
	file << endl;
	for (int i = 0; i < y; i++)
	{
		if (matrix[i])
		{
			file << i / quantity + 1 << ' ' << i % quantity + 1 << endl;
		}
	}
	return;
}
void Graph::list_of_edges(int v)
{
	int k = ((v - 1) * quantity + quantity);
	if (key_file_output) goto fo2;
	// вывод в консоль
	cout << endl;
	for (int i = (v - 1) * quantity; i < k; i++)
	{
		if (matrix[i]) cout << v << ' ' << i - (v - 1) * quantity + 1 << endl;
	}
	cout << endl;
	return;
	// вывод в файл
fo2:
	ofstream file(file_output_address);
	file << endl;
	for (int i = (v - 1) * quantity; i < k; i++)
	{
		if (matrix[i]) file << v << ' ' << i - (v - 1) * quantity + 1 << endl;
	}
	file << endl;
	file.close();
	return;
}
bool Graph::is_directed()
{
	return key_orientation;
}

// функция для вывода //
/* проверяет куда вывод и если в файл то проверяет возможность вывода,
меняет ключи*/
void Graph::info_output()
{
sio:
	// file_output_address используется здесь как input
	cout << endl;
	cout << "Пожалуйста, выберете способ вывода данных: " << endl;
	cout << "1. консоль" << endl;
	cout << "2. в файл" << endl;
	cin >> file_output_address;
	if (file_output_address == "1" || file_output_address == "консоль") {
		cout << "Вывод будет производиться в консоль" << endl;
		return;
	}
	if (file_output_address == "2" || file_output_address == "в файл") {
		goto file_output_info;
	}
	cout << endl << "Ввод является неккоректным:";
	cout << endl << "необходимо ввести либо номер пункта, либо фразу пункта с точностью до буквы(без цифры и точки)";
	system("pause");
	system("CLS");
	goto sio;

file_output_info:
	ofstream file;
	cout << endl << "нужно ввести адрес файла, в который будут выводиться данные";
	cout << endl << "если файл находится в папке, находящейся в папке с программой, то адрес выглядит так:";
	cout << endl << "docs\\file.txt";
	cout << endl << "если файл находится где-либо еще, то необходимо писать полный путь";
	cout << endl << "пример: D:\\docs\\file.txt";
	cout << endl << "как правило, файлы с русскими буквами в пути не открываются";
	cout << endl << "введите адрес файла(если нужно выйти из программы введите 1): ";
	cin >> file_output_address;

	// выход из программы
	if (file_output_address == "1") {
		exit(0);
	}
	file.open(file_output_address);
	// если неудача
	if (!file || !file.is_open()) {
		if (!file.is_open()) cout << "файл не открыт" << endl;
		if (!file) cout << "ошибка открытия файла" << endl;
		file.close();
		cout << "попробуйте снова" << endl;
		system("pause");
		system("CLS");
		goto file_output_info;
	}
	file.close();
	cout << endl <<"файл для вывода успешно открыт" << endl;
	return;
}

// функции ввода //

// направления на ввод графа в зависимости от ключа
void Graph::read_data()
{
	string input;
start_read:
	cout << endl << "введите ключ:" << endl;
	cout << "- e edges_list_file_path" << endl
		 << "- m adjacency_matrix_file_path" << endl
		 << "- l adjacency_list_file_path" << endl
		 << "- exit(выход)" << endl;
	cin >> input;
	if (input == "e") {
		read_edges();
		cout << "граф заполнен из файла со списком ребер" << endl;
		return;
	}
	if (input == "m") {
		read_matrix();
		cout << "граф заполнен из файла с матрицей" << endl;
		return;
	}
	if (input == "l") {
		read_adjacency();
		cout << "граф заполнен из файла со списками смежности" << endl;
		return;
	}
	if (input == "exit") {
		exit(0);
		return;
	}
	cout << "ключ неккоректен, попробуйте снова";
	system("pause");
	system("CLS");
	cout << "подсказка: нужно ввести одну английскую букву: e, m или l (или exit)";
	goto start_read;
}
// ввод графа в виде матрицы смежности
void Graph::read_matrix()
{
	string input = file_info_input();
	ifstream file(input);
	int input_1 = 0;

	// непосредственно ввод данных
	while (!file.eof()) {
		quantity++;
		file >> input_1;
		matrix.push_back(input_1);
	}
	file.close();
	quantity = sqrt(quantity);
	check_orientation();
	return;
}
// ввод графа в виде списка рёбер
void Graph::read_edges()
{
	// работа с файлом
	string input = file_info_input();
	ifstream file(input);

	// указан ли у ребер вес
	char check[50]{};
	file.getline(check, 50);
	bool key_weight = check_edges(check);

	// поиск ребра с самым большим номером
	int m = 0;
	file.close();
	file.open(input);
	while (!file.eof())
	{
		// если ребра без веса
		if (key_weight == 0)
		{
			file >> m;
			if (m > quantity) quantity = m;
			file >> m;
			if (m > quantity) quantity = m;

		}
		// если с весом
		else
		{
			file >> m;
			if (m > quantity) quantity = m;
			file >> m;
			if (m > quantity) quantity = m;
			file >> m;
		}
	}
	// создание матрицы смежности нужного размера
	m = quantity ^ 2;
	for (int i = 0; i < m; i++) {
		matrix.push_back(0);
	}

	// заполнение
	file.close();
	file.open(input);
	int n = 0;
	int position = 0;
	while (!file.eof())
	{
		// если ребра без веса
		if (key_weight == 0)
		{
			file >> m;
			file >> n;
			position = (m - 1) * quantity + n - 1;
			matrix[position] = 1;
		}
		// если с весом
		else
		{
			file >> m;
			file >> n;
			position = (m - 1) * quantity + n - 1;
			file >> matrix[position];
		}
	}

	check_orientation();
	file.close();
	return;
};
// ввод графа в виде списка смежных вершин
void Graph::read_adjacency()
{
	string input = file_info_input();
	ifstream file(input);

	// расчет количества вершин
	vector <int> q_v;
	char* str = new char[1024];
	cout << endl;
	while (!file.eof()) {
		memset(str, 0, 1024);
		file.getline(str, 1024, '\n');
		q_v.push_back(0);
		// расчет количества элементов в строке
		for (int i = 0; i < 1024; i++) {
			if (str[i] == ' ' && isdigit(str[i-1])) 
				q_v[q_v.size() - 1]++;
		}
		quantity++;
	}
	delete[] str;
	quantity--;

	// создание матрицы смежности нужного размера
	int position = 0;
	position = quantity * quantity;
	for (int i = 0; i < position; i++) {
		this->matrix.push_back(0);
	}

	file.close();
	file.open(input);

	// запись данных
	int k = 0;
	while (k < quantity) {
		// заполнение одной вершины
		for (int i = 0; i < q_v[k]; i++) {
			file >> position;
			position = k * quantity + position - 1;
			matrix[position] = 1;
		}
		k++;
	}
	file.close();
	check_orientation();
	return;
}

//									//
/*		  функции к лабам			*/
//									//
void Graph::lab_1() 
{
	// расчет вектора степеней вершин и вывод в консоль/файл
	cout << endl << "расчет вектора степеней вершин" << endl;
	int k = 0;
	int position = 0;
	ofstream file(file_output_address);
	// вывод сообщения о текущем шаге
	if (key_file_output) {
		file << endl;
		file << "Вектор степеней вершин: ";
	}
	else {
		cout << endl;
		cout << "Вектор степеней вершин: ";
	}
	for (int i = 0; i < quantity; i++)
	{
		// расчет степени текущей вершины
		for (int j = 0; j < quantity; j++)
		{
			position = i * quantity + j;
			if (matrix[position]) k++;
		}
		// вывод степени текущей вершины
		if (key_file_output) {
			file << k << ' ';
		}
		else {
			cout << k << ' ';
		}
		k = 0;
	}

	// отступ в выводе
	if (key_file_output) {
		file << endl;
	}
	else {
		cout << endl;
	}

	// замена отсутствия пути на большие числа
	for (int i = 0; i < quantity; i++) {
		for (int j = 0; j < quantity; j++) {
			position = i * quantity + j;
			if (!matrix[position] && i != j) {
				matrix[position] = 2000000;
			}
		}
	}
	position = 0;

	// алгоритм Флойда-Уоршелла
	cout << endl << "выполняется алгоритм Флойда-Уоршелла" << endl;
	k = 0; position = 0;
	int n = 0;
	for (int l = 0; l < quantity; l++) {
		for (int i = 0; i < quantity; i++) {
			for (int j = 0; j < quantity; j++) {
				// matrix[k] = matrix[i][j]
				// matrix[position] = matrix[i][l]
				// matrix[n] = matrix[l][j]
				k = i * quantity + j;
				position = i * quantity + l;
				n = l * quantity + j;
				// если обходного пути нет или поиск петли
				if (matrix[position] != 0 && matrix[n] != 0 && i != j) {
					//matrix[i][j] = min(matrix[i][j], matrix[i][l] + matrix[l][j]);
					//Новое значение ребра равно минимальному между старым
					//и суммой ребер i <-> l + l <-> j (если через k пройти быстрее)
					if (matrix[k] > (matrix[position] + matrix[n])) 
						matrix[k] = matrix[position] + matrix[n];
				}
			}
		}
	}

	// поиск радиуса и диаметра
	cout << endl << "выполняется поиск радиуса и диаметра" << endl;
	if (key_file_output) {
		file << "exccentricity: " << endl;
	}
	else {
		cout << "exccentricity: " << endl;
	}
	int radius = 2147483647, diametr = 0;
	int exc = 0;
	n = 0;
	k = 0;
	for (int i = 0; i < quantity; i++) {
		for (int j = 0; j < quantity; j++) {
			position = i * quantity + j;
			if (matrix[position] > exc) exc = matrix[position];
		}
		if (exc < radius) radius = exc;
		if (exc > diametr) diametr = exc;
		// вывод exc
		if (key_file_output) {
			file << exc << ' ';
		}
		else {
			cout << exc << ' ';
		}
		exc = 0;
	}

	// вывод радиуса и диаметра
	if (key_file_output) {
		file << endl;
		file << "радиус = " << radius << "; диаметр = " << diametr;
		file << endl;
	}
	else {
		cout << endl;
		cout << "радиус = " << radius << "; диаметр = " << diametr;
		cout << endl;
	}

	// если граф ориентированный, множества вершин ему не нужны
	if (key_orientation) {
		adjacency_matrix();
		file.close();
		cout << endl << "выполнение первой лабораторной работы успешно завершено" << endl;
		return;
	}

	// поиск центральных и перифирийных вершин
	cout << endl << "выполняется поиск центральных и перифирийных вершин" << endl;
	vector <int> centr_vertex;
	vector <int> perif_vertex;
	for (int i = 0; i < quantity; i++) {
		for (int j = 0; j < quantity; j++) {
			position = i * quantity + j;
			if (matrix[position] > exc) exc = matrix[position];
		}
		k = i + 1;
		if (exc == radius) centr_vertex.push_back(i+1);
		if (exc == diametr) perif_vertex.push_back(i+1);
		exc = 0;
	}
	
	// вывод множеств
	if (key_file_output) {
		file << endl;
		file << "множество централных вершин: ";
		for (int i = 0; i < centr_vertex.size(); i++) {
			file << centr_vertex[i] << ' ';
		}
		file << endl;
		file << "множество периферийных вершин: ";
		for (int i = 0; i < perif_vertex.size(); i++) {
			file << perif_vertex[i] << ' ';
		}
		file << endl;
	}
	else {
		cout << endl;
		cout << "множество централных вершин: ";
		for (int i = 0; i < centr_vertex.size(); i++) {
			cout << centr_vertex[i] << ' ';
		}
		cout << endl;
		cout << "множество периферийных вершин: ";
		for (int i = 0; i < perif_vertex.size(); i++) {
			cout << perif_vertex[i] << ' ';
		}
		cout << endl;
	}

	// замена отсутствия пути на нули
	for (int i = 0; i < quantity; i++) {
		for (int j = 0; j < quantity; j++) {
			position = i * quantity + j;
			if (matrix[position] == 2000000 && i != j) {
				matrix[position] = 0;
			}
		}
	}
	adjacency_matrix();

	file.close();
	cout << endl << "выполнение первой лабораторной работы успешно завершено" << endl;
	return;
}
void Graph::lab_2()
{
	ofstream file(file_output_address);

	// список посещенных вершин
	vector <bool> mas;
	mas.assign(quantity, 0);
	// состав текущей компоненты связности
	vector <int> component_now;
	component_now.assign(quantity, 0);
	// состав текущей компоненты сильной связности
	vector <int> component_now_1;
	component_now_1.assign(quantity, 0);
	// вектор компонент связности
	vector <vector <int>> components;
	// очередь для поиска в ширину
	vector <int> queue;
	int k = 0;
	int position = 0;
	int position_2 = 0;
	bool end = 0;
	int n = 0;

	// если граф ориентированный
	if (key_orientation) {
		
		// вывод информации
		cout << endl << "Расчет ориентированного графа на сильную связность" << endl;
		if (key_file_output) {
			file << endl << "Граф является ориентированным" << endl;
		}
		else {
			cout << endl << "Граф является ориентированным" << endl;
		}

		// поиск количества компонент сильной связности и их состав
		while (1) {
			//цикл на поиск непосещенной вершины
			k = -1;
			for (int i = 0; i < mas.size(); i++) {
				if (mas[i] == 0) {
					k = i;
					break;
				}
			}
			// если все вершины посещены
			if (k == -1) break;

			// поиск вершин, которые могут относиться к данной компоненте
			queue.push_back(k);
			component_now[k] = 1;
			while (queue.size() != 0) {
				// n - текущая вершина
				n = queue[0];
				queue.erase(queue.begin());

				// прогон вершины из очереди
				for (int j = 0; j < quantity; j++) {
					position = n * quantity + j;
					// если есть путь в вершину и она не посещена
					if (matrix[position] && mas[j] != 1 && component_now[j] != 1) {
						// добавляем в очередь
						queue.push_back(j);
						component_now[j] = 1;
					}
				}
			}

			// обратный поиск по найденной компоненте
			queue.push_back(k);
			component_now_1[k] = 1;
			while (queue.size() != 0) {
				// n - текущая вершина
				n = queue[0];
				queue.erase(queue.begin());

				// прогон вершины из очереди
				for (int j = 0; j < quantity; j++) {
					position = j * quantity + n;
					// если есть путь в вершину и она не посещена и есть в предпологаемой компоненте
					if (matrix[position] && component_now_1[j] != 1 && component_now[j] == 1) {
						// добавляем в очередь
						queue.push_back(j);
						component_now_1[j] = 1;
					}
				}
			}

			// оценка полученного результата и соответствующие изменения данных
			if (component_now_1.size() > 1) {
				// если найдена компонента связности
				components.push_back(component_now_1); // запоминаем её
				// помечаем как посещенные все вершины данной компоненты сильной связности
				for (int i = 0; i < quantity; i++) {
					if (component_now_1[i]) mas[i] = 1;
				}
			}
			else {
				mas[k] = 1;
			}

			// очищаем переменные
			for (int i = 0; i < quantity; i++) {
				component_now[i] = 0;
				component_now_1[i] = 0;
			}
		}

		// вывод информации
		cout << endl << "Поиск компонент сильной связности завершен" << endl;
		if (key_file_output) {
			// вывод в файл
			if (components.size()) {
				file << endl << "В графе " << components.size() << " компонент сильной связности" << endl;
				file << "вывод состава компонент сильной связности: " << endl;
				for (int i = 0; i < components.size(); i++) {
					file << i << ": ";
					for (int j = 0; j < quantity; j++) {
						if (components[i][j]) file << j + 1 << ' ';
					}
					file << endl;
				}
			}
			else {
				file << endl << "в этом графе нет компонент сильной связности" << endl;
			}

		}
		else {
			// вывод в консоль
			if (components.size()) {
				cout << endl << "В графе " << components.size() << " компонент сильной связности" << endl;
				cout << "вывод состава компонент сильной связности: " << endl;
				for (int i = 0; i < components.size(); i++) {
					cout << i << ": ";
					for (int j = 0; j < quantity; j++) {
						if (components[i][j]) cout << j + 1 << ' ';
					}
					cout << endl;
				}
			}
			else {
				cout << endl << "в этом графе нет компонент сильной связности" << endl;
			}

		}

		// зануление массива посещения вершин
		for (int j = 0; j < quantity; j++) {
			mas[j] = 0;
		}
		queue.clear();
		k = 0;
		components.clear();

		cout << "Начало поиска компонент слабой связности" << endl;

		// поиск количества компонент слабой связности и их состав
		while (1) {
			//цикл на поиск непосещенной вершины
			k = -1;
			for (int i = 0; i < mas.size(); i++) {
				if (mas[i] == 0) {
					k = i;
					break;
				}
			}
			// если все вершины посещены
			if (k == -1) break;

			// поиск вершин, которые могут относиться к данной компоненте
			queue.push_back(k);
			component_now[k] = 1;
			while (queue.size() != 0) {
				// n - текущая вершина
				n = queue[0];
				queue.erase(queue.begin());

				// прогон вершины из очереди
				for (int j = 0; j < quantity; j++) {
					position = n * quantity + j;
					position_2 = j * quantity + n;
					// если есть путь в вершину и она не посещена
					if ((matrix[position] || matrix[position_2]) && mas[j] != 1 && component_now[j] != 1) {
						// добавляем в очередь
						queue.push_back(j);
						component_now[j] = 1;
					}
				}
			}

			// оценка полученного результата и соответствующие изменения данных
			if (component_now.size() > 1) {
				// если найдена компонента связности
				components.push_back(component_now); // запоминаем её
				// помечаем как посещенные все вершины данной компоненты сильной связности
				for (int i = 0; i < quantity; i++) {
					if (component_now[i]) mas[i] = 1;
				}
			}
			else {
				mas[k] = 1;
			}

			// очищаем переменные
			for (int i = 0; i < quantity; i++) {
				component_now[i] = 0;
			}
		}

		// вывод информации
		cout << endl << "Поиск компонент слабой связности завершен" << endl;
		if (key_file_output) {
			// вывод в файл
			if (components.size()) {
				file << endl << "В графе " << components.size() << " компонент слабой связности" << endl;
				file << "вывод состава компонент слабой связности: " << endl;
				for (int i = 0; i < components.size(); i++) {
					file << i << ": ";
					for (int j = 0; j < quantity; j++) {
						if (components[i][j]) file << j + 1 << ' ';
					}
					file << endl;
				}
			}
			else {
				file << endl << "в этом графе нет компонент слабой связности (т.е. граф несвязный)" << endl;
			}

		}
		else {
			// вывод в консоль
			if (components.size()) {
				cout << endl << "В графе " << components.size() << " компонент слабой связности" << endl;
				cout << "вывод состава компонент слабой связности: " << endl;
				for (int i = 0; i < components.size(); i++) {
					cout << i << ": ";
					for (int j = 0; j < quantity; j++) {
						if (components[i][j]) cout << j + 1 << ' ';
					}
					cout << endl;
				}
			}
			else {
				cout << endl << "в этом графе нет компонент слабой связности (т.е. граф несвязный)" << endl;
			}

		}

	}
	// если граф неориентированный
	else {
		if (key_file_output) {
			file << endl << "Граф не является ориентированным" << endl;
		}
		else {
			cout << endl << "Граф не является ориентированным" << endl;
		}

		cout << "Начало поиска компонент связности" << endl;
		// поиск количества компонент связности и их состав
		while (1) {
			//цикл на поиск непосещенной вершины
			k = -1;
			for (int i = 0; i < mas.size(); i++) {
				if (mas[i] == 0) {
					k = i;
					break;
				}
			}
			// если все вершины посещены
			if (k == -1) break;

			// поиск вершин, которые могут относиться к данной компоненте
			queue.push_back(k);
			component_now[k] = 1;
			while (queue.size() != 0) {
				// n - текущая вершина
				n = queue[0];
				queue.erase(queue.begin());

				// прогон вершины из очереди
				for (int j = 0; j < quantity; j++) {
					position = n * quantity + j;
					// если есть путь в вершину и она не посещена
					if (matrix[position] && mas[j] != 1 && component_now[j] != 1) {
						// добавляем в очередь
						queue.push_back(j);
						component_now[j] = 1;
					}
				}
			}

			// оценка полученного результата и соответствующие изменения данных
			if (component_now.size() > 1) {
				// если найдена компонента связности
				components.push_back(component_now); // запоминаем её
				// помечаем как посещенные все вершины данной компоненты сильной связности
				for (int i = 0; i < quantity; i++) {
					if (component_now[i]) mas[i] = 1;
				}
			}
			else {
				mas[k] = 1;
			}

			// очищаем переменные
			for (int i = 0; i < quantity; i++) {
				component_now[i] = 0;
			}
		}

		// вывод информации
		cout << endl << "Поиск компонент связности завершен" << endl;
		if (key_file_output) {
			// вывод в файл
			if (components.size()) {
				file << endl << "В графе " << components.size() << " компонент связности" << endl;
				file << "вывод состава компонент связности: " << endl;
				for (int i = 0; i < components.size(); i++) {
					file << i << ": ";
					for (int j = 0; j < quantity; j++) {
						if (components[i][j]) file << j + 1 << ' ';
					}
					file << endl;
				}
			}
			else {
				file << endl << "в этом графе нет компонент связности (т.е. граф несвязный)" << endl;
			}

		}
		else {
			// вывод в консоль
			if (components.size()) {
				cout << endl << "В графе " << components.size() << " компонент связности" << endl;
				cout << "вывод состава компонент связности: " << endl;
				for (int i = 0; i < components.size(); i++) {
					cout << i << ": ";
					for (int j = 0; j < quantity; j++) {
						if (components[i][j]) cout << j + 1 << ' ';
					}
					cout << endl;
				}
			}
			else {
				cout << endl << "в этом графе нет компонент связности (т.е. граф несвязный)" << endl;
			}

		}
	}
}
void Graph::lab_3()
{
	ofstream file(file_output_address);
	//vector<bool> visited;   // Массив для отслеживания посещенных вершин
	//vector<int> tin, fup;   // Массивы времен входа и минимума обратного времени
	visited.assign(quantity, false);
	tin.assign(quantity, -1);
	fup.assign(quantity, -1);
	if (key_orientation) do_no_orient();

	cout << endl << "Идет расчет шарниров и  мостов" << endl;
	for (int i = 0; i < quantity; ++i) {
		if (!visited[i]) {
			dfs(i);
		}
	}
	sort (vertexs.begin(), vertexs.end());

	// вывод информации
	cout << endl << "Расчет завершен" << endl;
	if (key_file_output) {
		// вывод мостов
		if (bridges.size() != 0) {
			file << endl << "Мосты в графе: ";
			for (int i = 0; i < bridges.size(); i++) {
				file << bridges[i].v1 + 1 << " - " << bridges[i].v2 + 1 << "; ";
			}
		}
		else file << endl << "в графе нет мостов" << endl;
		// вывод шарниров
		if (vertexs.size() != 0) {
			file << endl << "Шарниры в графе: ";
			for (int i = 0; i < vertexs.size(); i++) {
				file << vertexs[i] + 1 << "; ";
			}
		}
		else file << endl << "в графе нет шарниров" << endl;
	}
	else {
		// вывод мостов
		if (bridges.size() != 0) {
			cout << endl << "Мосты в графе: ";
			for (int i = 0; i < bridges.size(); i++) {
				cout << bridges[i].v1 + 1 << " - " << bridges[i].v2 + 1 << "; ";
			}
		}
		else cout << endl << "в графе нет мостов" << endl;
		// вывод шарниров
		if (vertexs.size() != 0) {
			cout << endl << "Шарниры в графе: ";
			for (int i = 0; i < vertexs.size(); i++) {
				cout << vertexs[i] + 1 << "; ";
			}
		}
		else cout << endl << "в графе нет шарниров" << endl;
	}
}
void Graph::lab_4()
{
	vector<bool> visited(quantity, false);
	vector<int> minWeight(quantity, INT_MAX);
	vector<int> parent(quantity, -1);
	int position = 0;
	if (key_orientation) do_no_orient();

	for (int _ = 0; _ < quantity; ++_) {
		int minVertex = -1;
		for (int v = 0; v < quantity; ++v) {
			if (!visited[v] && (minVertex == -1 || minWeight[v] < minWeight[minVertex])) {
				minVertex = v;
			}
		}

		visited[minVertex] = true;

		for (int neighbor = 0; neighbor < quantity; ++neighbor) {
			position = minVertex * quantity + neighbor;
			int weight = matrix[position];

			if (weight > 0 && weight < minWeight[neighbor]) {
				parent[neighbor] = minVertex;
				minWeight[neighbor] = weight;
			}
		}
	}

	//vector<Edge> treeEdges;
	int totalWeight = 0;
	bridge one;
	for (int v = 1; v < quantity; ++v) {
		one.v1 = parent[v]; one.v2 = v;
		bridges.push_back(one);
		position = v * quantity + parent[v];
		totalWeight += matrix[position];
	}

	cout << "Spanning Tree:" << endl;
	for (int i = 0; i < bridges.size(); i++) {
		position = bridges[i].v1 * quantity + bridges[i].v2;
		cout << "(" << bridges[i].v1 + 1 << ", " << bridges[i].v2 + 1 << ")" << " weight = " << matrix[position] << endl;
	}
	cout << "Total Weight: " << totalWeight << endl;
}
void Graph::lab_5()
{
	int begin = begin_vertex_number();
	int end = end_vertex_number();
	std::vector<int> distances(quantity, std::numeric_limits<int>::max());
	std::vector<int> previous_vertices(quantity, -1);

	//struct Edge {
	//	int destination;
	//	int weight;
	//};

	priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

	// Инициализация начальной вершины
	distances[begin] = 0;
	pq.push(std::make_pair(0, begin));

	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();

		for (int v = 0; v < quantity; ++v) {
			int position = u * quantity + v;
			int weight = matrix[position];

			// Если ребро между вершинами существует и найден более короткий путь до вершины v
			if (weight > 0 && distances[u] + weight < distances[v]) {
				distances[v] = distances[u] + weight;
				previous_vertices[v] = u;
				pq.push(std::make_pair(distances[v], v));
			}
		}
	}

	// Восстановление пути от конечной вершины до начальной
	std::vector<int> path;
	int current_vertex = end;
	while (current_vertex != -1) {
		path.push_back(current_vertex);
		current_vertex = previous_vertices[current_vertex];
	}
	std::reverse(path.begin(), path.end());

	if(path.size() == 1 || path.size() == 0) {
		cout << endl << "Пути нет" << endl;
		return;
	}
	cout << endl << "Геодезическая цепь: " << endl;
	for (int i = 0; i < path.size(); i++) {
		cout << path[i] + 1 << ' ';
	}
	cout << endl << "длина пути: " << path.size() - 1;
}
void Graph::lab_6()
{
	//struct Edge {
	//	int source;
	//	int destination;
	//	int weight;
	//};
	vector<pair<int, int>> distances;
	vector<Edge> edges;
	// Создание списка ребер на основе матрицы смежности
	for (int i = 0; i < quantity; ++i) {
		for (int j = 0; j < quantity; ++j) {
			int position = i * quantity + j;
			int weight = matrix[position];
			if (weight != 0) {
				edges.push_back({ i, j, weight });
			}
		}
	}
	char algorithm = algorithm_input_6lab();

	// Проверка наличия ребер между вершинами для алгоритмов Беллмана-Форда-Мура и Левита
	if (algorithm == 'b' || algorithm == 't') {
		for (const auto& edge : edges) {
			if (edge.weight < 0) {
				cout << "Ошибка: граф содержит отрицательные ребра, алгоритм Беллмана-Форда-Мура и алгоритм Левита не могут быть применены." << endl;
				return;
			}
		}
	}

	int startVertex = begin_vertex_number();

	// Инициализация расстояний до всех вершин плюс бесконечность
	const int INF = INT_MAX / 2;
	distances.resize(quantity, { startVertex, INF });
	distances[startVertex] = { startVertex, 0 };

	// Запуск выбранного алгоритма
	if (algorithm == 'd') {
		// Алгоритм Дейкстры
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
		pq.push({ 0, startVertex });

		while (!pq.empty()) {
			int u = pq.top().second;
			int uDistance = pq.top().first;
			pq.pop();

			// Пропускаем вершины, расстояние до которых уже меньше текущего расстояния
			if (uDistance > distances[u].second) {
				continue;
			}

			for (const auto& edge : edges) {
				if (edge.source == u) {
					int v = edge.destination;
					int weight = edge.weight;

					if (uDistance + weight < distances[v].second) {
						distances[v] = { u, uDistance + weight };
						pq.push({ uDistance + weight, v });
					}
				}
			}
		}
	}
	else if (algorithm == 'b') {
		// Алгоритм Беллмана-Форда-Мура
		for (int i = 1; i < quantity; ++i) {
			for (const auto& edge : edges) {
				int u = edge.source;
				int v = edge.destination;
				int weight = edge.weight;

				if (distances[u].second != INF && distances[u].second + weight < distances[v].second) {
					distances[v] = { u, distances[u].second + weight };
				}
			}
		}
	}
	else if (algorithm == 't') {
		// Алгоритм Левита
		queue<int> q;
		vector<int> processed(quantity, 0);
		vector<int> distanceChanges(quantity, 0);

		q.push(startVertex);
		processed[startVertex] = 1;
		distanceChanges[startVertex] = 0;

		while (!q.empty()) {
			int u = q.front();
			q.pop();
			processed[u] = 0;

			for (const auto& edge : edges) {
				int v = edge.destination;
				int weight = edge.weight;

				if (distances[u].second + weight < distances[v].second) {
					distances[v] = { u, distances[u].second + weight };

					if (!processed[v]) {
						q.push(v);
						processed[v] = 1;
						distanceChanges[v]++;

						// Проверка наличия отрицательного цикла
						if (distanceChanges[v] >= quantity) {
							cout << "Ошибка: граф содержит отрицательный цикл, алгоритм Левита не может быть применен." << endl;
							return;
						}
					}
				}
			}
		}
	}
	cout << endl << "расстояния между вершинами: " << endl;
	for (int i = 0; i < distances.size(); i++) {
		cout << startVertex + 1 << " - ";
		if (distances[i].second == 1073741823) cout << "no" << endl;
		else cout << distances[i].second << endl;
	}
}
void Graph::lab_7()
{
	std::vector<int> h(quantity + 1, 0);
	std::vector<int> distance(quantity * quantity, std::numeric_limits<int>::max() / 2);

	// Расчет потенциалов с помощью алгоритма Беллмана - Форда
	for (int k = 1; k <= quantity; ++k) {
		for (int i = 1; i <= quantity; ++i) {
			for (int j = 1; j <= quantity; ++j) {
				int weight = matrix[(i - 1) * quantity + j - 1];
				if (weight != 0) {
					distance[(i - 1) * quantity + j - 1] = weight + h[i] - h[j];
				}
			}
		}
	}

	// Расчет кратчайших путей с помощью алгоритма Дейкстры
	for (int i = 1; i <= quantity; ++i) {
		std::vector<int> distanceFromSource(quantity * quantity, std::numeric_limits<int>::max() / 2);
		std::vector<bool> visited(quantity + 1, false);
		std::vector<int> path(quantity + 1, 0);

		int source = i;
		distanceFromSource[(source - 1) * quantity + source - 1] = 0;

		for (int count = 1; count <= quantity; ++count) {
			int minDistance = std::numeric_limits<int>::max();
			int u = 0;

			// Находим вершину с минимальным расстоянием
			for (int j = 1; j <= quantity; ++j) {
				if (!visited[j] && distanceFromSource[(source - 1) * quantity + j - 1] < minDistance) {
					minDistance = distanceFromSource[(source - 1) * quantity + j - 1];
					u = j;
				}
			}

			if (u == 0) {
				break;
			}

			visited[u] = true;

			// Обновляем расстояния до соседних вершин
			for (int v = 1; v <= quantity; ++v) {
				int weight = distance[(u - 1) * quantity + v - 1];
				if (weight != 0 && distanceFromSource[(source - 1) * quantity + u - 1] + weight < distanceFromSource[(source - 1) * quantity + v - 1]) {
					distanceFromSource[(source - 1) * quantity + v - 1] = distanceFromSource[(source - 1) * quantity + u - 1] + weight;
					path[v] = u;
				}
			}
		}

		// Выводим результаты для каждой пары вершин
		for (int j = 1; j <= quantity; ++j) {
			if (j != i) {
				std::cout << "Distance between vertices " << i << " and " << j << ": ";

				if (distanceFromSource[(source - 1) * quantity + j - 1] == std::numeric_limits<int>::max() / 2) {
					std::cout << "No path" << std::endl;
				}
				else {
					std::cout << distanceFromSource[(source - 1) * quantity + j - 1] + h[j] - h[i] << std::endl;
				}
			}
		}
	}
}
void Graph::lab_8()
{
	int begin_x = 0, begin_y = 0, end_x = 0, end_y = 0;
	cout << endl << "Введите последовательно координаты x и y сначала начальной вершины, затем конечной:" << endl;
	cin >> begin_x >> begin_y >> end_x >> end_y;
	cout << endl;

	struct Node {
		int x;
		int y;
		int height;
		int g_cost;
		int h_cost;
		int f_cost;
		Node* parent;

		Node(int x, int y, int height): x(x), y(y), height(height), g_cost(0), h_cost(0), f_cost(0), parent(nullptr) {}
	};

	std::vector<int> dx = { -1, 0, 1, -1, 1, -1, 0, 1 };
	std::vector<int> dy = { -1, -1, -1, 0, 0, 1, 1, 1 };

	auto isValid = [&](int x, int y) {
		return (x >= 0 && x < quantity && y >= 0 && y < quantity);
	};

	auto calcDistance = [&](Node* node, int end_x, int end_y) {
		return std::abs(node->x - end_x) + std::abs(node->y - end_y); // Манхэттенское расстояние
		//return std::sqrt(std::pow(node->x - end_x, 2) + std::pow(node->y - end_y, 2)); // Евклидово расстояние
		//return std::max(std::abs(node->x - end_x), std::abs(node->y - end_y)); // Чебышево расстояние
	};

	auto getNode = [&](int x, int y) {
		int position = x * quantity + y;
		return new Node(x, y, matrix[position]);
	};

	auto printPath = [&](Node* endNode) {
		std::vector<Node*> path;
		Node* currentNode = endNode;

		while (currentNode != nullptr) {
			path.push_back(currentNode);
			currentNode = currentNode->parent;
		}

		std::cout << "Path: ";

		for (int i = path.size() - 1; i >= 0; --i) {
			std::cout << "(" << path[i]->x << ", " << path[i]->y << ") ";

			if (i != 0) {
				std::cout << "-> ";
			}
		}

		std::cout << std::endl;
	};

	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	Node* startNode = getNode(begin_x, begin_y);
	Node* endNode = getNode(end_x, end_y);

	startNode->g_cost = 0;
	startNode->h_cost = calcDistance(startNode, endNode->x, endNode->y);
	startNode->f_cost = startNode->g_cost + startNode->h_cost;

	openList.push_back(startNode);

	while (!openList.empty()) {
		Node* currentNode = openList[0];
		int currentIndex = 0;

		for (int i = 1; i < openList.size(); ++i) {
			if (openList[i]->f_cost < currentNode->f_cost) {
				currentNode = openList[i];
				currentIndex = i;
			}
		}

		openList.erase(openList.begin() + currentIndex);
		closedList.push_back(currentNode);

		if (currentNode->x == endNode->x && currentNode->y == endNode->y) {
			printPath(currentNode);
			std::cout << "Path length: " << currentNode->g_cost << std::endl;
			float exploredPercentage = (float)closedList.size() / (quantity * quantity) * 100;
			std::cout << "Explored percentage: " << exploredPercentage << "%" << std::endl;
			return;
		}

		for (int i = 0; i < 8; ++i) {
			int newX = currentNode->x + dx[i];
			int newY = currentNode->y + dy[i];

			if (isValid(newX, newY)) {
				Node* neighborNode = getNode(newX, newY);

				if (std::find_if(closedList.begin(), closedList.end(), [&](Node* node) {
					return (node->x == neighborNode->x && node->y == neighborNode->y);
					}) != closedList.end()) {
					continue;
				}

				int newGCost = currentNode->g_cost + std::abs(neighborNode->height - currentNode->height);

				if (std::find_if(openList.begin(), openList.end(), [&](Node* node) {
					return (node->x == neighborNode->x && node->y == neighborNode->y);
					}) == openList.end() || newGCost < neighborNode->g_cost) {
					neighborNode->g_cost = newGCost;
					neighborNode->h_cost = calcDistance(neighborNode, endNode->x, endNode->y);
					neighborNode->f_cost = neighborNode->g_cost + neighborNode->h_cost;
					neighborNode->parent = currentNode;

					if (std::find_if(openList.begin(), openList.end(), [&](Node* node) {
						return (node->x == neighborNode->x && node->y == neighborNode->y);
						}) == openList.end()) {
						openList.push_back(neighborNode);
					}
				}
			}
		}
	}

	std::cout << "Path not found" << std::endl;

	
}
void Graph::lab_9()
{
	do_no_orient();
	auto start = high_resolution_clock::now();

	vector<Edge> result;
	double duration = 0;

	char key = algorithm_input_4lab();

	if (key == 'k') {
		result = kruskal(matrix, quantity);
		//auto stop = high_resolution_clock::now();
		result_cout(result);
		//auto duration = std::chrono::high_resolution_clock::now();
		//double l = high_resolution_clock::now();
		//cout << endl << "time of work: " << duration << endl;
		//return;
	}
	else if (key == 'p') {
		result = prim(matrix, quantity);
		//auto stop = high_resolution_clock::now();
		result_cout(result);
		//return;
	}
	else if (key == 'b') {
		result = boruvka(matrix, quantity);
		//auto stop = high_resolution_clock::now();
		result_cout(result);
		//return;
	}
	else if (key == 's') {
		result = kruskal(matrix, quantity);
		//auto stop = high_resolution_clock::now();
		//auto start = high_resolution_clock::now();
		result = prim(matrix, quantity);
		//auto stop = high_resolution_clock::now();
		//auto start = high_resolution_clock::now();
		result = boruvka(matrix, quantity);
		//auto stop = high_resolution_clock::now();
	}
	else {
		cout << "Invalid key!" << endl;
		return;
	}

	auto stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start).count() / 1000.0;
	cout << endl << duration;
}


// вспомогательные функции //

// проверяет открывается ли файл и есть ли в нём данные, возвращает адрес
string Graph::file_info_input()
{
	string input;
	ifstream file;
	system("CLS");
stfw:
	cout << endl << "нужно ввести адрес файла, из которого необходимо ввести данные";
	cout << endl << "если файл находится в папке, находящейся в папке с программой, то адрес выглядит так:";
	cout << endl << "docs\\file.txt";
	cout << endl << "если файл находится где-либо еще, то необходимо писать полный путь";
	cout << endl << "пример: D:\\docs\\file.txt";
	cout << endl << "как правило, файлы с русскими буквами в пути не открываются";
	cout << endl << "введите адрес файла(если нужно выйти из программы введите 1): "
		 << endl << endl;
	cin >> input;
	// выход из программы
	if (input == "1") {
		exit(0);
	}
	file.open(input);
	// если неудача
	if (!file || file.eof() || !file.is_open()) {
		if (file.eof()) cout << "файл пуст" << endl;
		if (!file.is_open()) cout << "файл не открыт" << endl;
		if (!file) cout << "ошибка открытия файла" << endl;
		file.close();
		cout << "попробуйте снова" << endl;
		system("pause");
		system("CLS");
		goto stfw;
	}

	cout << endl << "файл успешно открыт и он не пуст" << endl;
	file.close();
	return input;
}
// проверка на наличие весов ребер в файле со списком ребер
bool Graph::check_edges(char check[50])
{
	int i = 0;
	bool key = 0;
	while (isdigit(check[i]) || check[i] == ' ' && key == 0) {
		if (check[i] == ' ') key = 1;
		i++;
	}
	i++;
	if (isdigit(i)) return true;
	return false;
}
// проверка на ориентированность графа
void Graph::check_orientation()
{
	int n = 0, m = 0;
	for (int i = 0; i < quantity; i++) {
		for (int j = 0; j < quantity; j++) {
			n = i * quantity + j;
			m = j * quantity + i;
			if (matrix[n] != matrix[m]) {
				key_orientation = 1;
				return;
			}
		}
	}
	return;
}
// Поиск в глубину для 3 лабы
void Graph::dfs(int v, int p) {
	visited[v] = true;
	tin[v] = fup[v] = timer++;
	int children = 0;
	bool isArticulationPoint = false;
	int n = quantity;

	for (int u = 0; u < n; ++u) {
		int position = v * quantity + u;
		if (matrix[position] == 1) {
			if (u == p) continue;
			if (visited[u]) {
				fup[v] = min(fup[v], tin[u]);
			}
			else {
				dfs(u, v);
				fup[v] = min(fup[v], fup[u]);

				if (fup[u] >= tin[v]) {
					isArticulationPoint = true;
				}

				if (fup[u] > tin[v]) {
					// (v, u) - мост
					// cout << "Мост: " << v << " - " << u << endl;
					bridge z;
					z.v1 = v; z.v2 = u;
					bridges.push_back(z);
				}
				children++;
			}
		}
	}

	if (p == -1 && children >= 2) {
		// v - шарнирная вершина
		// cout << "Шарнирная вершина: " << v << endl;
		vertexs.push_back(v);
	}
	else if (p != -1 && isArticulationPoint) {
		// v - шарнирная вершина
		// cout << "Шарнирная вершина: " << v << endl;
		vertexs.push_back(v);
	}
};
// делает соотнесенный граф
void Graph::do_no_orient()
{
	int position = 0;
	int inv_position = 0;
	for (int i = 0; i < quantity; i++) {
		for (int j = 0; j < quantity; j++) {
			position = i * quantity + j;
			inv_position = j * quantity + i;
			if (matrix[position] == 1) matrix[inv_position] = 1;
		}
	}
}
// ввод начальной вершины для 5 и 6 лабы
int Graph::begin_vertex_number()
{
	int k = 0;
bvns:
	cout << endl << "введите номер начальной вершины от 0 до " << quantity << ':' << endl;
	cin >> k;
	if (k < 0 || k > quantity) {
		cout << "ввод неверный, попробуйте еще раз";
		goto bvns;
	}
	return k - 1;
}
// ввод конечной вершины для 5 лабы
int Graph::end_vertex_number()
{
	int k = 0;
evns:
	cout << endl << "введите номер конечной вершины от 0 до " << quantity << ':' << endl;
	cin >> k;
	if (k < 0 || k > quantity) {
		cout << "ввод неверный, попробуйте еще раз";
		goto evns;
	}
	return k - 1;
}
// возвращает какой алгоритм выполнять в 6 лабе
char Graph::algorithm_input_6lab()
{
	char algorithm = '0';
ai6s:
	cout << endl << "Введите ключ:" << endl;
	cout << "- d – алгоритм Дейкстры:" << endl;
	cout << "- b – алгоритм Беллмана-Форда-Мура" << endl;
	cout << "- t – алгоритм Левита" << endl;
	cin >> algorithm;
	if (algorithm != 'd' && algorithm != 'b' && algorithm != 't') {
		cout << "Неверный ввод, попробуйте снова" << endl;
		goto ai6s;
	}
	return algorithm;
}
char Graph::algorithm_input_4lab()
{
	char algorithm = '0';
ai6s:
	cout << endl << "Введите ключ:" << endl;
	cout << "- k – алгоритм Крускала" << endl;
	cout << "- p – алгоритм Прима" << endl;
	cout << "- b – алгоритм  Борувки" << endl;
	cout << "- s – расчёт производится тремя алгоритмами поочерёдно" << endl;
	cin >> algorithm;
	if (algorithm != 'k' && algorithm != 'p' && algorithm != 'b' && algorithm != 's') {
		cout << "Неверный ввод, попробуйте снова" << endl;
		goto ai6s;
	}
	return algorithm;
}
// к 4 лабе в2
int find(Subset subsets[], int i) {
	if (subsets[i].parent != i)
		subsets[i].parent = find(subsets, subsets[i].parent);
	return subsets[i].parent;
}
void Union(Subset subsets[], int x, int y) {
	int xroot = find(subsets, x);
	int yroot = find(subsets, y);

	if (subsets[xroot].rank < subsets[yroot].rank)
		subsets[xroot].parent = yroot;
	else if (subsets[xroot].rank > subsets[yroot].rank)
		subsets[yroot].parent = xroot;
	else {
		subsets[yroot].parent = xroot;
		subsets[xroot].rank++;
	}
}
bool compareEdges(Edge a, Edge b) {
	return a.weight < b.weight;
}
vector<Edge> kruskal(vector<int> matrix, int quantity) {
	vector<Edge> edges;
	for (int i = 0; i < quantity - 1; i++) {
		for (int j = i + 1; j < quantity; j++) {
			int weight = matrix[i * quantity + j];
			if (weight != 0) {
				edges.push_back({ i, j, weight });
			}
		}
	}

	vector<Edge> result;
	sort(edges.begin(), edges.end(), compareEdges);

	Subset* subsets = new Subset[quantity * sizeof(Subset)];
	for (int v = 0; v < quantity; v++) {
		subsets[v].parent = v;
		subsets[v].rank = 0;
	}

	int e = 0;
	int i = 0;
	while (e < quantity - 1 && i < edges.size()) {
		Edge next_edge = edges[i++];
		int x = find(subsets, next_edge.source);
		int y = find(subsets, next_edge.destination);

		if (x != y) {
			result.push_back(next_edge);
			Union(subsets, x, y);
			e++;
		}
	}

	delete[] subsets;

	return result;
}
vector<Edge> prim(vector<int> matrix, int quantity) {
	vector<Edge> result;
	vector<bool> mstSet(quantity, false);
	vector<int> key(quantity, INT_MAX);

	key[0] = 0;

	for (int count = 0; count < quantity - 1; count++) {
		int u = -1;

		for (int v = 0; v < quantity; v++)
			if (!mstSet[v] && (u == -1 || key[v] < key[u]))
				u = v;

		mstSet[u] = true;

		for (int v = 0; v < quantity; v++) {
			int weight = matrix[u * quantity + v];
			if (weight != 0 && !mstSet[v] && weight < key[v]) {
				result.push_back({ u, v, weight });
				key[v] = weight;
			}
		}
	}
	return result;
}
vector<Edge> boruvka(vector<int> matrix, int quantity) {
	vector<Edge> edges;
	for (int i = 0; i < quantity - 1; i++) {
		for (int j = i + 1; j < quantity; j++) {
			int weight = matrix[i * quantity + j];
			if (weight != 0) {
				edges.push_back({ i, j, weight });
			}
		}
	}

	vector<Edge> result;
	sort(edges.begin(), edges.end(), compareEdges);

	Subset* subsets = new Subset[quantity * sizeof(Subset)];
	for (int v = 0; v < quantity; v++) {
		subsets[v].parent = v;
		subsets[v].rank = 0;
	}

	int e = 0;
	int i = 0;
	while (e < quantity - 1 && i < edges.size()) {
		Edge next_edge = edges[i++];
		int x = find(subsets, next_edge.source);
		int y = find(subsets, next_edge.destination);

		if (x != y) {
			result.push_back(next_edge);
			Union(subsets, x, y);
			e++;
		}
	}

	delete[] subsets;

	return result;
}
void result_cout(vector<Edge> result) 
{
	cout << endl << "вывод результатов алгоритма:" << endl;
	int w = 0;
	for (int i = 0; i < result.size(); i++) {
		cout << result[i].source + 1 << ' ' << result[i].destination + 1 << ' ' << result[i].weight << endl;
		w += result[i].weight;
	}
	cout << endl;
	cout << w;
};
#endif
