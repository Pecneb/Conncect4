#include <iostream>
#include <fstream>

using namespace std;

enum fieldState {empty, Player1 , Player2};

struct GameState {
	int rowCount, columnCount;
	fieldState** Field;
	fieldState Nextstep;
};

void PrintGame(GameState& g) {
	cout << "  ";
	for(int i=0; i<g.columnCount; i++) {
		cout << char('A'+i);
	}
	cout << endl;
	for(int row=0; row < g.rowCount; row++) {
		if(row<9) cout << "0" << (row+1);
		else cout << (row+1);	
		for(int col=0; col < g.columnCount; col++) {
			switch(g.Field[row][col]) {
				case Player1:
					cout << "O";
					break;
				case Player2:
					cout << "X";
					break;
				default:
					cout << ".";
			}
		}
		cout << endl;
	} 
}

void InitGame(GameState& g) {
	cout << "Initializing game...\n";
	g.Field = NULL;	
	g.Nextstep = Player1;
	bool goodinit = true;
	do {
		cout << "Rows: "; cin >> g.rowCount; cout << endl;
		cout << "Columns: "; cin >> g.columnCount; cout << endl;
		if(g.rowCount < 6 || g.rowCount > 20) { 
			cout << "A soroknak 6 es 20 koze kell esniuk" << endl;
			goodinit = false;
		} else {
			goodinit = true;
		}
		if(g.columnCount < 6 || g.columnCount > 20) {
			cout << "Az oszlopoknak 6 es 20 koze kell esniuk" << endl;
			goodinit = false;
		} else {
			goodinit = true;
		}
	} while(!goodinit);
	g.Field = new fieldState*[g.rowCount];
	for(int row=0; row < g.rowCount; row++) {
		g.Field[row] = new fieldState[g.columnCount];
		for(int col=0; col < g.columnCount; col++) {
			g.Field[row][col] = empty;
		}
	}
}

void Destructor(GameState& g) {
	cout << "Exiting game..." << endl;
	cout << endl;
	for(int row=0; row < g.rowCount; row++) {
		delete[] g.Field[row];	
	}
	delete[] g.Field;
}

bool GoodStep(GameState& g, char col) {
	if(col < 'A' || col > 'A'+ g.columnCount) return false;
	if(g.Field[0][col - 'A'] != empty) return false;
	return true;
}

void MakeStep(GameState& g, char col) {
	for(int row=g.rowCount-1; row >= 0; row--) {
		if(g.Field[row][col-'A'] == empty) {
			g.Field[row][col-'A'] = g.Nextstep;
			break;
		}
	}
	if(g.Nextstep == Player1) g.Nextstep = Player2;
	else g.Nextstep = Player1;
}

bool Win(GameState& g) {
	int stepcount;
	int emptycount;
	fieldState state = Player1;
	for(int col=0; col<g.columnCount; col++) {
		stepcount = 0;
		for(int row=g.rowCount-1; row>=0; row--) {
			if(stepcount == 4) return true;
			if(g.Field[row][col] == state) {
				stepcount++;
			} else if(g.Field[row][col] != empty) {	
				state = g.Field[row][col];
				stepcount = 0;
			}
		}
	}
	return false;
}

bool GameOver(GameState& g) {
	int emptycount = 0;
	for(int row=0; row<g.rowCount; row++) {
		for(int col=0; col<g.columnCount; col++) {
			if(g.Field[row][col] == empty) {
				emptycount++;
			}
		}
	}
	if(emptycount == 0) {
		return true;
	}	
	return false;
}

// Still gotta implement saving the game to a text file!
void GameToText(GameState& g) {
	ofstream f("connect4.txt");
	if(f.is_open()) {
		f << g.rowCount << '\n';
		f << g.columnCount << '\n';
		for(int row=0; row<g.rowCount; row++) {
			for(int col=0; col<g.columnCount; col++) {
				f << g.Field[row][col];
			}
			f << '\n';
		}
	}
	f.close();
}

// This gotta be done by tomorrow, fuck this.
void TextToGame(GameState& g) {
	ifstream f("connect4.txt");
	char c;
	if(f.is_open()) {
		while(f.get(c)) cout << c;
	}
	f.close();
}

int main(int argc, char** argv) {
	GameState game;
	if(argv[1] == "betolt") {
		GameToText(game);
	} else {
		InitGame(game);
	}
	cout << "Connect4 game\n";
	cout << game.rowCount << " X " << game.columnCount << endl;
	PrintGame(game);
	char col;
	while(!Win(game)) {
	do{
		if(game.Nextstep==Player1) cout << "Player1";
		else cout << "Player2";
		cout << " lep, kerem az oszlopot: "; cin >> col; cout << endl;
	} while(!GoodStep(game, col));
	MakeStep(game, col);
	PrintGame(game);
	}
	if(game.Nextstep==Player1) cout << "Player2";
	else cout << "Player1";
	cout << " has won the game\n";
	GameToText(game);
	Destructor(game);	
	return 0;
}
