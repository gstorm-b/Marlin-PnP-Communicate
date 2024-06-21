#ifndef TIMECOUNTER_H
#define TIMECOUNTER_H

#include <chrono>

using namespace std::chrono;

class TimeCounter
{
public:
    TimeCounter();
    TimeCounter(int time);
    void MarkStartPoint();
    void SetTimeOut(int time);
    bool TimeOutChecking();
    bool TimeOutCheckingCylic();
    bool StartTimeCounter(int time);

private:
    high_resolution_clock::time_point timeStartPoint;
    int timeOut;
    bool IsCounterRunning;
};

#endif // TIMECOUNTER_H
