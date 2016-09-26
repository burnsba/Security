// apache license, etc etc

// compile with: gcc -O0 -msse2 capture.c

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <xmmintrin.h>

static void sigaction_sfpe(int signal, siginfo_t *si, void *arg)
{
    printf("inside SIGFPE handler\nexit now.");
    exit(1);
}

int main()
{
    struct sigaction sa;

    // setup signal handler
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sigaction_sfpe;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGFPE, &sa, NULL);

    // bits 7-12 are exception masks.
    // bit 9 is DBZ
    //_mm_setcsr(0x00001D80); // ignore all FPE except DBZ
    //_mm_setcsr(0x00001F80); // ignore all FPE

    // this will trigger DBZ FPE
    __m128 s1, s2;
    s1 = _mm_set_ps(1.0, 1.0, 1.0, 1.0);
    s2 = _mm_set_ps(0.0, 0.0, 0.0, 0.0);
    s2 = _mm_div_ss(s1, s2); // needs "s2 = " or gcc will optimize out, even with "-O0"

    printf("done (no error).\n");

    return 0;
}
