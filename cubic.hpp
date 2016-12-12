#include "Matrix.hpp"
#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
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
const int WHITE = 0;
const int YELLOW = 1;
const int RED = 2;
const int ORANGE = 3;
const int BLUE = 4;
const int GREEN = 5;
const int FRONT = 0;
const int BACK = 1;
const int UP = 2;
const int DOWN = 3;
const int LEFT = 4;
const int RIGHT = 5;
class Cubic;
class block;
/*
class center : public block{
public:
	virtual int type() override{
		return 0;
	}
};
class edge : public block{
public:
	virtual int type() override { return 1; }
};
class vertex : public block{
	virtual int type() override { return 2; }
};
*/
const int BlockNum = 26;
int get_id(int face_id,int block_id){ return face_id * 9 + block_id; }
class Cubic{
public:
	Matrix<int,54,1> ordianary,current;							//记录当前魔方，旋转过的魔方,记录这个块的内容
	Matrix<int,54,1> tracer;
	map<string,Matrix<int,54,54>> R;							//一个需要人工初始化的旋转－置换矩阵
	Matrix<int,54,54> Turn,inTurn; 		
							//一个根据面向当前面向方向的矩阵　满足　ordianary * Turn = current,current * inTurn = ordianary
	vector<string> ans;											//记录魔方答案
	block* bel[54];												//一个需要人工初始化的数组，表示每个位置指向的那个地方
	Cubic() = default;											//初始化内容,这里因为太庞大我们直接在外部初始化好了
	~Cubic() = default;
	void rotate(string cmd);									//旋转
	int Face(int col1,int col2);								//将魔方进行转置，将合适的颜色放过来
	int faceOrder[6][5];										//在front确定的前提下顺时针记录其他四个面的颜色
	//下面是求解操作
	void downCross();											// something tight and complicate
	void downFace();											// hard
	void secondLevel();											// quite easy
	void upCross();												// quite easy
	void upCrossCorrect();										// quite easy
	void upFace();												// quite easy
	void finish();												// very easy
	void setColor(int pos,int col);								//　设置pos位置的color
	int getColor(int pos) const;								//　询问pos位置的color
	int getColor(int face,int id) const;						//　询问face面id位的color
	int faceColor(int face) const ;								//　询问face面的color
	void show() const;											//　显示魔方
	void useEquation(string s[]);
}A;

class block{
	int pos[4];
	int face[4];
public:	
	int type;
	block(){
		memset(pos,0,sizeof(pos));
		memset(face,0,sizeof(face));
		type = 0;
	}
	~block() = default;
	int getColor(int id) const;					//获得这个块第i个块color面的值
	int getCenterColor(int face_id) const;		//获得这个块中心快为face_id的格子的color的值
	int getColorFace(int col) const;			//获得Color这个颜色的中心块的颜色值
	int getFaceColor(int col) const;			//获得中心快颜色为Color所对应的颜色值
	int check() const ;							//判断是否归位
	void add(int face_id,int num){
		type++;
		face[type] = face_id;
		pos[type] = num;
	}			
	void showColor()const {
		for (int i = 1;i <= type;i++) printf("%d",getColor(i));
		printf("\n");
	}
	void show() const{
		printf("Block Type : %d\n",type);
		for (int i = 1;i <= type;i++){
			printf("(%d,%d): %d\n",face[i],pos[i],getColor(i));
		}
		printf("show over!\n");
	}
	int hasColor(int col) const ;						
}B[27];

//findTarget: 寻找应该放在n这个位置的真正位置
int findTarget(int n){
	for (int i = 0;i < BlockNum;i++){
		if (B[i].type != B[n].type) continue;
		int flag = 1;
		for (int j = 1;j <= B[n].type;j++){
			if (!B[i].hasColor(B[n].getCenterColor(j))){
				flag = 0;
				break;
			}
		}
		if (flag) return i;
	}
	return -1;
}


int block::getColor(int id) const {
	if (id > type) return -1;
	return A.getColor(face[id],pos[id]);
}
int block::getCenterColor(int id) const{
	if (id > type) return -1;
	return A.getColor(face[id],4);
}
int block::check() const{
	for (int i = 1;i <= type;i++)
		if (getCenterColor(i) != getColor(i)) return 0;
	return 1;
}

int block::hasColor(int col) const{
	for (int i = 1;i <= type;i++)
		if (getColor(i) == col) return 1;
	return 0;
}

int block::getColorFace(int col) const{
	for (int i = 1;i <= type;i++){
		if (getColor(i) == col) return getCenterColor(i);
	}
	return -1;
}

int block::getFaceColor(int Face) const{
	for (int i = 1;i <= type;i++){
		if (getCenterColor(i) == Face) return getColor(i);
	}
	return -1;
}

void trans(Matrix<int,54,54> &cur,int x,int y){
	cur[x][x] = 0;
	cur[x][y] = 1;
}

