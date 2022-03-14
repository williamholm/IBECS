#pragma once
#include<array>

//returns true if x is in given array 
template<class T, int N>
constexpr bool isInArray(const T& x, const std::array<T, N>& arr)
{
	for (const auto& y : arr)
	{
		if (y == x)
		{
			return true;
		}
	}
	return false;
}

template<class T, int N, int M> //constexpr concatination of two arrays of the same type.
constexpr std::array<T, N + M> concatinate(const std::array<T, N>& arr1, const std::array<T, M>& arr2)
{
	std::array<T, N + M> temp = {};
	for (int i = 0; i < N; ++i)
	{
		temp[i] = arr1[i];
	}
	for (int i = N; i < N + M; ++i)
	{
		temp[i] = arr2[i - N];
	}

	return temp;
}
template<class T, int N> //for adding a single value to end of array
constexpr std::array<T, N + 1> concatinate(const std::array<T, N>& arr1, const T& x)
{
	std::array<T, N + 1> temp = {};
	for (int i = 0; i < N; ++i)
	{
		temp[i] = arr1[i];
	}
	temp[N] = x;

	return temp;
}
template<class T, int N> //for adding a single value to start of array
constexpr std::array<T, N + 1> concatinate(const T& x, const std::array<T, N>& arr1)
{
	std::array<T, N + 1> temp = {};
	for (int i = 0; i < N; ++i)
	{
		temp[i + 1] = arr1[i];
	}
	temp[0] = x;

	return temp;
}

//returns array with new size, all new T's are default initialized at end of original array. 
template<typename T, int currentSize, int newSize>
constexpr auto resizeArray(const std::array<T, currentSize>& arr)
{
	return concatinate(arr, std::array<T, newSize - currentSize>());
}
//different method for constexpr resize
template<typename T, int currentSize, int newSize>
constexpr std::array<T, newSize> resizeArray(const std::array<T, currentSize>& arr, const std::array<T, newSize>&)
{
	std::array<T, newSize> temp = {};
	for (int i = 0; i < newSize; ++i)
	{
		temp[i] = arr[i];
	}
	return temp;
}
template<typename T, int size>
constexpr auto noOfInstancesOf(const T& x, const std::array<T, size>& arr)
{
	int noOfX = 0;
	for (int i = 0; i < size; ++i)
	{
		if (x == arr[i])
		{
			++noOfX;
		}
	}
	return noOfX;
}
template<typename T, int size, int newSize>
constexpr auto removeInstancesOf(const T& x, const std::array<T, size>& arr)
{

	std::array<T, newSize> newArray = {};
	int counter = 0;
	for (int i = 0; i < size; ++i)
	{
		if (x != arr[i])
		{
			newArray[counter] = arr[i];
			++counter;
		}
	}
	return newArray;
}


//requires T be default initilizable
template<typename T, int N>
constexpr int noOfUniqueElements(const std::array<T, N>& arr)
{
	std::array<T, N> isDuplicate = {};
	int noOfUnique = 0;
	//need to do this to check if default initilization is in arr, as loop misses this case.
	if (isInArray(T(),arr))
	{
		++noOfUnique;
	}
	for (int i = 0; i < N; ++i)
	{
		if (!isInArray(arr[i],isDuplicate))
		{
			isDuplicate[noOfUnique] = arr[i];
			++noOfUnique;
		}
	}
	return noOfUnique;
}


template<int M, typename T, int N>
constexpr std::array<T,M> uniqueElements(const std::array<T, N>& arr)
{
	std::array<T, M> uniqueArray = {};

	int noOfUnique = 0;
	//need to do this to check if default initilization is in arr, as loop misses this case.
	if (isInArray(T(), arr))
	{
		++noOfUnique;
	}
	for (int i = 0; i < N; ++i)
	{
		if (!isInArray(arr[i], uniqueArray))
		{
			uniqueArray[noOfUnique] = arr[i];
			++noOfUnique;
		}
	}
	return uniqueArray;
}


//returns the position of value in the array of unique elements
template<class T, int N>
constexpr uint32_t positionInUniqueArr(const T& val, const std::array<T, N>& uniqueArr)
{
	assert(noOfUniqueElements(uniqueArr) == N && isInArray(val,uniqueArr));//ensure arr contains val and has only unique elements
	for (int i = 0; i < N; ++i)
	{
		if (uniqueArr[i] == val)
		{
			return i;
		}
	}
}

//
template<class T, int N, int M>
constexpr auto positionalArray(const std::array<T, N>& fullArr, const std::array<T, M>& uniqueArr)
{
	assert(M <= N);
	std::array<uint32_t, N> positionArray = {};
	for (int i = 0; i < N; ++i)
	{
		positionArray[i] = positionInUniqueArr(fullArr[i], uniqueArr);
	}
	return positionArray;
}

template<class T, int... sizes>
constexpr auto intersection(const std::array<T, sizes>&... arrays)
{
	//auto temp = intersectionRec(arrays...);
	//auto size = noOfInstancesOf(T(), temp);
	//temp = removeInstancesOf<T,size+1,size>(T(), temp);
	//return temp;
	return intersectionRec(arrays...);
}

template<class T, int N, int M>
constexpr auto intersectionRec(const std::array<T, N>& arr1, const std::array<T, M>& arr2)
{
	std::array<T, N> temp = {};
	int counter = 0;
	for (int i = 0; i < N; ++i)
	{
		if (isInArray(arr1[i], arr2))
		{
			temp[counter] = arr1[i];
			++counter;
		}
	}
	return temp;
}
template<class T,int N, int M, int... sizes>
constexpr auto intersectionRec(const std::array<T,N>& arr1, const std::array<T, M>& arr2, const std::array<T, sizes>&... arrays)
{
	auto first = intersectionRec(arr1, arr2);
	return intersectionRec(first, arrays...);

}
