#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>
#include <future>
#include <functional>
#include <thread>
#include <string>


int total_threads = 3;
int used_threads = 0;




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
        	if(used_threads < total_threads)
		{
			used_threads++;
			auto handle_low = std::async(quicksort, std::ref(unsortedlist), start, q-1);
			//quicksort(unsortedlist, start, q-1);
        		quicksort(unsortedlist, q+1, end);
			//handle_low.get();
			//used_threads--;
		}
		else
		{
			quicksort(unsortedlist, start, q-1);
        		quicksort(unsortedlist, q+1, end);
		}
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

    	for(int i = 0; i<1000000; i++)
    	{
    		unsorted_list.push_back(rng(0, 999999));
    
    
    	}
    

	std::vector<int> numbers = unsorted_list;

	std::vector<int> unsorted_list2 = unsorted_list;
	
	std::string blocking;
	std::cin>>blocking;
	std::cout<<"now testing async\n";
	auto start = std::chrono::high_resolution_clock::now(); 
	
	quicksort(unsorted_list, 0, unsorted_list.size()-1);
    
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    	
	std::cout<<total_threads<<" threads async time: "<<duration.count()<<"\n";



	total_threads = -1;
	start = std::chrono::high_resolution_clock::now(); 
	
	quicksort(unsorted_list2, 0, unsorted_list2.size()-1);
    
	stop = std::chrono::high_resolution_clock::now();

	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    	
	std::cout<<"serial time: "<<duration.count()<<"\n";




	
	start = std::chrono::high_resolution_clock::now(); 
	std::qsort(&numbers[0], numbers.size(), sizeof(int), compvar);
	
	stop = std::chrono::high_resolution_clock::now();

	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    	
	std::cout<<"c++ time: "<<duration.count()<<"\n";

	bool are_equal = numbers == unsorted_list;
	are_equal = are_equal && (numbers == unsorted_list2);

	std::cout<<"all equal: "<<are_equal<<"\n";

	std::cout<<"thread count: "<<std::thread::hardware_concurrency()<<"\n";
}
