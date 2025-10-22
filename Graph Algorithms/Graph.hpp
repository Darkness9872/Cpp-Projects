#pragma once
#ifndef LR_TG_LAB_CLASS
#define LR_TG_LAB_CLASS

#include <string>
#include <vector>

using namespace std;

struct bridge;
class Graph
{
protected:
	vector <int> matrix;
	int quantity = 0;
	bool key_orientation = 0;
	bool key_file_output = 0;
	string file_output_address;
	// ��� ������� ����
	vector<bool> visited;   // ������ ��� ������������ ���������� ������
	vector<int> tin, fup;   // ������� ������ ����� � �������� ��������� �������
	int timer = 0;
	vector <int> vertexs;	// ��� ��������
	vector <bridge> bridges; // ��� ������ � �����
	// ������� ��� ����� ������
	void read_data();
	void read_matrix();
	void read_edges();
	void read_adjacency();
	// ��������������� �������
	int begin_vertex_number();
	int end_vertex_number();
	char algorithm_input_6lab();
	char algorithm_input_4lab();
	string file_info_input();
	bool check_edges(char check[50]);
	void check_orientation();
	void dfs(int v, int p = -1);
	void do_no_orient();
public:
	// ������� ��� ������
	void info_output();
	// �������� �������
	Graph(bool key_lab8 = 0);
	// ����� ������� ��
	int weight(int vi, int vj);
	bool is_edge(int vi, int vj);
	void adjacency_matrix();
	void adjacency_list(int v);
	void list_of_edges();
	void list_of_edges(int v);
	bool is_directed();
	// ������� ������������ �����
	void lab_1();
	void lab_2();
	void lab_3();
	void lab_4();
	void lab_5();
	void lab_6();
	void lab_7();
	void lab_8();
	void lab_9();
};
struct bridge {
	int v1 = 0, v2 = 0;
};
#endif
