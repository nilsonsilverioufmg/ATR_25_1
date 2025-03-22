#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include "shared_memory_buffer.hpp"

using namespace boost::interprocess;

int main ()
{

   //Create a shared memory object.
   shared_memory_object shm
      (open_only                    // open only
      ,"MySharedMemory1"              //name
      ,read_write  //read-write mode
      );

   //Map the whole shared memory in this process
   mapped_region region
      (shm                       //What to map
      ,read_write //Map it as read-write
      );

   //Get the address of the mapped region
   void * addr       = region.get_address();

   //Obtain the shared structure
   shared_memory_buffer * data = (shared_memory_buffer*)addr;

   int index = 0;
   while (true)
   {
      data->nstored.wait();
      data->mutex.wait();
      int item = data->items[index % shared_memory_buffer::NumItems];
			std::cout << "Item consumed: " << item << std::endl;
			++index;
			data->mutex.post();
      data->nempty.post();
   }
   return 0;
}
