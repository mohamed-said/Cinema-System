/*
Movies System (Cinema System)

Name: Mohamed Alaa		ID: 20110282
Name: Mohamed Hassan	ID: 20110289
Name: Mohamed Said		ID: 20110298
Name: Mohamed Osama		ID: 20110287
*/
// Libraries Used
#include "stdafx.h" // No Idea :D
#include <iostream> // cin - cout
#include <fstream> // open() - close() - fail() - good()
#include <conio.h> // _getch()
#include <Windows.h> // COORD - SetConsoleTextAttribute - SetConsoleCursorPosition
#include <vector> // push_back() - clear() - size() - erase()
#include <string> // insert() - clear() - size() - erase()
#include <sstream> // >> - <<
#include <ctype.h> // isalpha() - isdigit() - isspace() - ispunct()

// Keys Difinitions (ASCII CODES)
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define ESCAPE 27
#define BACKSPACE 8

using namespace std;

// Movie Data
struct Movie {

	string Name;
	string cinema;
	float price;
	int quantity;
};

// System Functions
void gotoxy(int, int); // Go to a certain position on the console
void color(int); // Text color
int toInt(string); // Converts a string to integer
float toFloat(string); // Converts a string to float
string toStr(int); // Converts an integer to string
string toStr(float); // Converts a float to string
void getIn(string&, int, int, int, int, int, bool&, string); // Get line manually
void coutWidth(string, unsigned int); // Constrain a max length
void LoadUp(vector<Movie>&, string); // Load movie data to memory
void printMovie(Movie, int, int, int, int, int); // Print movie details
void ListMovie(vector<Movie>&, vector<Movie>&, bool); // List of movies
void PagiMovie(vector<Movie>&, int, int); // Movies using pagination
void MovieDetails(vector<Movie>&, vector<Movie> &, int, bool); // Display movie details
void addMovie(vector<Movie>&); // Add new movie
void saveNexit(vector<Movie>&, string); // Save data and exit
void SearchMovie(vector<Movie> &, vector<Movie> &); // Search Movie
void doSearch(vector<Movie> &, vector<Movie> &, int); // Proceed Searching
void deleteMovie(vector<Movie> &, bool quick, string, string); // Delete Movie
void updateMovie(vector<Movie> &, bool, string, string); // Update Moive
void doUpdate(vector<Movie> &, int); // Proceed Updating
//void Update(vector<Movie>& data, bool quick, string film, string cinema);

// Style Functions
void logo(); // Cinema Logo
void menu(vector<Movie>&); // Main Menu
void borders(int, int); // Borders :D

// constants
const int limit = 8; // pagination limits
const string file = "data.dat"; // data file
vector<Movie> search(0); // search results container

/////////////////////////////////////////////////////////////
int main() {
	
	vector<Movie> data(0);
	LoadUp(data, file);
	menu(data);

	gotoxy(0, 23);
	return 0;
}
////////////////////////////////////////////////////


///////////////////////////////////////////////// SYSTEM /////////////////////////////////////////////////
void gotoxy(int x, int y) {

	COORD Place;
	Place.X = x;
	Place.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Place);
	return;
}

void color(int c) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
	return;
}

int toInt(string s) {

	int x;
	stringstream ss;
	ss << s;
	ss >> x;
	return x;
}

float toFloat(string s) {

	float x;
	stringstream ss;
	ss << s;
	ss >> x;
	return x;
}

string toStr(int x) {

	string s;
	stringstream ss;
	ss << x;
	ss >> s;
	return s;
}

string toStr(float x) {

	string s;
	stringstream ss;
	ss << x;
	ss >> s;
	return s;
}

