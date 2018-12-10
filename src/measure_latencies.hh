#pragma once

#include <iostream>
#include <stdlib.h>

#include <sstream>


#define MEASURE_LATENCIES_PRINT 1

#if MEASURE_LATENCIES_PRINT == 1
    #define INIT_COUNTING struct timespec start_time, end_time;
    #define START_COUNTING clock_gettime(CLOCK_MONOTONIC, &start_time);
#else
    #define INIT_COUNTING {}
    #define START_COUNTING {}
#endif


#if MEASURE_LATENCIES_PRINT == 1
    #define STOP_COUNTING_PRINT(msg) { clock_gettime(CLOCK_MONOTONIC, &end_time); \
                                double duration_ns = (end_time.tv_sec > start_time.tv_sec ? (1e9-start_time.tv_nsec + end_time.tv_nsec ) : end_time.tv_nsec - start_time.tv_nsec) ;\
                                stringstream ss;\
                                ss << msg << ": "<< duration_ns/1e6 <<"ms" <<endl; \
                            cout <<ss.str(); }
#else
    #define STOP_COUNTING_PRINT(msg) {}
#endif

