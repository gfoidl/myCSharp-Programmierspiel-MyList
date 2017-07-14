#pragma once

#include <stdexcept>
#include "ListItem.h"
#include "MyListEnumerator.h"

namespace Inflames2K
{
	typedef unsigned int uint;
	//-------------------------------------------------------------------------
	template<typename T>
	struct Tuple
	{
		ListItem<T>* Item;
		int Index;

		Tuple(ListItem<T>* item, const int index)
		{
			this->Item = item;
			this->Index = index;
		}
	};
	//-------------------------------------------------------------------------
	template<typename T>
	class MyList
	{
	public:
		MyList();
		~MyList();
		T getItem(const uint index);
		void setItem(const uint index, const T& value);
		uint Count;
		bool IsReadOnly() { return false; }
		void Add(const T& value) { this->InsertInternal(value, _tail); }
		void Clear();
		bool Contains(const T& item) { return (this->IndexOf(item)) >= 0; }
		void CopyTo(T* array, const uint arrayIndex);
		MyListEnumerator<T>* GetEnumerator() { return new MyListEnumerator<T>(_head->Next, _tail); }
		int IndexOf(const T& item);
		void Insert(const uint index, const T& item);
		bool Remove(const T& item);
		void RemoveAt(const uint index);
	//-------------------------------------------------------------------------
	private:
		ListItem<T>* _head;
		ListItem<T>* _tail;

		void InsertInternal(const T& value, ListItem<T>* rightElement);
		ListItem<T>* GetItemInternal(uint index);
		Tuple<T>* GetItemInternal(const T& item);
		bool RemoveInternal(ListItem<T>* item);
	};
	//-------------------------------------------------------------------------
	template<typename T>
	MyList<T>::MyList()
	{
		_head = new ListItem<T>(T());
		_tail = new ListItem<T>(T());

		this->Clear();
	}
	//-------------------------------------------------------------------------
	template<typename T>
	MyList<T>::~MyList()
	{
		this->Clear();

		delete _head;
		delete _tail;
	}
	//-------------------------------------------------------------------------
	template<typename T>
	T MyList<T>::getItem(const uint index)
	{
		if (index > this->Count) throw std::invalid_argument("Index out of range");

		ListItem<T>* item = this->GetItemInternal(index);

		return item->Value;
	}
	//-------------------------------------------------------------------------
	template<typename T>
	void MyList<T>::setItem(const uint index, const T& value)
	{
		if (index > this->Count) throw std::invalid_argument("Index out of range");

		ListItem<T>* item = this->GetItemInternal(index);

		ListItem<T>* newItem = new ListItem<T>(value);

		item->Previous->Next = newItem;
		item->Next->Previous = newItem;
		newItem->Previous = item->Previous;
		newItem->Next = item->Next;

		delete item;
	}
	//-------------------------------------------------------------------------
	template<typename T>
	void MyList<T>::Clear()
	{
		ListItem<T>* current = _head->Next;

		while (current != NULL && current->Next != NULL)
		{
			ListItem<T>* next = current->Next;
			delete current;
			current = next;
		}

		_head->Next = _tail;
		_tail->Previous = _head;
		this->Count = 0;
	}
	//-------------------------------------------------------------------------
	template<typename T>
	void MyList<T>::CopyTo(T* array, const uint arrayIndex)
	{
		uint i = arrayIndex;

		for (ListItem<T>* current = _head->Next; current != _tail; current = current->Next)
			array[i++] = current->Value;
	}
	//-------------------------------------------------------------------------
	template<typename T>
	int MyList<T>::IndexOf(const T& item)
	{
		Tuple<T>* tuple = this->GetItemInternal(item);
		int index = tuple->Index;

		delete tuple;

		return index;
	}
	//-------------------------------------------------------------------------
	template<typename T>
	void MyList<T>::Insert(const uint index, const T& item)
	{
		ListItem<T>* rightElement = NULL;

		if (index == this->Count)
			rightElement = _tail;
		else
			rightElement = this->GetItemInternal(index);

		this->InsertInternal(item, rightElement);
	}
	//-------------------------------------------------------------------------
	template<typename T>
	bool MyList<T>::Remove(const T& value)
	{
		ListItem<T>* item = (this->GetItemInternal(value))->Item;

		return this->RemoveInternal(item);
	}
	//-------------------------------------------------------------------------
	template<typename T>
	void MyList<T>::RemoveAt(const uint index)
	{
		ListItem<T>* item = this->GetItemInternal(index);

		this->RemoveInternal(item);
	}
	//-------------------------------------------------------------------------
	template<typename T>
	void MyList<T>::InsertInternal(const T& value, ListItem<T>* rightElement)
	{
		ListItem<T>* toInsert = new ListItem<T>(value);

		toInsert->Previous = rightElement->Previous;
		toInsert->Next = rightElement;
		rightElement->Previous->Next = toInsert;
		rightElement->Previous = toInsert;

		this->Count++;
	}
	//-------------------------------------------------------------------------
	template<typename T>
	ListItem<T>* MyList<T>::GetItemInternal(const uint index)
	{
		if (index >= this->Count) throw std::invalid_argument("Index out of range");

		ListItem<T>* current = NULL;

		if (index < this->Count / 2)
		{
			current = _head->Next;

			for (uint i = 0; i < index; ++i)
				current = current->Next;
		}
		else
		{
			current = _tail->Previous;

			for (uint i = this->Count - 1; i > index; --i)
				current = current->Previous;
		}

		return current;
	}
	//-------------------------------------------------------------------------
	template<typename T>
	Tuple<T>* MyList<T>::GetItemInternal(const T& item)
	{
		ListItem<T>* current = _head->Next;
		int index = 0;

		for (; current != _tail; current = current->Next, ++index)
			if (current->Value == item)
				return new Tuple<T>(current, index);

		return new Tuple<T>(NULL, -1);
	}
	//-------------------------------------------------------------------------
	template<typename T>
	bool MyList<T>::RemoveInternal(ListItem<T>* item)
	{
		if (item == NULL) return false;

		item->Previous->Next = item->Next;
		item->Next->Previous = item->Previous;
		this->Count--;

		delete item;

		return true;
	}
}