#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;

int val = 0;

mutex m;

void imprime(int i)
{
	int val_local = 0;
	while (true)
	{
		m.lock();
		val_local = i;
		cout << "Thread id:  "<< this_thread::get_id() 
			<< " - Value: " << val  
			<< " - Local value: " << val_local << endl;
		m.unlock();
		this_thread::sleep_for(1s);
	}
}


int main()
{
	thread t(imprime,1);
  this_thread::sleep_for(5s);
	val = 1;
	imprime(2);
	t.join(); 
	return(0);
}
