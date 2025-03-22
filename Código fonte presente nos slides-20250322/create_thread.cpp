#include <iostream>
#include <thread>

using namespace std;

void g()
{
	//while (true);
  cout << "g()" << endl;
}

void f(int a, int b)
{
	thread t2(g);
	for (int i =0; i < 2; ++i)
	{
		cout << "f()" << endl;
		this_thread::sleep_for(1s);
	}
  t2.join();
}

int main()
{
	int val = 3;
	thread t(f, val, 2);
	f(3,4);
	for (int i =0; i < 10; ++i)
	{
		cout << "main()" << endl;
		this_thread::sleep_for(1s);
	}
	t.join();
	return(0);
}

