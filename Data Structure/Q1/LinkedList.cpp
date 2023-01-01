#include <iostream>
#include "LinkedList.h"

using namespace std;

template <typename T>
LinkedList<T>::LinkedList() : length(0), current(nullptr), first(nullptr), sec_first(nullptr), sec_length(0), loop_length(0), first_common_node(nullptr)
{
	std::cout << "请输入链表的值：" << std::endl;

	T temp_val;
	int _cnt = 0;

	while (cin >> temp_val && _cnt < 100)
	{
		if (_cnt == 0)
		{
			current = new Node<int>{temp_val, nullptr};
			first = current;
		}
		else
		{
			current->next = new Node<int>{temp_val, nullptr};
			current = current->next;
		}
		_cnt++;
		if (cin.get() == '\n')
			break;
	}
	current = first;

	if (_cnt >= DEFUALT_LENGTH)
		std::cout << "数据太多了！" << endl;

	length = _cnt;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
	current = first;
	for (int i = 0; i < length; i++)
	{
		if (current->next == first_common_node)
		{
			delete current;
			break;
		}
		Node<T> *_t = current;
		current = current->next;
		delete _t;
	}
	current = sec_first;
	for (int i = 0; i < sec_length; i++)
	{
		if (current->next == first_common_node)
		{
			delete current;
			break;
		}
		Node<T> *_t = current;
		current = current->next;
		delete _t;
	}
	current = first_common_node;
	for (int i = 0; i < length; i++)
	{
		if (current->next == nullptr)
		{
			delete current;
			break;
		}
		Node<T>* _t = current;
		current = current->next;
		delete _t;
	}
}

template <typename T>
bool LinkedList<T>::Delete(T aim_val)
{
	current = first;
	for (int cnt = 1; cnt < length; cnt++)
	{
		if (cnt == 1 && current->data == aim_val)
		{
			first = current->next;
			length--;
		}
		if (current->next->data == aim_val)
		{
			Node<T> *_t = current->next;
			current->next = current->next->next;
			delete _t;
			length--;
		}
		current = current->next;
	}
	return 1;
}

template <typename T>
bool LinkedList<T>::Insert(T aim_val, T new_val)
{
	current = first;
	for (int cnt = 0; cnt < length; cnt++)
	{
		if (current->data == aim_val)
		{
			if (cnt == length - 1) 
			{
				current->next = new Node<int>{new_val, nullptr};
			}
			else
			{
				Node<T> *temp = new Node<int>{new_val, nullptr};
				temp->next = current->next;
				current->next = temp;
			}
			length++;

			current = first;
			return 1;
		}
		current = current->next;
	}
	return 0;
}

template <typename T>
void LinkedList<T>::showAllVal(bool flag)
{
	int _length = 0;
	if (!flag)
	{
		_length = length;
		current = first;
	}
	else
	{
		_length = sec_length;
		current = sec_first;
	}

	while (current != nullptr)
	{
		cout << current->data << " ";
		current = current->next;
	}
	cout << endl;
}

template <typename T>
Node<T> *LinkedList<T>::searchNode(T aim_val)
{
	current = first;
	for (int i = 0; i < length; i++)
	{
		if (current->data == aim_val)
			return current;
		current = current->next;
	}
	return nullptr;
}

template <typename T>
Node<T> *LinkedList<T>::getNode(int id, bool flag)
{
	if (!flag)
		current = first;
	else
		current = sec_first;

	for (int i = 0; i < id - 1; i++)
	{
		if (current == nullptr)
			return NULL;
		current = current->next;
	}
	return current;
}

template <typename T>
bool LinkedList<T>::reverseConnect(T m, T n)
{
	if (m > n)
	{
		T _t = m;
		m = n;
		n = _t;
	}

	if (m == n)
		return false;

	Node<T> *frontNode = getNode(m);
	Node<T> *latterNode = getNode(n);
	// 调整新表头
	sec_first = latterNode->next;
	// 重新计算长度
	Node<T> *_p = sec_first;
	int minus_length = 1;
	for (; _p->next != nullptr; _p = _p->next)
		minus_length++;
	length -= minus_length;
	_p->next = frontNode;
	// 计算第二表头开始的链表长度
	latterNode->next = nullptr;
	_p = sec_first;
	for (sec_length=1; _p->next != nullptr; _p = _p->next)
		sec_length++;
	// 创建环路
	latterNode->next = frontNode;

	return true;
}

template <typename T>
Node<T> *LinkedList<T>::loopDetect(bool flag)
{
	Node<T> *fast = nullptr;
	Node<T> *slow = nullptr;
	if (!flag)
	{
		fast = first;
		slow = first;
	}
	else
	{
		fast = sec_first;
		slow = sec_first;
	}

	do
	{
		if (fast->next == nullptr || fast->next->next == nullptr)
		{
			loop_length = 0;
			return NULL;
		}
		slow = slow->next;
		fast = fast->next->next;
	} while (fast != slow);

	Node<T> *_ptr = fast->next;
	Node<T> *addr = fast;
	int cnt = 1;
	while (_ptr != addr)
	{
		cnt++;
		_ptr = _ptr->next;
	};
	loop_length = cnt;
	return getNode(length - cnt + 1);
}

template <typename T>
bool LinkedList<T>::unhitchLoop(bool flag)
{
	if (loop_length == 0)
		return 0;
	int _length = flag ? sec_length : length;
	getNode(_length, flag)->next = nullptr;
	return 1;
}

template <typename T>
Node<T> *LinkedList<T>::crossDetect()
{
	Node<T> *end1 = nullptr;
	current = first;
	while (current->next != nullptr)
	{
		current = current->next;
	};
	end1 = current;

	Node<T> *end2 = nullptr;
	current = sec_first;
	while (current->next != nullptr)
	{
		current = current->next;
	};
	end2 = current;

	if (end1 != end2)
		return nullptr;
	else
	{
		Node<T>* _first = ((length < sec_length) ? sec_first : first);
		int _length = ((length < sec_length) ? length : sec_length);
		bool flag = ((length < sec_length) ? 0 : 1);
		Node<T> *aimNode = nullptr;
		int range = 9;
		// 查找一个非共同节点
		while (range != 0)
		{
			range--;
			aimNode = getNode(range * 0.1 * _length, flag);
			for (current=_first ; current != nullptr; current = current->next)
			{
				if (current == aimNode)
					break;
				else if (current->next == nullptr)
					goto stop;
			}
		}
		// 从该节点向后查找，第一个共同节点就是交叉节点
	stop:
		current = ((length < sec_length) ? sec_first : first);
		for (int i = range * 0.1 * _length; i < (range + 1) * 0.1 * _length; i++)
		{
			aimNode = getNode(i, flag);
			for (current = _first; current != nullptr; current = current->next)
			{
				if (current == aimNode)
				{
					first_common_node = current;
					return current;
				}
			}
		}
	}
	return NULL;
}
