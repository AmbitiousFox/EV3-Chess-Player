// records a single move and outputs a record of moves made into output file, fout.
void recordMove(fout, string moves_x[], string moves_y[], string move, int & moveNum)
{
	bool first = 1;
	for(char & c: move){
		if(first = 1){
			string moves_x[moveNum] = c;
			first = 0;
		}
		else{
			string moves_y[moveNum] = c;
		}
	}
	fout << moves_x << endl << moves_y << endl;
}





// reads moves from an input file and returns the moves in chronologcial order
string readMoves(fin){
	string move = "";
	string x = "";
	string y = "";
	string moves_x[] = {};
	string moves_y[] = {};
	fin >> moves_x >> moves_y;
	for(int i = 0; i < ()sizeof(moves_y)/2); i++)
	move + moves_x[i] + moves_y[i];
	return move;
}