void getIn(string &str, int max, int px, int py, int ex, int ey, bool &esc, string type = "str") {

	int x = 0;
	char m;
	gotoxy(px + x, py);
	while((m = _getch())) {
		if(m < 0) {
			m = _getch();
			
			if(m == LEFT) {
				if(x > 0) {
					x--;
				}
			}else if(m == RIGHT) {
				if(x < (int)str.size()) {
					x++;
				}
			}
		}else if(m == ENTER) {
			if(!str.empty()) {
				break;
			}else {
				gotoxy(ex, ey);
				color(12);
				cout << "\t\t\t\t";
				gotoxy(ex, ey);
				cout << "Required";
			}
		}else if(m == ESCAPE) {
			esc = false;
			break;
		}else if(m == BACKSPACE) {
			if(!str.empty() && x > 0) {
				gotoxy(ex, ey);
				cout << "\t\t\t\t";
				str.erase(x - 1, 1);
				x--;
			}
		}else {
			if(type == "str") {
				if(isalpha(m) || isdigit(m) || ispunct(m) || isspace(m)) {
					if((int)str.size() >= max) {
						gotoxy(ex, ey);
						color(12);
						cout << "Maximum " << max << " characters";
					}else {
						gotoxy(ex, ey);
						cout << "\t\t\t\t";
						str.insert(x, 1, m);
						x++;
					}
				}
			}else if(type == "int") {

				if(isdigit(m) || m == '.') {
					if((int)str.size() >= max) {
						gotoxy(ex, ey);
						color(12);
						cout << "Maximum " << max << " digits";
					}else {
						gotoxy(ex, ey);
						cout << "\t\t\t\t";
						str.insert(x, 1, m);
						x++;
					}
				}else {
					gotoxy(ex, ey);
					color(12);
					cout << "Only digits";
				}
			}
		}
		gotoxy(px, py);
		for(int i = 0; i <= (int)str.size(); i++) cout << " ";
		gotoxy(px, py);
		color(14);
		cout << str;
		gotoxy(px + x, py);
	}

	return;
}

void coutWidth(string s, unsigned int max) {

	if(s.size() > max) {
		cout << s.substr(0, max - 3);
		cout << "...";
	}else {
		cout << s;
	}
	return;
}

void LoadUp(vector<Movie> &data, string file) {

	ifstream d(file);

	if(!d.fail()) {
		Movie M;
		string temp, name, cinema;
		int quantity, x = 0;
		float price;
		while(d.good() && !d.eof()) {
			getline(d, temp); // Get the whole line to split it up
			if(d.fail()) {
				break;
			}
			x = 0;
			name = temp.substr(1, temp.find("]", x) - 1);
			x = temp.find("[", x + 1) + 1;
			cinema = temp.substr(x, temp.find("]", x) - x);
			x = temp.find("[", x + 1) + 1;
			quantity = toInt(temp.substr(x, temp.find("]", x) - x));
			x = temp.find("[", x + 1) + 1;
			price = toFloat(temp.substr(x, temp.find("]", x) - x));
			
			M.Name = name;
			M.cinema = cinema;
			M.quantity = quantity;
			M.price = price;

			data.push_back(M);
		}
	}
	return;
}

