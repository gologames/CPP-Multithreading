#include <iostream>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;

bool enabled = false;
mutex mut;
condition_variable cond;

void worker(int num)
{
    while (true)
    {   
        unique_lock<mutex> locker(mut);
        cond.wait(locker, []{ return enabled; });
        cout << num << '\n';
        locker.unlock();
        this_thread::sleep_for(chrono::seconds(num));
    }
}
  
int main()
{
    thread th1(worker, 1);
    thread th2(worker, 2);
    th1.detach();
    th2.detach();
  
    while (true)
    {
        char ch;
        if (cin.get(ch))
        {
            lock_guard<mutex> locker(mut);
            if (ch == 's')
            {
                bool last_state = enabled;
                enabled = true;
                if (!last_state) cond.notify_all(); 
            }
            if (ch == 'q') enabled = false;
        }
    }

    return 0;
}