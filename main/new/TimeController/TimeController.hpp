#ifndef TIMECONTROLLER_HPP
# define TIMECONTROLLER_HPP

#include <time.h>
#include <sys/time.h>
#include <map>
#include <iostream>

class TimeController
{
private:
	std::map<void*, clock_t> timer_list;

public:
	void init_time(void* id)
	{
		timer_list[id] = clock();
		std::cout << "timer start : " << (double)timer_list[id] << std::endl;
	}

	double get_time(void *id)
	{
		clock_t end;
		end = clock();
	//	std::cout << i << " end :" << (double)end << std::endl;
	//	std::cout << i << " start :" << (double)timer_list[i] << std::endl;
//		std::cout << "time : " << (double)(end - timer_list[i]) << std::endl;
		return (double(end - timer_list[id]));
	}

	void del_time(void *id)
	{
		timer_list.erase(id);
		std::cout << "timer delete" << std::endl;
	}

	void clean_time(void* id)
	{
		if (timer_list.find(id) != timer_list.end())
			timer_list[id] = clock();
		else
			return ;
		std::cout << "timer reset" << std::endl;
	}

	bool find_time(void* id)
	{
		if (timer_list.find(id) != timer_list.end())
			return (true);
		else
			return (false);
	}

	void check_time(void)
	{
		for (std::map<void*, clock_t>::iterator it = timer_list.begin(); it != timer_list.end(); it++) {
	//		std::cout << "Asd" << std::endl;
		}
	//	std::cout << "Asd" << std::endl;
	}
};


#endif