void ListMovie(vector<Movie> &data, vector<Movie>&search, bool s = false) {

	system("CLS");
	logo();
	int size = (s) ? (int)search.size() : (int)data.size();

	if(size > 0) {
		color(10);
		gotoxy(4, 8);	cout << "________________________________________________________________________";
		gotoxy(3, 9);	cout << "|       Movie Name           |      Cinema          |  Price  |    Qt.   |";
		gotoxy(3, 10);	cout << "|____________________________|______________________|_________|__________|";
		gotoxy(3, 11);	cout << "|                            |                      |         |          |";
		gotoxy(3, 12);	cout << "|                            |                      |         |          |";
		gotoxy(3, 13);	cout << "|                            |                      |         |          |";
		gotoxy(3, 14);	cout << "|                            |                      |         |          |";
		gotoxy(3, 15);	cout << "|                            |                      |         |          |";
		gotoxy(3, 16);	cout << "|                            |                      |         |          |";
		gotoxy(3, 17);	cout << "|                            |                      |         |          |";
		gotoxy(3, 18);	cout << "|                            |                      |         |          |";
		gotoxy(3, 19);	cout << "|____________________________|______________________|_________|__________|";
		color(15);
		gotoxy(10, 22); cout << "Back";
		char ch;
		int start = 0, m;

		if(size - start > limit) { // Num Of Entries per Page grater than or equals Limit
			m = limit;
		}else { // Num Of Entries per Page less than Limit
			m = size - start;
		}
		if(s) {
			PagiMovie(search, start, limit);
		}else {
			PagiMovie(data, start, limit);
		}
		gotoxy(0, 0);
		while((ch = _getch())) {
			
			if(ch < 0) {
				ch = _getch();
				if(ch == RIGHT) {
					if(start + limit < size) {
						start += limit;
						if(s) {
							PagiMovie(search, start, limit);
						}else {
							PagiMovie(data, start, limit);
						}
						if(size - start > limit) { // Num Of Entries per Page grater than or equals Limit
							if(m != limit) {
								gotoxy(2, 11 + m);
								cout << " ";
							}
							m = limit;

						}else { // Num Of Entries per Page less than Limit
							if(m != size - start) {
								gotoxy(2, 11 + m);
								cout << " ";
							}
							m = size - start;
						}
					}
				}else if(ch == LEFT) {
					if(start - limit >= 0) {
						start -= limit;
						if(s) {
							PagiMovie(search, start, limit);
						}else {
							PagiMovie(data, start, limit);
						}
						if(size - start > limit) { // Num Of Entries per Page grater than or equals Limit
							if(m != limit) {
								gotoxy(2, 11 + m);
								cout << " ";
							}
							m = limit;
						}else { // Num Of Entries per Page less than Limit
							if(m != size - start) {
								gotoxy(2, 11 + m);
								cout << " ";
							}
							m = size - start;
						}
					}
				}else if(ch == UP) {
					
					if(size - start > limit) { // Num Of Entries per Page grater than or equals Limit
						if(m == limit) {
							gotoxy(8, 22);
							cout << " ";
						}else {
							gotoxy(2, 11 + m);
							cout << " ";
						}
						if(m - 1 >= 0) {
							m--;
						}else {
							m = limit;
						}
						if(m == limit) {
							gotoxy(8, 22);
							color(15);
							cout << "\4";
						}else {
							gotoxy(2, 11 + m);
							color(15);
							cout << "\4";
						}
					}else { // Num Of Entries per Page less than Limit

						if(m == size - start) {
							gotoxy(8, 22);
							cout << " ";
						}else {
							gotoxy(2, 11 + m);
							cout << " ";
						}
						if(m - 1 >= 0) {
							m--;
						}else {
							m = size - start;
						}
						if(m == size - start) {
							gotoxy(8, 22);
							color(15);
							cout << "\4";
						}else {
							gotoxy(2, 11 + m);
							color(15);
							cout << "\4";
						}

					}

				}else if(ch == DOWN) {
					if(size - start > limit) { // Num Of Entries per Page grater than or equals Limit
						if(m == limit) {
							gotoxy(8, 22);
							cout << " ";
						}else {
							gotoxy(2, 11 + m);
							cout << " ";
						}
						if(m < limit) {
							m++;
						}else {
							m = 0;
						}
						if(m == limit) {
							gotoxy(8, 22);
							color(15);
							cout << "\4";
						}else {
							gotoxy(2, 11 + m);
							color(15);
							cout << "\4";
						}
					}else { // Num Of Entries per Page less than Limit
						if(m == size - start) {
							gotoxy(8, 22);
							cout << " ";
						}else {
							gotoxy(2, 11 + m);
							cout << " ";
						}
						if(m < size - start) {
							m++;
						}else {
							m = 0;
						}
						if(m == size - start) {
							gotoxy(8, 22);
							color(15);
							cout << "\4";
						}else {
							gotoxy(2, 11 + m);
							color(15);
							cout << "\4";
						}
					}
				}
			}else if(ch == ENTER) {
				break;
			}else if(ch == ESCAPE) {
				if(size - start > limit) {
					m = limit;
				}else {
					m = size - start;
				}
				break;
			}
			gotoxy(0, 0);
		}

		if(size - start > limit) { // Num Of Entries per Page grater than or equals Limit
			if(m == limit) {
				menu(data);
			}else {
				MovieDetails(data, search, start + m, s);
			}

		}else {
			if(m == size - start) {
				menu(data);
			}else {
				MovieDetails(data, search, start + m, s);
			}
		}

	}else { // NO MOVIES DATA
		gotoxy(33, 15);
		color(12);
		if(s) {
			cout << "Couldn't Find Data";
		}else {
			cout << "No Movies Data";
		}
		gotoxy(36, 19);
		color(15);
		cout << "\4 Back";
		gotoxy(0, 0);
		char m;
		while((m = _getch())) {
			if(m == ENTER || m == ESCAPE) {
				break;
			}
		}
		menu(data);
	}
	return;
}

