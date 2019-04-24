#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>

int rng(int start, int end)
{

	static std::random_device rd; // obtain a random number from hardware
    
	static std::mt19937 eng(rd());

	
	std::uniform_int_distribution<> distr(start, end); // define the range

        return distr(eng);


}


int partition(std::vector<int> &unsortedlist, int start, int end)
{
    
    int pivot = unsortedlist.at(end);
    
    int i = start -1;
    
    for(int j = start; j < end ; j++)
    {
        if(unsortedlist.at(j) <= pivot)
        {
            i++;
            std::swap(unsortedlist.at(i), unsortedlist.at(j));
        }
    
    }
    
    std::swap(unsortedlist.at(i+1), unsortedlist.at(end));
    
    return i + 1;

}

int randomized_partition(std::vector<int> &unsortedlist, int start, int end)
{
	int i  = rng(start, end);

	
	std::swap(unsortedlist.at(i), unsortedlist.at(end));
	
	return partition(unsortedlist, start, end);

}


void quicksort(std::vector<int> &unsortedlist, int start, int end)
{
    	
	if(start < end)
	{
		int q = randomized_partition(unsortedlist, start, end);
        	quicksort(unsortedlist, start, q-1);
        	quicksort(unsortedlist, q+1, end);
    	}


}


int compvar(const void *one, const void *two)
{
    int a = *((int*)one);
    int b = *((int*)two);
    if (a<b)
       return -1;
    if (a == b)
       return 0;
    return 1;   

}



int main()
{
	std::vector<int> unsorted_list;

    	for(int i = 0; i<10000; i++)
    	{
    		unsorted_list.push_back(rng(0, 999999));
    
    
    	}
    

	std::vector<int> numbers = unsorted_list;

	auto start = std::chrono::high_resolution_clock::now(); 
	
	quicksort(unsorted_list, 0, (unsigned int)(unsorted_list.size()-1));
    
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    	
	std::cout<<"my time"<<duration.count()<<"\t";

	
	start = std::chrono::high_resolution_clock::now(); 
	std::qsort(&numbers[0], numbers.size(), sizeof(int), compvar);
	
	stop = std::chrono::high_resolution_clock::now();

	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    	
	std::cout<<"c++ time"<<duration.count()<<"\n";

}
