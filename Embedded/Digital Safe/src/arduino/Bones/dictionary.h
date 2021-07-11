#ifndef __bones_dictionary_H__
#define __bones_dictionary_H__

#define DICT_INIT_CAP 10
#define DICT_CAP_INC 5

namespace Bones
{
	template <typename T, typename U>
	class Dictionary
	{
	private:
		T* keys;
		U* values;
		int count;
		int capacity;
	public:
		Dictionary() : count(0), capacity(DICT_INIT_CAP)
		{
			keys = new T[DICT_INIT_CAP];
			values = new U[DICT_INIT_CAP];
		}

		~Dictionary()
		{
			delete[] keys;
			delete[] values;
		}
	public:
		U operator[] (int index)
		{
			return values[index];
		}
	public:
		void Set(T key, U value)
		{
			for (int i = 0; i < count; i++)
				if (keys[i] == key) { values[i] = value; return; }

			if (count < capacity)
			{
				keys[count] = key;
				values[count] = value;
				count++;
			}
			else
			{
				capacity += DICT_CAP_INC;
				T* tempKeys = new T[capacity];
				U* tempValues = new U[capacity];

				for (int i = 0; i < count; i++)
				{
					tempKeys[i] = keys[i];
					tempValues[i] = values[i];
				}

				delete[] keys;
				delete[] values;
				keys = tempKeys;
				values = tempValues;
				keys[count] = key;
				values[count] = value;
				count++;
			}
		}

		void Remove(T key)
		{
			T* tempKeys = new T[capacity];
			U* tempValues = new U[capacity];
			int index = 0;
			int currentCount = count;

			for (int i = 0; i < currentCount; i++)
			{
				if (keys[i] != key)
				{
					tempKeys[index] = keys[i];
					tempValues[index] = values[i];
					index++;
				}
				else
				{
					count--;
				}
			}

			delete[] keys;
			delete[] values;
			keys = tempKeys;
			values = tempValues;
		}

		bool TryGetValue(T key, U& value)
		{
			for (int i = 0; i < count; i++)
				if (keys[i] == key) { value = values[i]; return true; }

			return false;
		}

		int Count()
		{
			return count;
		}

		int Size()
		{
			return capacity * sizeof(T) + capacity * sizeof(U);
		}
	};
}

#endif