void PagiMovie(vector<Movie> &data, int start, int limit) {

	for(int i = 0; i < limit; i++) {
		gotoxy(3, 11 + i);
		color(10);
		cout << "|                            |                      |         |          |";
	}
	gotoxy(51, 22);	cout << "\t\t\t";
	int current = start / limit + 1, total = (data.size() % limit == 0) ? data.size() / limit : data.size() / limit + 1;
	if(current == 1 && current == total) {
		gotoxy(61, 22);	cout << current;
	}else if(current == 1) {
		color(7);
		gotoxy(65, 22);	cout << "Next \32";
		gotoxy(61, 22);	cout << current;
	}else if(current == total) {
		color(7);
		gotoxy(51, 22);	cout << "\33 Prev";
		gotoxy(61, 22);	cout << current;
	}else {
		color(7);
		gotoxy(61 - (toStr(current).size() / 2), 22);	cout << current;
		gotoxy(51, 22);	cout << "\33 Prev";
		gotoxy(65, 22);	cout << "Next \32";
	}
	int t = 0;
	for(unsigned int i = start; i < data.size() && t < limit; i++) {
		color(15);
		printMovie(data[i], 5, 34, 57, 67, 11 + t);
		t++;
	}
	color(15);
	gotoxy(8, 22); cout << "\4";
	return;
}

void printMovie(Movie M, int name, int cine, int pr, int qt, int y) {

	gotoxy(name, y);
	coutWidth(M.Name, 26);
	gotoxy(cine, y);
	coutWidth(M.cinema, 20);
	gotoxy(pr, y);
	coutWidth(toStr(M.price), 7);
	gotoxy(qt, y);
	coutWidth(toStr(M.quantity), 8);
	return;
}

void MovieDetails(vector<Movie> &data, vector<Movie> &search, int m, bool s = false) {

	system("CLS");
	logo();
	color(10);
	gotoxy(4, 8);	cout << "________________________________________________________________________";
	gotoxy(3, 9);	cout << "|              |                                                         |";
	gotoxy(3, 10);	cout << "|  Movie Name  |                                                         |";
	gotoxy(3, 11);	cout << "|______________|_________________________________________________________|";
	gotoxy(3, 12);	cout << "|              |                                                         |";
	gotoxy(3, 13);	cout << "|    Cinema    |                                                         |";
	gotoxy(3, 14);	cout << "|______________|_________________________________________________________|";
	gotoxy(3, 15);	cout << "|              |                                                         |";
	gotoxy(3, 16);	cout << "|     Price    |                                                         |";
	gotoxy(3, 17);	cout << "|______________|_________________________________________________________|";
	gotoxy(3, 18);	cout << "|              |                                                         |";
	gotoxy(3, 19);	cout << "|      Qt.     |                                                         |";
	gotoxy(3, 20);	cout << "|______________|_________________________________________________________|";
	color(7);
	gotoxy(8, 22);	cout << "U -> Update Movie     D -> Delete Movie     ESCAPE -> Back";
	color(15);
	if(s) {
		gotoxy(20, 10);	coutWidth(search[m].Name, 55);
		gotoxy(20, 13);	coutWidth(search[m].cinema, 55);
		gotoxy(20, 16);	coutWidth(toStr(search[m].price), 55);
		gotoxy(20, 19);	coutWidth(toStr(search[m].quantity), 55);
	}else {
		gotoxy(20, 10);	coutWidth(data[m].Name, 55);
		gotoxy(20, 13);	coutWidth(data[m].cinema, 55);
		gotoxy(20, 16);	coutWidth(toStr(data[m].price), 55);
		gotoxy(20, 19);	coutWidth(toStr(data[m].quantity), 55);
	}
	gotoxy(0, 0);
	char ch;
	int op = 3;
	while((ch = _getch())) {

		if(ch < 0) {
			continue;
		}else {

			if(toupper(ch) == 'U') {
				op = 1;
				break;
			}else if(toupper(ch) == 'D') {
				op = 2;
				break;
			}else if(ch == ESCAPE) {
				op = 3;
				break;
			}

		}
	}

	if(op == 1) {
		//Update(data, false, data[m].Name, data[m].cinema);
		updateMovie(data, false, data[m].Name, data[m].cinema);
	}else if(op == 2) {
		deleteMovie(data, false, data[m].Name, data[m].cinema);
	}else if(op == 3) {
		ListMovie(data, search, s);
	}

	return;
}

