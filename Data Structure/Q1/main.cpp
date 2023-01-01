#include"LinkedList.h"
#include"LinkedList.cpp"
#include<iostream>

using namespace std;

int main()
{
	LinkedList<int> myList;
	cout << "请输入形成环的起始处和结尾处：" << endl;
	int m, n = 0;
	cin >> m >> n;
	myList.reverseConnect(m, n);

	cout << "环的起始处的节点值为：" << myList.loopDetect()->data << endl;
	myList.unhitchLoop();

	cout << "从第一个头结点开始的链表为：" << endl;
	myList.showAllVal(0);
	cout << "从第二个头结点开始的链表为：" << endl;
	myList.showAllVal(1);

	cout <<"交叉处的节点值为：" << myList.crossDetect()->data << endl;
	return 0;
}