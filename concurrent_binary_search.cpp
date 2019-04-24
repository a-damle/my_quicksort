#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <numeric> // for iota
#include <chrono> // for timing

template<typename T>
bool iterative_binary_search(const std::vector<T> & list, const T & value)
{
  signed long lo = 0;
  signed long hi = list.size()-1;

  while(lo <= hi)
    {
      signed long mid = lo + ( (hi-lo) >> 1);

      if(value < list[mid])
	hi = mid-1;
      else if (value > list[mid])
	lo = mid+1;
      else
	return true;
    }
  
  return false;
}

void concurrent(const std::vector<int> & data)
{  
  int searchfor[3] = {-500, 0, 343};

  std::future<bool> fut0 = std::async(&iterative_binary_search<int>, data, searchfor[0]);
  std::future<bool> fut1 = std::async(&iterative_binary_search<int>, data, searchfor[1]);
  std::future<bool> fut2 = std::async(&iterative_binary_search<int>, data, searchfor[2]);

  bool value;

  value = fut0.get();
  std::cout << "Found " << searchfor[0] << " : " << value << std::endl;

  value = fut1.get();
  std::cout << "Found " << searchfor[1] << " : " << value << std::endl;

  value = fut2.get();
  std::cout << "Found " << searchfor[2] << " : " << value << std::endl;
}

void serial(const std::vector<int> & data)
{
  int searchfor[3] = {-500, 0, 343};
  bool value0, value1, value2;

  value0 = iterative_binary_search(data, searchfor[0]);
  value1 = iterative_binary_search(data, searchfor[1]);
  value2 = iterative_binary_search(data, searchfor[2]);

  std::cout << "Found " << searchfor[0] << " : " << value0 << std::endl;
  std::cout << "Found " << searchfor[1] << " : " << value1 << std::endl;
  std::cout << "Found " << searchfor[2] << " : " << value2 << std::endl;
}

int main()
{
  // create a vector [-345, -344, ...]
  std::vector<int> data(40000000);
  std::iota(data.begin(), data.end(), -345);

  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::chrono::duration<double> elapsed_seconds;

  // time concurrent
  start = std::chrono::system_clock::now();
  concurrent(data);
  end = std::chrono::system_clock::now();  
  elapsed_seconds = end-start;
  std::cout << "Concurrent elapsed time: " << elapsed_seconds.count() << "s\n";

  // time serial
  start = std::chrono::system_clock::now();
  serial(data);
  end = std::chrono::system_clock::now();  
  elapsed_seconds = end-start;
  std::cout << "Serial elapsed time: " << elapsed_seconds.count() << "s\n";

  return 0;
}
