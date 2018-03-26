# ifndef TIMER_H
# define TIMER_H

#include <thread>
#include <chrono>

class Timer
{
    std::thread Thread;
    bool Alive = false;
    long CallNumber = -1L;
    long repeat_count = -1L;
    std::chrono::milliseconds interval = std::chrono::milliseconds(0);
    std::function<void(void)> funct = nullptr;

    void SleepAndRun()
    {
        std::this_thread::sleep_for(interval);
        if(Alive)
            Function()();//first one calls function & second one will call function that this function returns
    }

    void ThreadFunc()
    {
        if(CallNumber == Infinite)
            while(Alive)
              SleepAndRun();
        else
            while(repeat_count--)
              SleepAndRun();
    }

public:
    static const long Infinite = -1L;

    Timer (){};

    Timer(const std::function<void(void)> &f) : funct (f) {}

    Timer(const std::function<void(void)> &f,
          const unsigned long &i,
          const long repeat = Timer::Infinite) : funct(f),
                                                 interval(std::chrono::milliseconds(i)),
                                                 CallNumber(repeat) {}

    void Start(bool Async = true)
    {
        if(IsAlive())
           return;
        Alive = true;
        repeat_count = CallNumber;
        if(Async)
            Thread = std::thread(ThreadFunc,this);
        else
            this->ThreadFunc();
    }

    void Stop()
    {

        Alive = false;
        Thread.join();
        //infact stop the timer
    }

    void SetFunction(const std::function<void(void)> &f)
    {
        funct = f;
    }

    bool IsAlive() const {return Alive;}

    void RepeatCount(const long r)
    {
        if(Alive)
            return;
        CallNumber = r;
        //set the number for repeatances
    }

    long GetLeftCount() const {return repeat_count;}//get how many iterations are left

    long RepeatCount () const {return CallNumber;}

    void SetInterval(const unsigned long &i)
    {
        if(Alive)
            return;
        interval = std::chrono::milliseconds(i);
    }

    unsigned long Interval() const {return interval.count();}
    //to get a no. from interval .count is there, we are converting to chromo milliseconds to long

    const std::function<void(void)> &Function() const
    {

        return funct;
    }


};
# endif // TIMER_H
