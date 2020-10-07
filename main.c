#include <signal.h>
#include <stdio.h>
#include <string.h>

#include <time.h>
#include <sys/time.h>

#define LOOP 10000

unsigned int iGlobalTick = 0;

double fDoubleSquare(double src, int cnt);
int createTimer(timer_t* timerID, int sec, int msec);

int main(int argc, char* argv[]) 
{ 
    double res = 0.0f;
    timer_t timerID;

    createTimer(&timerID, 0, 1);

    for (int i = 0; i <= LOOP; i++)
    {
        res = res + (((4.0f * fDoubleSquare(1.0f / 5.0f, (2 * i) + 1)) - fDoubleSquare(1.0f / 239.0f, (2 * i) + 1)) * ((4.0f * fDoubleSquare(-1.0f, i)) / ((2.0f * (double)i) + 1.0f)));
    }

    printf("time = %.4f\r\n", ((float)iGlobalTick / 1000.0f));
    printf("res  = %.60f\r\n", res);

    return 0;
}

double fDoubleSquare(double src, int cnt)
{
	double res = src;
	if (cnt > 1) {
		for (int i = 2; i <= cnt; i++) {
			res = res * src;
		}
	}
	else if (cnt == 1) {
		res = src;
	}
	else if (cnt == 0) {
		res = 1.0f;
	}

	return res;
}

// 타이머 주기에 따라 호출될 타이머
void timer()
{
    iGlobalTick++;
}

int createTimer(timer_t* timerID, int sec, int msec)
{
    struct sigevent         te;
    struct itimerspec       its;
    struct sigaction        sa;
    int                     sigNo = SIGRTMIN;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer;
    sigemptyset(&sa.sa_mask);

    if (sigaction(sigNo, &sa, NULL) == -1)
    {
        printf("sigaction error\n");
        return -1;
    }

    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = timerID;
    timer_create(CLOCK_REALTIME, &te, timerID);

    its.it_interval.tv_sec = sec;
    its.it_interval.tv_nsec = msec * 1000000;
    its.it_value.tv_sec = sec;
    its.it_value.tv_nsec = msec * 1000000;

    timer_settime(*timerID, 0, &its, NULL);

    return 0;
}