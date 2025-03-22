#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include "shared_memory_buffer.hpp"

using namespace boost::interprocess;

int main ()
{


   //Create a shared memory object.
   shared_memory_object shm
	   (open_or_create                  //only create
      ,"MySharedMemory1"              //name
      ,read_write                    //read-write mode
      );

   //Set size
   shm.truncate(sizeof(shared_memory_buffer));

   //Map the whole shared memory in this process
   mapped_region region
      (shm                       //What to map
      ,read_write //Map it as read-write
      );

   //Get the address of the mapped region
   void * addr       = region.get_address();

   //Construct the shared structure in memory
   shared_memory_buffer * data = new (addr) shared_memory_buffer;

   const int NumMsg = 100;

   //Insert data in the array
   int index = 0;
   while (true)
   {
      data->nempty.wait();
      data->mutex.wait();
      data->items[index % shared_memory_buffer::NumItems] = index;
			std::cout << "Item produced: " << index << std::endl;
			++index;
      data->mutex.post();
      data->nstored.post();
   }

   return 0;
}
