#pragma once

#include <stdexcept>
#include <memory>
#include <vector>
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
        int          Index;

        Tuple(ListItem<T>* item, const int index)
        {
            this->Item  = item;
            this->Index = index;
        }
    };
    //-------------------------------------------------------------------------
    template<typename T>
    class MyList
    {
    public:
        typedef std::unique_ptr<MyListEnumerator<T>> Iterator;

        MyList();
        ~MyList();

        T    getItem          (const uint index);
        void     setItem          (const uint index, const T& value);
        T    operator[]   (const uint index)                    { return this->getItem(index); }
        uint     Count        ()                                    { return _count; }
        bool     IsReadOnly   ()                                    { return false; }
        void     Add              (const T& value)                  { this->InsertInternal(value, _tail); }
        void     Clear        ();
        bool     Contains     (const T& item)                   { return (this->IndexOf(item)) >= 0; }
        void     CopyTo       (T* array, const uint arrayIndex);
        void     CopyTo       (std::vector<T> vector);
        Iterator GetEnumerator()                                { return std::make_unique<MyListEnumerator<T>>(_head->Next, _tail); }
        int      IndexOf      (const T& item);
        void     Insert       (const uint index, const T& item);
        bool     Remove       (const T& item);
        void     RemoveAt     (const uint index);
    //-------------------------------------------------------------------------
    private:
        uint         _count;
        ListItem<T>* _head;
        ListItem<T>* _tail;

        void                          InsertInternal (const T& value, ListItem<T>* rightElement);
        ListItem<T>*                  GetItemInternal(uint index);
        std::unique_ptr<Tuple<T>> GetItemInternal(const T& item);
        bool                          RemoveInternal (ListItem<T>* item);
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
        if (index > _count) throw std::invalid_argument("Index out of range");

        ListItem<T>* item = this->GetItemInternal(index);

        return item->Value;
    }
    //-------------------------------------------------------------------------
    template<typename T>
    void MyList<T>::setItem(const uint index, const T& value)
    {
        if (index > _count) throw std::invalid_argument("Index out of range");

        ListItem<T>* item = this->GetItemInternal(index);
        item->Value = value;
    }
    //-------------------------------------------------------------------------
    template<typename T>
    void MyList<T>::Clear()
    {
        ListItem<T>* current = _head->Next;

        while (current != nullptr && current->Next != nullptr)
        {
            ListItem<T>* next = current->Next;
            delete current;
            current = next;
        }

        _head->Next     = _tail;
        _tail->Previous = _head;
        _count          = 0;
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
    inline void MyList<T>::CopyTo(std::vector<T> vector)
    {
        for (ListItem<T>* current = _head->Next; current != _tail; current = current->Next)
            vector.push_back(current->Value);
    }
    //-------------------------------------------------------------------------
    template<typename T>
    int MyList<T>::IndexOf(const T& item)
    {
        std::unique_ptr<Tuple<T>> tuple = this->GetItemInternal(item);

        return tuple->Index;
    }
    //-------------------------------------------------------------------------
    template<typename T>
    void MyList<T>::Insert(const uint index, const T& item)
    {
        ListItem<T>* rightElement = nullptr;

        if (index == _count)
            rightElement = _tail;
        else
            rightElement = this->GetItemInternal(index);

        this->InsertInternal(item, rightElement);
    }
    //-------------------------------------------------------------------------
    template<typename T>
    bool MyList<T>::Remove(const T& value)
    {
        std::unique_ptr<Tuple<T>> tuple = this->GetItemInternal(value);

        return this->RemoveInternal(tuple->Item);
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

        toInsert->Previous           = rightElement->Previous;
        toInsert->Next               = rightElement;
        rightElement->Previous->Next = toInsert;
        rightElement->Previous       = toInsert;

        _count++;
    }
    //-------------------------------------------------------------------------
    template<typename T>
    ListItem<T>* MyList<T>::GetItemInternal(const uint index)
    {
        if (index >= _count) throw std::invalid_argument("Index out of range");

        ListItem<T>* current = nullptr;

        if (index < _count / 2)
        {
            current = _head->Next;

            for (uint i = 0; i < index; ++i)
                current = current->Next;
        }
        else
        {
            current = _tail->Previous;

            for (uint i = _count - 1; i > index; --i)
                current = current->Previous;
        }

        return current;
    }
    //-------------------------------------------------------------------------
    template<typename T>
    std::unique_ptr<Tuple<T>> MyList<T>::GetItemInternal(const T& item)
    {
        ListItem<T>* current = _head->Next;
        int index            = 0;

        for (; current != _tail; current = current->Next, ++index)
            if (current->Value == item)
                return std::make_unique<Tuple<T>>(current, index);

        return std::make_unique<Tuple<T>>(nullptr, -1);
    }
    //-------------------------------------------------------------------------
    template<typename T>
    bool MyList<T>::RemoveInternal(ListItem<T>* item)
    {
        if (item == nullptr) return false;

        item->Previous->Next = item->Next;
        item->Next->Previous = item->Previous;
        _count--;

        delete item;

        return true;
    }
}
