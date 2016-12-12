#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include <cstring>
using namespace std;
template<typename Tp,int Height,int Width>
struct	Matrix{
	Tp x[Height][Width];int notAMatrix;
public:
	Tp get(int i,int j) const { return x[i][j]; }
	Tp set(int i,int j,Tp k){ return x[i][j] = k; }
	Tp* operator[](const int a) { return x[a]; }
	Matrix(){
		memset(x,0,sizeof(x));
		notAMatrix = 0;
	};
	Matrix(const char *s){
		if (strcmp(s,"notAMatrix") == 0) notAMatrix = 1;
	}
	Matrix(Tp d){
		notAMatrix = 0;
		for (int i = 0;i < Height;i++)
			for (int j = 0;j < Width;j++)
				x[i][j] = d * Tp(i==j);
	}
	Matrix(const Matrix<Tp,Height,Width> &a){
		notAMatrix = 0;
		for (int i = 0;i < Height;i++)
			for (int j = 0;j < Width;j++)
				this -> x[i][j] = a.get(i,j);
	}
	~Matrix() = default;
	Matrix(Tp a[Height][Width]);
	void print() const {
		if (notAMatrix){
			cout << "By somehow this is not a valid Matrix!!!!" << endl;
		}
		for (int i = 0;i < Height;i++){
			for (int j = 0;j < Width;j++)
				cout << this->get(i,j) <<" ";
			cout << endl;
		}
	}
	int valid()const { return !notAMatrix; }
	Matrix<Tp,Height,Width> invert() const;
	Tp Det() const;
};

template<typename Tp,int n,int m,int k>
Matrix<Tp,n,k> operator*(const Matrix<Tp,n,m> &a,const Matrix<Tp,m,k> &b){
	Matrix<Tp,n,k> ret(Tp(0));
	if (!a.valid() || !b.valid()){
		cerr << "You want to mulitpy invalid Matrix!!!" << endl;
	}
	for (int i = 0;i < n;i++){
		for (int j = 0;j < k;j++){
			for (int t = 0; t < m;t++){
				ret[i][j] = ret[i][j] + a.get(i,t) * b.get(t,j);
 			}
		}
	}
	return ret;
}

template<typename Tp,int n,int m>
Matrix<Tp,n,m> operator+(const Matrix<Tp,n,m> &a,const Matrix<Tp,n,m> &b){
	Matrix<Tp,n,m> ret(0);
	if (!a.valid() || !b.valid()){
		cerr << "You want to mulitpy invalid Matrix!!!" << endl;
	}
	for (int i = 0;i < n;i++)
		for (int j = 0;j < m;j++)
			ret[i][j] = a.get(i,j) + b.get(i,j);
}

template<typename Tp,int n,int m>
Matrix<Tp,n,m>& operator+=(Matrix<Tp,n,m> &a,const Matrix<Tp,n,m> &b){
	if (!a.valid() || !b.valid()){
		cerr << "You want to mulitpy invalid Matrix!!!" << endl;
	}
	for (int i = 0;i < n;i++)
		for (int j = 0;j < m;j++)
			a[i][j] = a[i][j] + b.get(i,j);
	return a;
}

template<typename Tp,int n,int m>
Matrix<Tp,n,m>::Matrix(Tp a[n][m]){
	this -> notAMatrix = 0;
	for (int i = 0;i < n;i++)
		for (int j = 0;j < m;j++)
			this -> x[i][j] = a[i][j];
}

template<typename Tp,int n,int m>
Matrix<Tp,n,m> Matrix<Tp,n,m>::invert() const{
	if (n!=m){
		cerr << "You can not calculate the invert of a not squared Matrix!!" << endl;
		return Matrix<Tp,n,m>("notAMatrix");
	}
	Matrix<Tp,n,m> cur(*this),ret(1);
	for (int i = 0;i < n;i++){
		int now = -1;
		for (int j = i;j < n;j++)
			if (cur[j][i] != Tp(0)){
				now = j;
				break;
			}
		if (now == -1){ return Matrix<Tp,n,m>("notAMatrix"); }
		for (int j = 0;j < n;j++) swap(cur[i][j],cur[now][j]),swap(ret[i][j],ret[now][j]);

//		cout << cur << endl << ret << "finished a step \n\n" << endl;

		Tp num = cur[i][i];
		for (int j = 0;j < n;j++) cur[i][j] = cur[i][j] / num, ret[i][j] = ret[i][j] / num;
		for (int k = i + 1;k < n;k++){
			Tp num = cur[k][i];
			for (int j = 0;j < n;j++)
				cur[k][j] = cur[k][j] - cur[i][j] * num, ret[k][j] = ret[k][j] - ret[i][j] * num;

//			cout << cur << endl << ret << "finished a step \n\n" << endl;
		}

//		cout << cur << endl << ret << "finished a step \n\n" << endl;

	}

	for (int i = n - 1;i >= 0;i--)
		for (int j = 0;j < i;j++){
			Tp num = cur[j][i];
			for (int k = 0;k < n;k++)
				cur[j][k] = cur[j][k] - cur[i][k] * num,
				ret[j][k] = ret[j][k] - ret[i][k] * num;

		}
//	cout << cur << endl << ret << "finished a step \n\n" << endl;
	return ret;
}

template<typename Tp,int n,int m>
Tp Matrix<Tp,n,m>::Det() const{
	if (n != m){
		cerr << "You can not calculate the Count with a not squared Matrix!!" << endl;
	}
	Matrix<Tp,n,m> X = *this;
	Tp f(1);
	Tp ret = Tp(1);
	for (int i = 1;i < n;i++){
		int cur = -1;
		for (int j = i;j < n;j++){
			if (X.x[j][i] != Tp(0)){
				cur = j;
				break;
			}
		}
		if (cur != i){
			f = -f;
			for (int j = i;j < n;j++) swap(X[i][j],X[cur][j]);
		}
		Tp num = X[i][i];
		f = f * num;
		for (int j = i;j < n;j++) X[i][j] = X[i][j] / num;
		for (int k = i+1;k < n;k++){
			Tp num = X[k][i];
			for (int j = i;j < n;j++)
				X[k][j] = X[k][j] - X[i][j] * num;
		}
	}
	return f;
}

template<typename Tp,int n,int m>
int operator==(const Matrix<Tp,n,m> &a,const Matrix<Tp,n,m> &b){
	for (int i = 0;i < n;i++)
		for (int j = 0;j < m;j++)
			if (a.get(i,j) != b.get(i,j)) return 0;
	return 1;
}

template<typename Tp,int n,int m>
int operator!=(const Matrix<Tp,n,m> &a,const Matrix<Tp,n,m> &b){
	return !(a == b);
}

template<typename Tp,int n,int m>
ostream& operator<<(ostream &os,const Matrix<Tp,n,m> &A){
	if (!A.valid()){
		os << "Not A Matrix";
	}else{
		for (int i = 0;i < n;i++){
			for (int j = 0;j < m;j++)
				os << A.get(i,j) << " ";
			os << "\n";
		}
	}
}
#endif
