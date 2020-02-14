#ifndef _TICTACTOE3D_DEADLINE_H_
#define _TICTACTOE3D_DEADLINE_H_

#include <stdint.h>
#include <stdlib.h>

// Windows
#ifdef _WIN32
#include <Windows.h>
static inline double get_cpu_time() {
    FILETIME a,b,c,d;
    if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){
        return (double)(d.dwLowDateTime |
            ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    } else {
        return 0;
    }
}

// Posix/Linux
#else
#include <time.h>
#include <sys/time.h>
static inline double get_cpu_time() {
    return (double)clock() / CLOCKS_PER_SEC;
}
#endif

namespace TICTACTOE3D {

///encapsulates a time
class Deadline
{
public:
    ///initializes the time to \p pTime (in microseconds since the epoch)
    explicit Deadline(double pTime=-1)
        :   mTime(pTime)
    {
    }

    ///Times are invalid when they are default-initialised.
    bool isValid() const        {    return mTime!=-1;    }

    ///converts the time to a struct timeval
    void toTimeval(struct timeval &pTime) const
    {
        pTime.tv_sec=((long)(mTime * 1e6))/1000000;
        pTime.tv_usec=((long)(mTime * 1e6))%1000000;
    }

    ///converts the time from this time to \p pUntil to a struct timeval
    void toTimevalUntil(const Deadline &pUntil,struct timeval &pDiff) const
    {
        long lDiff=(long)(pUntil.mTime-mTime * 1e6);
        if(lDiff<=0)
        {
            pDiff.tv_sec=0;
            pDiff.tv_usec=0;
        }
        else
        {
            pDiff.tv_sec=lDiff/1000000;
            pDiff.tv_usec=lDiff%1000000;
        }
    }

    //Returns a Deadline object representing the CPU time in seconds.
    static Deadline now()
    {
        return Deadline(get_cpu_time());
    }

    //Returns the value of this deadline in seconds.
    double getSeconds() const    {    return mTime;    }

    ///comparison operator
    bool operator<(const Deadline &pOther) const
    {
        return mTime<pOther.mTime;
    }

    ///comparison operator
    bool operator<=(const Deadline &pOther) const
    {
        return mTime<=pOther.mTime;
    }

    ///comparison operator
    bool operator>(const Deadline &pOther) const
    {
        return mTime>pOther.mTime;
    }

    ///comparison operator
    bool operator>=(const Deadline &pOther) const
    {
        return mTime>=pOther.mTime;
    }

    ///comparison operator
    bool operator==(const Deadline &pOther) const
    {
        return mTime==pOther.mTime;
    }

    ///comparison operator
    bool operator!=(const Deadline &pOther) const
    {
        return mTime!=pOther.mTime;
    }

    ///adds a delta (in seconds) to this deadline object
    Deadline operator+(double pDelta) const
    {
        return Deadline(mTime+pDelta);
    }

    ///subtracts a delta (in seconds) from this deadline object
    Deadline operator-(double pDelta) const
    {
        return Deadline(mTime-pDelta);
    }

    ///Subtracts one deadline object from another, returning the 
    ///difference in seconds.
    double operator-(const Deadline &pRH) const
    {
        return mTime-pRH.mTime;
    }

private:
    double mTime;
};

/*namespace TICTACTOE3D*/ }

#endif
