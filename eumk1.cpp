#include <set>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <functional>  

using namespace std;
struct pair_{
	string left;
	string right;
};

bool operator<(pair_ a, pair_ b){
		if (a.left < b.left) return true;
		else return false;
}

int n;
vector <string> text;
vector <pair_> pairs;
set <string> words_exclusion;
multimap <int, string, greater<>> often_words;

void input(){
	ifstream file("text.txt");
	string word;

	while (!file.eof()){
		file >> word;
		text.push_back(word);
	}
	file.close();

	file.open("word-exclusion.txt");
	while (!file.eof()){
		file >> word;
		words_exclusion.insert(word);
	}
	file.close();

	file.open("n.txt");
	file >> n;
	file.close();
}

void create_often_words(){
	map <string, int> words;
	for (auto i : text)
		//если слово не пренадлежит словам-исключениям
		if(!words_exclusion.count(i)){
			//если нет ещё такого слова, добавляем в словарь
			if (!words.count(i)) words.insert({ i, 1 });
			else { //иначе ищем это слово в словаре 
				auto &elem = words.find(i);
				//и увеличиваем счётчик таких слов на единицу
				elem->second++;
			} 
		}
	//если словарь не пуст
	if (words.size()){
		//заполняем новый словарь, упорядоченный по количеству слов в словаре
		//в порядке убывания, чтобы можно было выделить N наиболее 
		//часто встречающихся слов
		for (auto i : words) 
			often_words.insert({ i.second, i.first });
		//очищаем память, выделенную под временный словарь
		words.erase(words.begin(), words.end());
	}
}

void add_pair(map <pair_, int> &a, string left, string right){
	pair_ p;
	p.left = left;
	p.right = right;
	if (!a.count(p)) a.insert({ p, 1 });
	else {
			auto &elem = a.find(p);
			elem->second++;
	}
}

void create_pairs(){
	map <pair_, int> a;
	map <int, pair_, greater<>> b;
	int k = 0;
	//проходим по N часто встречающимся словам в тексте
	for (auto i = begin(often_words); k < n; k++, i++) {
		auto j = begin(text);
		//проходим по всему тексту
		while (j != end(text)){	
			//ищем указатель на текущее часто встречающееся слово в тексте
			for (;  j != end(text) && i->second != *j ; j++);

			if (j != end(text)){ 
				//если оно является первым словом текста
				if (j == begin(text)) {
					//если правое от него слово не пренадлежит словам-исключениям
					if (!words_exclusion.count(*(j+1)))
						//добавляем данную пару слов в словарь
						add_pair(a, *j, *(j + 1));
				}

				//если оно является последним словом текста
				else if (j == end(text)-1) {
					if (!words_exclusion.count(*(j - 1))) 
						add_pair(a, *(j - 1), *j);
				}

				//когда возможна пара и слева, и справа
				else {
					if (!words_exclusion.count(*(j - 1))) add_pair(a, *(j - 1), *j);
					if (!words_exclusion.count(*(j + 1))) add_pair(a, *j, *(j + 1));
				}

				j++;
			}
		}

		//если словарь пар составлен для данного слова
		if (a.size()){
			//упорядочиваем его по убыванию количества слов в новом словаре
			for (auto i : a) b.insert({ i.second, i.first });
			//добавляем самую часто встречающуюся пару в вектор таких пар
			pairs.push_back(begin(b)->second);
			//освобождаем память
			a.erase(a.begin(), a.end());
			b.erase(b.begin(), b.end());
		}
	}
}

void output() {
	ofstream file("result.txt");
	for (auto i : pairs)
		file << i.left << ' ' << i.right << endl;
	file.close();
}

void main(){
	input();
	create_often_words();
	create_pairs();
	output();
}