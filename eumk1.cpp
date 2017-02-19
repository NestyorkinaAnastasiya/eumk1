#include <set>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <functional>  

using namespace std;

int n;
vector <string> text;
vector <pair<string, string>> pairs;
set <string> words_exclusion;
multimap <int, string, greater<>> often_words;

void input() {
	ifstream file("text.txt");
	string word;

	while (file >> word)
		text.push_back(word);
	file.close();

	file.open("word-exclusion.txt");
	while (file >> word)
		words_exclusion.insert(word);
	file.close();

	file.open("n.txt");
	file >> n;
	file.close();
}

void create_often_words() {
	map <string, int> words;
	for (auto i : text)
		//���� ����� �� ����������� ������-�����������
		if (!words_exclusion.count(i)) {
			//���� ��� ��� ������ �����, ��������� � �������
			if (!words.count(i)) words.insert({ i, 1 });
			//����� ���� ��� ����� � ������� 
			//� ����������� ������� ����� ���� �� �������
			else words.find(i)->second++;

		}
	//���� ������� �� ����
	if (words.size()) {
		//��������� ����� �������, ������������� �� ���������� ���� � �������
		//� ������� ��������, ����� ����� ���� �������� N �������� 
		//����� ������������� ����
		for (auto i : words)
			often_words.insert({ i.second, i.first });
		//������� ������, ���������� ��� ��������� �������
		words.clear();
	}
}

void add_pair(map <pair<string, string>, int> &a, string left, string right) {
	pair<string, string> p;
	p.first = left;
	p.second = right;
	if (!a.count(p)) a.insert({ p, 1 });
	else a.find(p)->second++;
}

void create_pairs() {
	map <pair<string, string>, int> a;
	map <int, pair<string, string>, greater<>> b;
	int k = 0;
	//�������� �� N ����� ������������� ������ � ������
	for (auto i = begin(often_words); k < n; k++, i++) {
		//�������� �� ����� ������
		for (auto j = begin(text); j != end(text); j++)
			if (i->second == *j)
				//���� ��� �������� ������ ������ ������
				if (j == begin(text)) {
					//���� ������ �� ���� ����� �� ����������� ������-�����������
					if (!words_exclusion.count(*(j + 1)))
						//��������� ������ ���� ���� � �������
						add_pair(a, *j, *(j + 1));
				}

				//���� ��� �������� ��������� ������ ������
				else if (j == end(text) - 1) {
					if (!words_exclusion.count(*(j - 1)))
						add_pair(a, *(j - 1), *j);
				}

				//����� �������� ���� � �����, � ������
				else {
					if (!words_exclusion.count(*(j - 1))) add_pair(a, *(j - 1), *j);
					if (!words_exclusion.count(*(j + 1))) add_pair(a, *j, *(j + 1));
				}
	
		//���� ������� ��� ��������� ��� ������� �����
		if (a.size()) {
			//������������� ��� �� �������� ���������� ���� � ����� �������
			for (auto i : a) b.insert({ i.second, i.first });
			//��������� ����� ����� ������������� ���� � ������ ����� ���
			pairs.push_back(begin(b)->second);
			//����������� ������
			a.clear();	b.clear();
		}
	}
}

void output() {
	ofstream file("result.txt");
	for (auto i : pairs)
		file << i.first << ' ' << i.second << endl;
	file.close();
}

void main() {
	input();
	create_often_words();
	create_pairs();
	output();
}