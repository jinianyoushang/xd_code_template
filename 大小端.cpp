#include<iostream>


using namespace std;

//����Ƿ���С�� 
int checkSystem(){
	union check{
		int i;
		char ch;
	}c;
	c.i=1;
	return (c.ch==1);
}


int main(){
	
	cout<<checkSystem()<<endl;
		
	return 0;
}