#include "cubic.hpp"
void Cubic::downCross(){ 			
	int tarPos = 24;
	for (int frontFace = 2;frontFace < 6;frontFace++){					//枚举归为
		int target;
		cerr << "DownCross check color " << frontFace << " ..."<< endl;
		A.Face(frontFace,WHITE);
		if (B[tarPos].check()) continue;
		target = findTarget(tarPos);
		//如果在底面但是没有归为的话
		//接下来这段程序处理了白色方块在顶面或者底面的情况
		cerr << "find Target Position .. " << target << endl;
		if (9 <= target && target <= 16){
			if (target == 14) A.rotate("F");
			else if (target == 16) A.rotate("F'");
			else{
				A.rotate("D"), A.rotate("D");
				if (target == 9) A.rotate("B'");
				else if (target == 11) A.rotate("B");
				A.rotate("D"), A.rotate("D");
			}
			target = findTarget(tarPos);
		}
		if (target > 16 && B[target].getColorFace(WHITE) == WHITE){		//虽然白色时正确的但是另一个颜色不对，我们先把他放到２４这个位置在通过两次前变换换上去
			A.Face(B[target].getColorFace(frontFace),WHITE);
			A.rotate("F");
			A.rotate("F");
			A.Face(frontFace,WHITE);
			target = findTarget(24);
		}
		if (target < 9 && B[target].getColorFace(WHITE) == YELLOW){		//将白色方框放到７这个位置在一举转下来
			while (findTarget(tarPos) != 7)
				A.rotate("U");
			A.rotate("F"),A.rotate("F");
			continue;													//这里处理的可能出现的情况１
		}					
		
		if (target > 16 && B[target].getColorFace(WHITE != WHITE)){
			A.Face(B[target].getColorFace(WHITE),WHITE);
			A.rotate("F");
			A.rotate("F");
			A.Face(frontFace,WHITE);
			target = findTarget(tarPos);
		}
		while (target != 7){
			A.rotate("U");
			target = findTarget(tarPos);
		}
		A.rotate("F");A.rotate("D"); A.rotate("R'"); A.rotate("D'");	
	}
}

void Cubic::downFace(){							//simple test passed
	int tarPos = 25;				//我们总是通过旋转放到25位置上	
	int rightFace, target; 			//右面的颜色
	for (int frontFace = 2; frontFace < 6; frontFace++){
		//找到rightFace
		rightFace = A.Face(frontFace,0);
		target = findTarget(tarPos);
		if (target == tarPos && B[target].check()) continue;	
		//首先处理白色面在黄色的方式。
		if (target < 9 && B[target].getColorFace(WHITE) == YELLOW){
			while (target != 8){
				A.rotate("U");
				target = findTarget(tarPos);
			}
			A.rotate("F'");
			A.rotate("U'");
			A.rotate("F");
			target = findTarget(tarPos);
		}
		
		//处理在底下的情况
		if (target > 9){
			int cnt = 0;
			while (target != 25){
				A.rotate("D");
				target = findTarget(tarPos);
				cnt++;
			}
			if (B[25].getColorFace(WHITE) == rightFace){
				A.rotate("F'");
				A.rotate("U");
				A.rotate("F");
			}else{
				A.rotate("R");
				A.rotate("U'");
				A.rotate("R'");
			}
			while (cnt--)
				A.rotate("D'");
			target = findTarget(tarPos);
		}
		//处理最后一种情况可以一步回到正确的位置
		if (target < 9){
			while (target != 8){
				A.rotate("U");
				target = findTarget(tarPos);
			}
			if (B[8].getColorFace(WHITE) == frontFace){
				A.rotate("F'");
				A.rotate("U'");
				A.rotate("F");
			}else if (B[8].getColorFace(WHITE) == rightFace){
				A.rotate("R");
				A.rotate("U");
				A.rotate("R'");
			}
		}
	}
}

void Cubic::secondLevel(){
	string Eq1[6] = {"U","R","U'","R'"};
	string Eq2[6] = {"U'","F'","U","F"};
	int rightFace,tarPos = 16;
	for (int frontFace = 2; frontFace < 6;frontFace++){
		rightFace = A.Face(frontFace,0);
		int target = findTarget(tarPos);
		if (target == tarPos && B[target].check()) continue;
		//如果不在合法的位置里面那么就把它弄出来
		if (target > 8){
			int col1 = B[target].getCenterColor(1);
			int col2 = B[target].getCenterColor(2);
			if (A.Face(col1,0) != col2){
				int cur = A.Face(col2,0);
				if (cur != col1){
					cerr << "WRONG\nWRONG\nWRONG!!!!";
				}
			}
			A.useEquation(Eq1);
			A.useEquation(Eq2);
			A.Face(frontFace,0);
			target = findTarget(tarPos);
		}
		//弄进合法的位置去
		while (!((target == 5 && B[target].getColorFace(rightFace) == rightFace) || 
				 (target == 7 && B[target].getColorFace(frontFace) == frontFace))){
					A.rotate("U");
					target = findTarget(tarPos);
		}
		
		if (target == 5 && B[target].getColorFace(rightFace) == rightFace){
			A.useEquation(Eq2), A.useEquation(Eq1);
		}
		if (target == 7 && B[target].getColorFace(frontFace) == frontFace){
			A.useEquation(Eq1), A.useEquation(Eq2);
		}
	}
	
}

