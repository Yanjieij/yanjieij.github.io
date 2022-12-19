#include"LinkedList.h"
#include"LinkedList.cpp"
#include<iostream>

using namespace std;

int main()
{
	LinkedList<int> myList;
	myList.reverseConnect(3, 7);
	cout << myList.loopDetect()->data << endl;
	myList.unhitchLoop();
	myList.showAllVal(0);
	myList.showAllVal(1);
	cout << myList.crossDetect()->data << endl;
	return 0;
}