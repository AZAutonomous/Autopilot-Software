#include <iostream>
#include <fstream>

using namespace std;

void printMemes() {
	printf("memes\n");
	printf("memes\n");
	printf("memes\n");
	printf("memes\n");
}

int addTwoInts(int numa, int numb) {
	int returnValue = numa + numb;
	return returnValue;
}

void addOne(int* a) {
	*a = *a + 1;
}

int main(void){
	int a = 5;
	int *z = &a;
	//addOne(z);
	a = a + 1;
	printf("%d\n", *z);
	
}