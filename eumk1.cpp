#include <set>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <functional>  
#include <ctime>

using namespace std;

int n;
vector <string> text;
multimap <int, pair<string, string>, greater<>> pairs;
set <string> words_exclusion;
multimap <int, string, greater<>> often_words;

void input() {
	ifstream file("text10000000(4).txt");
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
		if (!words_exclusion.count(i)) words[i]++;

	//��������� ����� �������, ������������� �� ���������� ���� � �������
	//� ������� ��������, ����� ����� ���� �������� N �������� 
	//����� ������������� ����
	for (auto i : words)
		often_words.insert({ i.second, i.first });
}

void create_pairs() {
	map <pair<string, string>, int> a;
	int k = 0;

	//�������� �� N ����� ������������� ������ � ������
	for (auto i = begin(often_words); k < n && i != end(often_words); k++, i++) {
		//���� ��� �������� ������ ������ ������
		if (text[0] == i->second && !words_exclusion.count(text[1]))
			a[make_pair(i->second, text[1])]++;
		//���� ��� �������� ��������� ������ ������
		if (text[size(text) - 1] == i->second && !words_exclusion.count(text[size(text) - 2]))
			a[make_pair(text[size(text) - 2], i->second)]++;

		//�������� �� ����� ������
		for (auto j = begin(text)+1; j != end(text)-1; j++)
			if (i->second == *j) {
				if (!words_exclusion.count(*(j - 1)))
					a[make_pair(*(j - 1), *j)]++;
				if (!words_exclusion.count(*(j + 1)))
					a[make_pair(*j, *(j + 1))]++;
			}

		pair<pair<string, string>, int> res({ "","" }, 0);

		for (auto p : a) if (p.second > res.second) res = p;

		//��������� ����� ����� ������������� ���� � ������ ����� ���
		pairs.insert({ res.second, res.first });
		a.clear();
	}
}

void output() {
	ofstream file("result.txt");
	for (auto i : pairs)
		file << i.first << " - " << i.second.first << ' ' << i.second.second << endl;
	file.close();
}

void main() {
	input();
	unsigned int beginTime = clock();
	create_often_words();
	create_pairs();
	unsigned int endTime = clock();
	ofstream file("time.txt");
	file << endTime - beginTime;
	file.close();
	output();
}