void addMovie(vector<Movie> &data) {

	Movie m;
	string temp;
	bool esc = true;
	system("CLS");
	logo();
	color(10);
	gotoxy(4, 8);	cout << "________________________________________________________________________";
	gotoxy(3, 9);	cout << "|              |                                                         |";
	gotoxy(3, 10);	cout << "|  Movie Name  |                                                         |";
	gotoxy(3, 11);	cout << "|______________|_________________________________________________________|";
	gotoxy(3, 12);	cout << "|              |                                                         |";
	gotoxy(3, 13);	cout << "|    Cinema    |                                                         |";
	gotoxy(3, 14);	cout << "|______________|_________________________________________________________|";
	gotoxy(3, 15);	cout << "|              |                                                         |";
	gotoxy(3, 16);	cout << "|     Price    |                                                         |";
	gotoxy(3, 17);	cout << "|______________|_________________________________________________________|";
	gotoxy(3, 18);	cout << "|              |                                                         |";
	gotoxy(3, 19);	cout << "|      Qt.     |                                                         |";
	gotoxy(3, 20);	cout << "|______________|_________________________________________________________|";
	color(7);
	gotoxy(57, 22);	cout << "ESCAPE -> Back";
	getIn(m.Name, 54, 20, 10, 8, 22, esc, "str");
	if(!esc) {
		menu(data);
		return;
	}
	getIn(m.cinema, 54, 20, 13, 8, 22, esc, "str");
	if(!esc) {
		menu(data);
		return;
	}
	getIn(temp, 8, 20, 16, 8, 22, esc, "int");
	if(!esc) {
		menu(data);
		return;
	}
	m.price = toFloat(temp);
	temp.clear();
	getIn(temp, 8, 20, 19, 8, 22, esc, "int");
	if(!esc) {
		menu(data);
		return;
	}
	m.quantity = toInt(temp);
	data.push_back(m);
	menu(data);
	return;
}

void saveNexit(vector<Movie> &data, string file) {

	ofstream d(file);
	int size = (int)data.size();
	for(int i = 0; i < size; i++) {
		d << "[" << data[i].Name << "] [" << data[i].cinema << "] [" << data[i].quantity << "] [" << data[i].price << "]";
		if(i < size - 1) {
			d << endl;
		}
	}
	exit(0);
	return;
}

