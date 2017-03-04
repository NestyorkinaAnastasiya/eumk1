#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib> 
#include <ctime>
#include <algorithm>
using namespace std;

int countWords, countLetters;
vector <string> text;
set <string> words_exclusion = {"at","in", "on",
"to","from", "into", "after", "during", "and", "or"};
vector<char> letters;
set <char> L = { 'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m' };

void input() {
	ifstream file("n.txt");
	file >> countWords;
	file >> countLetters;
	file.close();
	for (auto i : L)
		letters.push_back(i);
}

void generator()
{
	ofstream file("text.txt");
	int i;
	string s;
	srand(time(0));
	for (int j = 0; j < countWords;){
		i = rand() % (letters.size());
		s = letters[i];
		for (int k = 1; k < countLetters; k++){
			i = rand() % (letters.size());
			s += letters[i];
		}
		while(i==0)
			i = rand() % (20);
		for (int r = 0; r < i && (text.size() + r) < countWords; r++)
		{
			text.push_back(s);
			j++;
		}
	}	

	random_shuffle(begin(text),end(text));

	for (int j = 0; j < text.size();j++){
		file << text[j];
		if ((j + 1) != countWords)
			if ((j + 1) % 6 == 0) file << endl;
			else file << ' ';
	}
	file.close();

	file.open("word-exclusion.txt");
	for (auto i : words_exclusion)
		file << i << ' ';
	file.close();
}

void main()
{
	input();
	generator();
}
