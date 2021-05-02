#include <iostream>
#include <fstream>

using namespace std;

enum fieldState {Player1 , Player2, empty};

struct GameState {
	int rowCount, columnCount;
	fieldState** Field;
	fieldState Nextstep;
};

void PrintGame(GameState& g) {
	for(int row=0; row < g.rowCount; row++) {	
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

void InitGame(GameState& g, int rowCount, int columnCount) {
	cout << "Initializing game...\n";
	g.Field = NULL;	
	g.Nextstep = Player1;
	g.Field = new fieldState*[rowCount];
	for(int row=0; row < rowCount; row++) {
		g.Field[row] = new fieldState[columnCount];
		for(int col=0; col < columnCount; col++) {
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

int main(int argc, char** argv) {
	cout << "Connect4 game\n";
	GameState game;
       	game.rowCount = 6;
	game.columnCount = 7;
	cout << game.rowCount << " X " << game.columnCount << endl;
	InitGame(game, game.rowCount, game.columnCount);
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
	Destructor(game);	
	return 0;
}
