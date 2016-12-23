#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <set>
#include <map>
using namespace std;
char s[6][20];
map<char,int> m;
int main()
{
	freopen("question","w",stderr);
	FILE *out = fopen("ColorTrans", "w");
	for (int i = 0;i < 6;i++){
		scanf("%s",s[i]);
	}
	m[s[0][4]] = 0;
	m[s[1][4]] = 1;
	m[s[2][4]] = 5;
	m[s[3][4]] = 4;
	m[s[4][4]] = 2;
	m[s[5][4]] = 3;
	for (int i = 0;i < 6;i++){
		for (int j = 0;s[i][j];j++){
			fprintf(stderr, "%d", m[s[i][j]]);
		}
		fprintf(stderr, "\n");
	}
	fclose(stderr);
	for (int i = 0;i < 6;i++){
		for (map<char,int>::iterator it = m.begin();it != m.end();it++){
			if (it -> second == i){
				fprintf(out, "%c", it -> first);
			}
		}
	}
	fclose(out);

	system("./cubic");
	freopen("output.txt","r",stdin);
	int n;
	scanf("%d",&n);
	char st[5];
	for (int i = 0;i < n;i++){
		scanf("%s", st);
		if (st[1] == '\'') st[1] = 'i';
		printf("%s ",st);
	}

//	system("./render");
	return 0;

}

/*
RRYOGOBOG
WBYYBYBWB
RWRBRGOBO
GWBBOYRRY
ORGOWRYYG
OGWGYWWGW
*/
/*
RYWYGOBYO
GWOGBBYGO
RBRWYRWBB
YGGWWOYGO
GRYWROWRB
WBGYORBOR
*/