void Cubic::upCross(){
	string Eq[10] = {"F","U","R","U'","R'","F'"};
	int pos_list[4] = {1,3,5,7};
	int cnt = 0;
	int rightFace = A.Face(2,0);
	for (int i = 0;i < 4;i++){
		if (B[pos_list[i]].getColorFace(YELLOW) == YELLOW) cnt++;
	} 
	if (cnt == 4) return;
	if (cnt == 0){				//没有归位用一边公式摆成一条直线形式
		A.useEquation(Eq);
		upCross();
	}
	if (cnt == 2){
		if (B[1].getColorFace(YELLOW) == YELLOW && B[7].getColorFace(YELLOW) == YELLOW){
			A.Face(rightFace,0);
		}
		if (B[3].getColorFace(YELLOW) == YELLOW && B[5].getColorFace(YELLOW) == YELLOW){
			A.useEquation(Eq);
			upCross();
			return;
		}
		while (!(B[1].getColorFace(YELLOW) == YELLOW && B[3].getColorFace(YELLOW) == YELLOW)){
			rightFace = A.Face(rightFace,0);
		}
		if (B[1].getColorFace(YELLOW) == YELLOW && B[3].getColorFace(YELLOW) == YELLOW){
			A.useEquation(Eq);
			upCross();
			return;
		}
	}
}

void Cubic::upCrossCorrect(){
	string Eq[20] = {"R","U","R'","U","R","U","U","R'"};
	int pos_list[4] = {1,3,5,7};
	int cnt = 0;
	int rightFace = A.Face(2,0);
	for (int i = 0;i < 4;i++){
		if (B[pos_list[i]].check()) cnt++;
	}
	
	if (cnt == 4){
		return;
	}
	
	if (cnt == 1){
		while (!B[7].check())
			rightFace = A.Face(rightFace,0);
		A.useEquation(Eq);
		upCrossCorrect();
		return;
	}
	
	if (cnt == 2){
		if (B[1].check() && B[7].check()){
			A.Face(rightFace,0);
			A.useEquation(Eq);
			upCrossCorrect();
			return;
		}else if (B[3].check() && B[5].check()){
			A.useEquation(Eq);
			upCrossCorrect();
			return;
		}else{
			A.rotate("U");
			upCrossCorrect();
			return;
		}
	}
	
	if (cnt == 0){
		A.rotate("U");
		upCrossCorrect();
	}
}

void Cubic::upFace(){
	int pos_list[4] = {0,2,6,8};
	int cnt = 0, rightFace = A.Face(2,0);
	string Eq[20] = {"R'","U","L","U'","R","U","L'","U'"};
	for (int i = 0;i < 4;i++){
		if (B[pos_list[i]].getColorFace(YELLOW) == YELLOW) cnt++;
	}
	if (cnt == 4) return;
	if (cnt == 1){
		while (B[0].getColorFace(YELLOW) != YELLOW)
			rightFace = A.Face(rightFace,0);
		A.useEquation(Eq);
		upFace();
		return;
	}
	
	if (cnt == 2){
		while (B[0].getColorFace(YELLOW) == YELLOW)
			rightFace = A.Face(rightFace,0);
		A.useEquation(Eq);
		upFace();
		return;
	}
	if (cnt == 0){
		useEquation(Eq);
		upFace();
		return;
	}
}

void Cubic::finish(){
	string Eq[20] = {"L","F","R'","U","L","U'","R","U","L'","U'","F'","L'"};
	int pos_list[4] = {0,2,6,8};
	int cnt = 0, rightFace = A.Face(2,0);
	for (int i = 0;i < 4;i++){
		if (B[pos_list[i]].check()) cnt++;
	}
	if (cnt == 4) return;
	if (cnt == 1){
		while (!B[8].check()) rightFace = A.Face(rightFace,0);
		useEquation(Eq);
		finish();
		return;
	}
	if (cnt == 0){
		useEquation(Eq);
		finish();
		return;
	}
}

int main()
{
	freopen("output.txt","w",stdout);
	Cubic_init("question"); 
	A.downCross();			cerr << "DownCross OK" << endl;
	A.downFace();			cerr << "downFace OK" << endl;
	A.secondLevel();		cerr << "secondLevel OK" << endl;
	A.upCross();			cerr << "upCross OK" << endl;
	A.upCrossCorrect();		cerr << "upCrossCorrect OK " << endl;
	A.upFace();				cerr << "upFace OK" << endl;
	A.finish();				cerr << "Finished !!!" << endl;
	
	A.show();
	return 0;
}
