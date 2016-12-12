#include "Matrix.hpp"
#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;
// frame work
// This is just a frame work code
/* Face : 0 for front  1 for back
		  2 for up     3 for down
		  4 for left   5 for right
	Color:  0 for write 1 for yellow
			2 for ret 	3 for oragne
			4 for blue  5 for green
	numbered : 按照展开面的左上角为０号右下角为8号编号
			　　每个块的编号时Face 编号　* 9 + 自身编号
*/
#include "cubic.hpp"
void Cubic::downCross(){}
void Cubic::downFace(){}
void Cubic::secondLevel(){}
void Cubic::upCross(){}
void Cubic::upCrossCorrect(){}
void Cubic::upFace(){}
void Cubic::finish(){}

int main()
{
	freopen("question","w",stderr);
	srand(time(0));
	Cubic_init("standard");
	string cmd[30] = {"U","U'","F","F'","B","B'","R","R'","D","D'","L","L'"};
	for (int i = 1;i <= 15;i++){
		int x = rand()%12;
		A.rotate(cmd[x]);
	}
	A.show();
	for (int i = 0;i < 54;i++){
		if (i%3 == 0) fprintf(stderr,"\n");
		fprintf(stderr,"%d",A.ordianary[i][0]); 
	}
	fprintf(stderr, "\n");
}