void Cubic_init(char *start_file){
	//Initiallize array B from file Block
	
	for (int i = 0;i < 54;i++) A.tracer.set(i,0,i);
	
	FILE *f = fopen("Block","r");
	int a,b,c;
	for (int i = 0;i < 54;i++){
		fscanf(f,"%d%d%d",&a,&b,&c);
		B[a].add(b,c);
	#ifdef DEBUG
		cerr << a << " " << b << " " << c << endl;
	#endif
		A.bel[get_id(b,c)] = B + a;
	}
#ifdef DEBUG		
	cerr << "BlOCK OK!" << endl;
#endif
	char buff[200];
	f = fopen("Rotate","r");
	//Initiallize map R from file Rotate
	static int transPattern[8][2]; // = {{0,6},{1,3},{2,0},{3,7},{4,4},{5,1},{6,8},{7,5},{8,2}};
	for (int x = 0;x < 8;x++) fscanf(f,"%d%d",&transPattern[x][0],&transPattern[x][1]);
	for (int i = 0;i < 8;i++){
		fscanf(f,"%s%d",buff,&a);
		Matrix<int,54,54> cur = Matrix<int,54,54>(1);
		if (a != -1)
			for (int x = 0;x < 8;x++)
				trans(cur,get_id(a,transPattern[x][0]),get_id(a,transPattern[x][1]));
		for (int j = 0;j < 12;j++){
			int x,y;
			fscanf(f,"%d%d",&x,&y);
			trans(cur,x,y);
		}
		string now = buff;
		A.R[now] = cur;
		A.R[now + '\''] = cur.invert();
		if (cur * cur * cur != cur.invert()){
			cerr << now << " error" << endl;
		}
	#ifdef DEBUG
		cerr << "Rotate " << buff << " ok" << endl;
	#endif 	
	}
	
	f = fopen("faceOrder","r");
	for (int i = 0;i < 6;i++){
		for (int j = 0;j < 4;j++)
			fscanf(f,"%d",&A.faceOrder[i][j]);
		A.faceOrder[i][4] = A.faceOrder[i][0];
	}
	
	f = fopen(start_file,"r");
	static int arr[9];
	for (int i = 0;i < 6;i++){
		for (int j = 0;j < 9;j++) 
			fscanf(f,"%1d",arr+j);
		for (int j = 0;j < 9;j++)
			A.setColor(get_id(arr[4],j),arr[j]);
	}
	A.Turn = A.inTurn = Matrix<int,54,54>(1);
}

void Cubic::rotate(string cmd){
	Matrix<int,54,54> cur = R[cmd];
	current = cur * current;
	tracer = cur * tracer;
	cur = inTurn * cur * Turn;
	int flag = 1;
	for (map<string,Matrix<int,54,54>>::iterator it = R.begin();it != R.end();it++){
		if (it -> second == cur){
			ordianary = cur * ordianary;
			flag = 0;
			ans.push_back(it->first);
		}
	} 
	if (flag) cerr << "No correct Matching Found!!!" << endl;
}

int Cubic::getColor(int pos) const {
	return current.get(pos,0);
}

int Cubic::getColor(int face,int id) const {
	return current.get(face * 9 + id,0);
}

int Cubic::faceColor(int face) const{
	return current.get(face * 9 + 4,0);
}

void Cubic::setColor(int pos,int col){
	current[pos][0] = ordianary[pos][0] = col;
}

int Cubic::Face(int frontFace,int downFace){
	current = ordianary;
	Turn = Matrix<int,54,54>(1);
	if (faceColor(4) == downFace || faceColor(5) == downFace){
		Turn = R["U"] * R["UM"] * R["D'"] * Turn;
		current = Turn * ordianary;
		tracer = Turn * tracer;
	}
	while (faceColor(3) != downFace){
		Turn = R["L"] * R["LM"] * R["R'"] * Turn;
		current = Turn * ordianary;
		tracer = Turn * tracer;
	}
	while (faceColor(0) != frontFace){
		Turn = R["U"] * R["UM"] * R["D'"] * Turn;
		current = Turn * ordianary;
		tracer = Turn * tracer;
	}
	inTurn = Turn.invert();
	int rightFace;
	for (int i = 1;i <= 4;i++){
		if (faceOrder[frontFace][i] == downFace)
			return faceOrder[frontFace][i-1];
	}
}

void Cubic::show() const {
	cout << ans.size() << endl;
	for (auto x : ans){
		cout << x << " ";
	}
	cout << endl;
	for (int i = 0;i < 54;i++){
		if (i % 3 == 0) cout << endl;
		cout << ordianary.get(i,0);
	}
	cout << endl;
}

void Cubic::useEquation(string s[]){
	for (int i = 0;s[i] != string("");i++){
		rotate(s[i]);
	}
}

