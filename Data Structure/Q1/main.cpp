#include"LinkedList.h"
#include"LinkedList.cpp"
#include<iostream>

using namespace std;

int main()
{
	LinkedList<int> myList;
	cout << "�������γɻ�����ʼ���ͽ�β����" << endl;
	int m, n = 0;
	cin >> m >> n;
	myList.reverseConnect(m, n);

	cout << "������ʼ���Ľڵ�ֵΪ��" << myList.loopDetect()->data << endl;
	myList.unhitchLoop();

	cout << "�ӵ�һ��ͷ��㿪ʼ������Ϊ��" << endl;
	myList.showAllVal(0);
	cout << "�ӵڶ���ͷ��㿪ʼ������Ϊ��" << endl;
	myList.showAllVal(1);

	cout <<"���洦�Ľڵ�ֵΪ��" << myList.crossDetect()->data << endl;
	return 0;
}