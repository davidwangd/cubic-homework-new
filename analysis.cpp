#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstdlib>
using namespace std;
char a[1000];
char b[1000];
int k=0;
long long n;
void Short (char *a,char *b)
{
    n=strlen(a);
    int i=0;
    while (i<n)
    {
        if(a[i]==a[i+1]&&a[i+2]==a[i])
        {
            b[k]=a[i+2];
            b[k+1]='\'';
            k=k+2;
            i=i+3;
            //flag=flag+1;
        }
        else if(a[i]==a[i+2]&&a[i+4]==a[i+2]&&a[i+1]=='\''&&a[i+3]=='\''&&a[i+5]=='\'')
        {
            b[k]=a[i];
            k=k+1;
            i=i+6;
            //flag=flag+1;
        }
        else if(a[i]==a[i+2]&&a[i+1]=='\'')
        {
            i=i+3;
        }
        else if(a[i]==a[i+1]&&a[i+2]=='\'')
        {
            i=i+3;
        }
        else if(a[i]=='U'&&(a[i+1]=='D')&&a[i+2]=='U'&&a[i+3]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+4;
            //flag=flag+1;
        }
        else if(a[i]=='U'&&a[i+1]=='D'&&a[i+2]=='\''&&a[i+3]=='U'&&a[i+4]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+5;
            //flag=flag+1;
        }
        else if(a[i]=='L'&&(a[i+1]=='R')&&a[i+2]=='L'&&a[i+3]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+4;
            //flag=flag+1;
        }
        else if(a[i]=='L'&&a[i+1]=='R'&&a[i+2]=='\''&&a[i+3]=='L'&&a[i+4]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+5;
            //flag=flag+1;
        }
        else if(a[i]=='F'&&a[i+1]=='B'&&a[i+2]=='F'&&a[i+3]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+4;
            //flag=flag+1;
        }
        else if(a[i]=='F'&&a[i+1]=='B'&&a[i+2]=='\''&&a[i+3]=='F'&&a[i+4]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+5;
            //flag=flag+1;
        }
        else if(a[i]=='D'&&a[i+1]=='U'&&a[i+2]=='D'&&a[i+3]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+4;
            //flag=flag+1;
        }
        else if(a[i]=='D'&&a[i+1]=='U'&&a[i+2]=='\''&&a[i+3]=='D'&&a[i+4]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+5;
            //flag=flag+1;
        }
        else if(a[i]=='R'&&a[i+1]=='L'&&a[i+2]=='R'&&a[i+3]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+4;
            //flag=flag+1;
        }
        else if(a[i]=='R'&&a[i+1]=='L'&&a[i+2]=='\''&&a[i+3]=='R'&&a[i+4]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+5;
            //flag=flag+1;
        }
        else if(a[i]=='B'&&a[i+1]=='F'&&a[i+2]=='B'&&a[i+3]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+4;
            //flag=flag+1;
        }
        else if(a[i]=='B'&&a[i+1]=='F'&&a[i+2]=='\''&&a[i+3]=='B'&&a[i+4]=='\'')
        {
            b[k]=a[i+1];
            k=k+1;
            i=i+5;
            //flag=flag+1;
        }
        else {b[k]=a[i];k=k+1;i=i+1;}
    }
    b[k] = 0;
}
int main ()
{
    int number=1;
    cin>>a;
    Short(a,b);
    long long stra;
    stra=strlen(a);
    long long strb;
    strb=strlen(b);
    while(stra!=strb)
    {
        cout<<number<<endl;
        if(number%2!=0)
        {
            for(int i=0;i<stra;i++){a[i]=0;}
            Short(b,a);
            stra=strlen(a);
            strb=strlen(b);
            cout << b << endl << a << endl;
            number=number+1;
        }
        else if(number%2==0)
        {
            for(int i=0;i<strb;i++){b[i]=0;}
            Short(a,b);
            stra=strlen(a);
            strb=strlen(b);
            cout << a << endl << b << endl;
            number=number+1;
        }
        if(stra==strb) cout<<"OK";
    }
    cout<<strlen(b)<<endl;
    cout<<b;
}
