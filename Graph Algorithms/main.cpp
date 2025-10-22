#include "Graph.hpp"
#include "Functions_labs.hpp"

#include <iostream>


/* task2\matrix_t2_004.txt
   task1\matrix_t1_013.txt
   task1\list_of_edges_t1_001
   task1\list_of_adjacency_t1_013.txt
   task3\matrix_t3_005.txt
   task4\matrix_t4_007.txt
   task5\matrix_t5_004.txt
   task6\matrix_t6_005.txt
   task7\matrix_t7_001.txt
   task8\map_003.txt
*/
int main()
{
	setlocale(LC_ALL, "Russian");
start:
	cout << "Добро пожаловать в программу, вы можете ввести номер лабораторной работы для соответствующего решения или вызвать справку для общей информации" << endl;
	cout << "Внимание! Справку возможно вызвать только на данном этапе работы" << endl;
	cout << "введите число от 1 до 11 для открытия соответствующей лабораторной, ''help'' или 12 для выхода из программы" << endl;
	string input_1;
	cin >> input_1;
	// вызов справки
	if (input_1 == "help") {
		cout << endl;
		cout << "выполнил: студент группы М3О-225Бк-21 Пехов Дмитрий Викторович" << endl;
		cout << "предмет - теория графов" << endl;
		cout << "ключи будут запрашиваться в ходе выполнения программы с подробным объяснением" << endl;
		system("pause");
		system("CLS");
		goto start;
	}
	// вызов 1 лабы
	if (input_1 == "1") {
		lab_1();
		return 0;
	}
	// вызов 2 лабы
	if (input_1 == "2") {
		lab_2();
		return 0;
	}
	// вызов 3 лабы
	if (input_1 == "3") {
		lab_3();
		return 0;
	}
	// вызов 4 лабы
	if (input_1 == "4") {
		lab_9();
		return 0;
	}
	// вызов 5 лабы
	if (input_1 == "5") {
		lab_5();
		return 0;
	}
	// вызов 6 лабы
	if (input_1 == "6") {
		lab_6();
		return 0;
	}
	// вызов 7 лабы
	if (input_1 == "7") {
		lab_7();
		return 0;
	}
	// вызов 8 лабы
	if (input_1 == "8") {
		lab_8();
		return 0;
	}
	// вызов 9 лабы
	if (input_1 == "9") {

	}
	// вызов 10 лабы
	if (input_1 == "10") {

	}
	// вызов 11 лабы
	if (input_1 == "11") {

	}
	// выход
	if (input_1 == "12") {
		return 0;
	}
	cout << "ввод неккоректен, попробуйте снова" << endl;
	system("pause");
	system("CLS");
	goto start;
}