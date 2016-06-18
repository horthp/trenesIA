
#include <iostream>
#include <stdio.h>

using namespace std;
int misterio(int a, int b);

int main(void){
	int x = 18, y=10, result=0;		
	result = misterio(x, y);
	printf("\n misterio develado ==> %d", result);
	
	return 0;
}

int misterio(int a, int b){
	/* caso base*/
	cout<<endl<< "a ="<< a << " b = "<< b; /* Esto no está en el libro*/
	if(b == 1){
		return a;
	}
	else{
		return a + misterio(a, b-1);
		cout<<"fin recursion";
	}	
}