void SearchMovie(vector<Movie> &data, vector<Movie> &search) {

	system("CLS");
	logo();
	if(!data.empty()) {

		int op = 0;
		char m;
		gotoxy(33, 13);
		cout << "Search By Name";
		gotoxy(33, 14);
		cout << "Search By Cinema";
		gotoxy(33, 15);
		cout << "Back";
		gotoxy(31, 13 + op);
		cout << "\4";
		gotoxy(0, 0);
		while((m = _getch())) {
			if(m < 0) {
				m = _getch();
				if(m == UP) {
					gotoxy(31, 13 + op);
					cout << " ";
					if(op > 0) {
						op--;
					}else {
						op = 2;
					}
					gotoxy(31, 13 + op);
					cout << "\4";
				}else if(m == DOWN) {
					gotoxy(31, 13 + op);
					cout << " ";
					if(op < 2) {
						op++;
					}else {
						op = 0;
					}
					gotoxy(31, 13 + op);
					cout << "\4";
				}
			}else if(m == ENTER) {
				break;
			}else if(m == ESCAPE) {
				op = 2;
				break;
			}
			gotoxy(0, 0);
		}

		if(op == 2) {
			menu(data);
		}else {
			doSearch(data, search, op);
		}
	}else {
		gotoxy(33, 15);
		color(12);
		cout << "No Movies Data";
		gotoxy(36, 19);
		color(15);
		cout << "\4 Back";
		gotoxy(0, 0);
		char m;
		while((m = _getch())) {
			if(m == ENTER || m == ESCAPE) {
				break;
			}
		}
		menu(data);
	}
	return;
}

void doSearch(vector<Movie> &data, vector<Movie> &search, int type) {

	system("CLS");
	logo();
	search.clear();
	string str;
	bool esc = true;

	color(10);
	gotoxy(13, 12);
	cout << "******************************************************";
	gotoxy(11, 13);
	cout << "*                                                        *";
	gotoxy(10, 14);
	cout << "*                                                          *";
	gotoxy(11, 15);
	cout << "*                                                        *";
	gotoxy(13, 16);
	cout << "******************************************************";
	color(7);
	gotoxy(33, 22);
	cout << "Enter -> Search";
	gotoxy(60, 22);
	cout << "Escape -> Back";


	gotoxy(32, 10);
	if(type == 0) {
		cout << "Enter Movie Name";
	}else {
		cout << "Enter Cinema Name";
	}
	gotoxy(30, 16);
	getIn(str, 54, 13, 14, 5, 22, esc, "str");
	if(!esc) {
		menu(data);
		return;
	}

	for(int i = 0; i < (int)data.size(); i++) {
		if(type == 0) {
			if(data[i].Name == str) {
				search.push_back(data[i]);
			}
		}else {
			if(data[i].cinema == str) {
				search.push_back(data[i]);
			}
		}
	}
	ListMovie(data, search, true);

	return;
}

void deleteMovie(vector<Movie> &data, bool quick = true, string name = "", string cinema = "") {

	if(quick) {
		system("CLS");
		logo();
		bool esc = true;

		color(10);
		gotoxy(13, 9);
		cout << "*** Enter Movie Name *********************************";
		gotoxy(11, 10);
		cout << "*                                                        *";
		gotoxy(10, 11);
		cout << "*                                                          *";
		gotoxy(11, 12);
		cout << "*                                                        *";
		gotoxy(13, 13);
		cout << "******************************************************";
		
		color(10);
		gotoxy(13, 15);
		cout << "*** Enter Cinema Name ********************************";
		gotoxy(11, 16);
		cout << "*                                                        *";
		gotoxy(10, 17);
		cout << "*                                                          *";
		gotoxy(11, 18);
		cout << "*                                                        *";
		gotoxy(13, 19);
		cout << "******************************************************";

		color(7);
		gotoxy(33, 22);
		cout << "Enter -> Search";
		gotoxy(60, 22);
		cout << "Escape -> Back";

		getIn(name, 54, 13, 11, 5, 22, esc, "str");
		if(!esc) {
			menu(data);
			return;
		}
		getIn(cinema, 54, 13, 17, 5, 22, esc, "str");
		if(!esc) {
			menu(data);
			return;
		}


	}
	bool found = false;
	system("CLS");
	logo();

	for(int i = 0; i < (int)data.size(); i++) {
		if(data[i].Name == name && data[i].cinema == cinema) {
			data.erase(data.begin() + i);
			found = true;
			break;
		}
	}

	if(found) {

		gotoxy(31, 15);
		color(10);
		cout << "Done Successfully";
		gotoxy(36, 19);
		color(15);
		cout << "\4 Back";
		gotoxy(0, 0);
		char m;
		while((m = _getch())) {
			if(m == ENTER || m == ESCAPE) {
				break;
			}
		}
		menu(data);

	}else {

		gotoxy(31, 15);
		color(12);
		cout << "Couldn't Find Data";
		gotoxy(36, 19);
		color(15);
		cout << "\4 Back";
		gotoxy(0, 0);
		char m;
		while((m = _getch())) {
			if(m == ENTER || m == ESCAPE) {
				break;
			}
		}
		menu(data);
	}

	return;
}

