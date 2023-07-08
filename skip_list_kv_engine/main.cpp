#include <iostream>
#include "SkipList.h"

#define FILE_PATH "./stroe/dumpFile"

int main()
{
    SkipList<int,std::string> skip_list(6);
	skip_list.insert_element(1, "九点睡是村里人"); 
	skip_list.insert_element(3, "十点睡是厂里人"); 
	skip_list.insert_element(7, "十一点睡是校内人"); 
	skip_list.insert_element(8, "十二点睡是官府人"); 
	skip_list.insert_element(9, "一点睡是码农"); 
	skip_list.insert_element(19, "两点睡是码农"); 
	skip_list.insert_element(20, "三点睡是码农");
	skip_list.insert_element(22, "四点五点六点睡,还得是码农");
    
    std::cout << "skip_list size:" << skip_list.size() << std::endl; 

    skip_list.dump_file();

    // skipList.load_file();

    skip_list.search_element(9);
    skip_list.search_element(18);


    skip_list.display_list();

    skip_list.delete_element(3);
    skip_list.delete_element(7);

    std::cout << "skip_list size:" << skip_list.size() << std::endl;

    skip_list.display_list();
}
