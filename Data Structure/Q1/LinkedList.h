#pragma once
#define DEFUALT_LENGTH 100

template<typename T>
class List
{
public:
	virtual bool Delete(T val) = 0;
	virtual bool Insert(T val, T new_val) = 0;
	virtual void showAllVal(bool flag) = 0;
};

template<typename T>
struct Node
{
	T data = NULL;
	Node* next = nullptr;
};

template<typename T>
class LinkedList :public List<T>
{
public:
	LinkedList();
	~LinkedList();
	bool Delete(T aim_val);
	bool Insert(T aim_val, T new_val);
	bool reverseConnect(T m, T n);
	Node<T>* searchNode(T aim_val);
	Node<T>* getNode(int id, bool flag = 0);
	void showAllVal(bool flag = 0);
	Node<T>* loopDetect(bool flag = 0);
	bool unhitchLoop(bool flag = 0);
	Node<T>* crossDetect();
private:
	int length;
	int sec_length;
	int loop_length;
	Node<T>* first;
	Node<T>* sec_first;
	Node<T>* current;
	Node<T>* first_common_node;
};