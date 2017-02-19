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
		//если слово не пренадлежит словам-исключениям
		if (!words_exclusion.count(i)) {
			//если нет ещё такого слова, добавляем в словарь
			if (!words.count(i)) words.insert({ i, 1 });
			//иначе ищем это слово в словаре 
			//и увеличиваем счётчик таких слов на единицу
			else words.find(i)->second++;

		}
	//если словарь не пуст
	if (words.size()) {
		//заполняем новый словарь, упорядоченный по количеству слов в словаре
		//в порядке убывания, чтобы можно было выделить N наиболее 
		//часто встречающихся слов
		for (auto i : words)
			often_words.insert({ i.second, i.first });
		//очищаем память, выделенную под временный словарь
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
	//проходим по N часто встречающимся словам в тексте
	for (auto i = begin(often_words); k < n; k++, i++) {
		//проходим по всему тексту
		for (auto j = begin(text); j != end(text); j++)
			if (i->second == *j)
				//если оно является первым словом текста
				if (j == begin(text)) {
					//если правое от него слово не пренадлежит словам-исключениям
					if (!words_exclusion.count(*(j + 1)))
						//добавляем данную пару слов в словарь
						add_pair(a, *j, *(j + 1));
				}

				//если оно является последним словом текста
				else if (j == end(text) - 1) {
					if (!words_exclusion.count(*(j - 1)))
						add_pair(a, *(j - 1), *j);
				}

				//когда возможна пара и слева, и справа
				else {
					if (!words_exclusion.count(*(j - 1))) add_pair(a, *(j - 1), *j);
					if (!words_exclusion.count(*(j + 1))) add_pair(a, *j, *(j + 1));
				}
	
		//если словарь пар составлен для данного слова
		if (a.size()) {
			//упорядочиваем его по убыванию количества слов в новом словаре
			for (auto i : a) b.insert({ i.second, i.first });
			//добавляем самую часто встречающуюся пару в вектор таких пар
			pairs.push_back(begin(b)->second);
			//освобождаем память
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