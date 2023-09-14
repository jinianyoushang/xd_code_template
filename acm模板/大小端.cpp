#include<iostream>


using namespace std;

//检查是否是小端 
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