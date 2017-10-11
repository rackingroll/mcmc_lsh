#pragma once
class Bucket
{
	private:
		int *arr;
		int isInit=-1;
		int index=0;
	public:
		Bucket();
		int totalAdded;
		static int _size;
		//void setSize(int size);
		int add(int id);
		int retrieve(int index);
		int * sample();
		int * getAll();
		int getSize();
		~Bucket();
};

