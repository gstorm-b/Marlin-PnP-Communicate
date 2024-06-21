#include "TimeCounter.h"

TimeCounter::TimeCounter() {
    IsCounterRunning = false;
}

TimeCounter::TimeCounter(int time) {
    timeOut = time;
    IsCounterRunning = false;
}

void TimeCounter::MarkStartPoint() {
    if(IsCounterRunning) {
        return;
    }
    timeStartPoint = high_resolution_clock::now();
}

void TimeCounter::SetTimeOut(int time) {
    if(IsCounterRunning) {
        return;
    }
    timeOut = time;
}

bool TimeCounter::TimeOutChecking() {
    auto currentTimePoint = high_resolution_clock::now();
    int durationTime = duration_cast<milliseconds>(currentTimePoint - timeStartPoint).count();
    if(durationTime >= timeOut) {
        return true;
    }
    return false;
}

bool TimeCounter::TimeOutCheckingCylic() {
    if(IsCounterRunning) {
        return false;
    }

    auto currentTimePoint = high_resolution_clock::now();
    int durationTime = duration_cast<milliseconds>(currentTimePoint - timeStartPoint).count();
    if(durationTime >= timeOut) {
        timeStartPoint = high_resolution_clock::now();
        return true;
    }
    return false;
}

bool TimeCounter::StartTimeCounter(int time) {
    if(!IsCounterRunning) {
        timeOut = time;
        timeStartPoint = high_resolution_clock::now();
        IsCounterRunning = true;
    }

    if(TimeOutChecking()) {
        IsCounterRunning = false;
        return true;
    }
    return false;
}