void updateMovie(vector<Movie> &data, bool quick = true, string name = "", string cinema = "") {

	if(quick) {
		system("CLS");
		logo();
		bool esc = true;

		color(10);
		gotoxy(13, 9);
		cout << "*** Enter Movie Name *********************************";
		gotoxy(11, 10);
		cout << "*                                                        *";
		gotoxy(10, 11);
		cout << "*                                                          *";
		gotoxy(11, 12);
		cout << "*                                                        *";
		gotoxy(13, 13);
		cout << "******************************************************";
		
		color(10);
		gotoxy(13, 15);
		cout << "*** Enter Cinema Name ********************************";
		gotoxy(11, 16);
		cout << "*                                                        *";
		gotoxy(10, 17);
		cout << "*                                                          *";
		gotoxy(11, 18);
		cout << "*                                                        *";
		gotoxy(13, 19);
		cout << "******************************************************";

		color(7);
		gotoxy(33, 22);
		cout << "Enter -> Search";
		gotoxy(60, 22);
		cout << "Escape -> Back";

		getIn(name, 54, 13, 11, 5, 22, esc, "str");
		if(!esc) {
			menu(data);
			return;
		}
		getIn(cinema, 54, 13, 17, 5, 22, esc, "str");
		if(!esc) {
			menu(data);
			return;
		}

	}

	bool found = false;
	int idx;
	system("CLS");
	logo();

	for(int i = 0; i < (int)data.size(); i++) {
		if(data[i].Name == name && data[i].cinema == cinema) {
			idx = i;
			found = true;
			break;
		}
	}

	if(found) {
		doUpdate(data, idx);
	}else {

		gotoxy(31, 15);
		color(12);
		cout << "Couldn't Find Data";
		gotoxy(36, 19);
		color(15);
		cout << "\4 Back";
		gotoxy(0, 0);
		char m;
		while((m = _getch())) {
			if(m == ENTER || m == ESCAPE) {
				break;
			}
		}
		menu(data);
	}
	return;
}

void doUpdate(vector<Movie> &data, int idx) {

	Movie m = data[idx];
	string temp;
	bool esc = true;
	system("CLS");
	logo();
	color(10);
	gotoxy(4, 8);	cout << "________________________________________________________________________";
	gotoxy(3, 9);	cout << "|              |                                                         |";
	gotoxy(3, 10);	cout << "|  Movie Name  |                                                         |";
	gotoxy(3, 11);	cout << "|______________|_________________________________________________________|";
	gotoxy(3, 12);	cout << "|              |                                                         |";
	gotoxy(3, 13);	cout << "|    Cinema    |                                                         |";
	gotoxy(3, 14);	cout << "|______________|_________________________________________________________|";
	gotoxy(3, 15);	cout << "|              |                                                         |";
	gotoxy(3, 16);	cout << "|     Price    |                                                         |";
	gotoxy(3, 17);	cout << "|______________|_________________________________________________________|";
	gotoxy(3, 18);	cout << "|              |                                                         |";
	gotoxy(3, 19);	cout << "|      Qt.     |                                                         |";
	gotoxy(3, 20);	cout << "|______________|_________________________________________________________|";
	color(7);
	gotoxy(57, 22);	cout << "ESCAPE -> Back";

	gotoxy(20,10);	cout << m.Name;
	gotoxy(20, 13);	cout << m.cinema;
	gotoxy(20, 16);	cout << m.price;
	gotoxy(20, 19); cout << m.quantity;
	getIn(m.Name, 54, 20, 10, 8, 22, esc, "str");
	if(!esc) {
		menu(data);
		return;
	}
	getIn(m.cinema, 54, 20, 13, 8, 22, esc, "str");
	if(!esc) {
		menu(data);
		return;
	}
	temp = toStr(m.price);
	getIn(temp, 8, 20, 16, 8, 22, esc, "int");
	if(!esc) {
		menu(data);
		return;
	}
	m.price = toFloat(temp);
	temp.clear();
	temp = toStr(m.quantity);
	getIn(temp, 8, 20, 19, 8, 22, esc, "int");
	if(!esc) {
		menu(data);
		return;
	}
	m.quantity = toInt(temp);
	data[idx] = m;
	
	system("CLS");
	logo();
	gotoxy(31, 15);
	color(10);
	cout << "Done Successfully";
	gotoxy(36, 19);
	color(15);
	cout << "\4 Back";
	gotoxy(0, 0);
	char c;
	while((c = _getch())) {
		if(c == ENTER || c == ESCAPE) {
			break;
		}
	}
	menu(data);

	return;
}

