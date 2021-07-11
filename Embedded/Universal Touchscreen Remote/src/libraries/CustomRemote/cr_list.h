#ifndef __cr_list__
#define __cr_list__

#define LIST_INIT_CAP 15
#define LIST_CAP_INC 15

template <typename T>
class List
{
private:
    T* items;
    int count;
    int capacity;
    
public:
    List() : count(0), capacity(LIST_INIT_CAP)
    {
        items = new T[LIST_INIT_CAP];
    }

    ~List()
    {
        delete[] items;
    }
    
public:
    T operator[] (int index)
    {
        return items[index];
    }
    
public:
    void Add(T item)
    {
        if (count < capacity)
        {
            items[count++] = item;
        }
        else
        {
            capacity += LIST_CAP_INC;
            T* temp = new T[capacity];

            for (int i = 0; i < count; i++)
                temp[i] = items[i];

            delete[] items;
            items = temp;
            items[count++] = item;
        }
    }

    void Remove(T item)
    {
        T* temp = new T[capacity];
        int index = 0;
        int currentCount = count;

        for (int i = 0; i < currentCount; i++)
        {
            if (items[i] != item)
                temp[index++] = items[i];
            else
                count--;
        }

        delete[] items;
        items = temp;
    }

    int Count()
    {
        return count;
    }

    int Size()
    {
        return capacity * sizeof(T);
    }
};


#endif