///////////////////////////////////////////////// STYLE /////////////////////////////////////////////////

void borders(int x, int y) {

	gotoxy(0, 0);
	color(15);
	for(int i = 0; i <= x; i++)	cout << "#";
	for(int i = 0; i < y; i++) {
		gotoxy(0, i);
		cout << "#";
		gotoxy(x, i);
		cout << "#";
	}
	gotoxy(0, y);
	for(int i = 0; i <= x; i++)	cout << "#";
	return;
}

void logo() {

	borders(79, 24);
	gotoxy(18, 2);
	cout << "####   ######  ##  ##  ######  ##   ##   ####";
	gotoxy(17, 3);
	cout << "##  ##    ##    ### ##  ##      ### ###  ##  ##";
	gotoxy(17, 4);
	cout << "##        ##    ## ###  ####    ## # ##  ######";
	gotoxy(17, 5);
	cout << "##  ##    ##    ##  ##  ##      ##   ##  ##  ##";
	gotoxy(18, 6);
	cout << "####   ######  ##  ##  ######  ##   ##  ##  ##";

	return;
}

void menu(vector<Movie> &data) {
	
	system("CLS");
	logo();
	char ch;
	int op = 0;

	gotoxy(30, 13);
	color(15);
	cout << "View Movies data";
	gotoxy(30, 14);
	cout << "Search Movies";
	gotoxy(30, 15);
	cout << "Add New Movie";
	gotoxy(30, 16);
	cout << "Update Movies";
	gotoxy(30, 17);
	cout << "Delete Movies";
	gotoxy(30, 18);
	cout << "Exit";

	gotoxy(28, 13 + op);
	cout << "\4";
	gotoxy(0, 0);
	while((ch = _getch())) {

		if(ch < 0) {
			ch = _getch();

			if(ch == UP) {
				gotoxy(28, 13 + op);
				cout << " ";
				if(op > 0) {
					op--;
				}else {
					op = 5;
				}
			}else if(ch == DOWN) {
				gotoxy(28, 13 + op);
				cout << " ";
				if(op < 5) {
					op++;
				}else {
					op = 0;
				}
			}
			gotoxy(28, 13 + op);
			cout << "\4";
			gotoxy(0, 0);
		}else if(ch == ESCAPE) {
			op = 5;
			break;
		}else if(ch == ENTER) {
			break;
		}
	}

	if(op == 0) {
		ListMovie(data, search, false);
	}else if(op == 1) {
		SearchMovie(data, search);
	}else if(op == 2) {
		addMovie(data);
	}else if(op == 3) {
		//Update(data, true, "", "");
		updateMovie(data, true);
	}else if(op == 4) {
		deleteMovie(data, true);
	}else if(op == 5) {
		saveNexit(data, file);
	}

	return;
}

