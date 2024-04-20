#ifndef MY_TIME_H
#define MY_TIME_H
#define CV_TIME 0

#if CV_TIME
   #define TYPE_OF_TIME "opencv"
   #include <opencv2/opencv.hpp>
   typedef int64 myTime;
#elif WIN32
   #define TYPE_OF_TIME "clock_t"
   #include <time.h>
   typedef clock_t myTime;
#else
   #define TYPE_OF_TIME "std::chrono"
   #include <chrono>
   typedef std::chrono::time_point<std::chrono::system_clock> myTime;
#endif

myTime myTimeInit();
double myTimeDurationMs(myTime & begin, myTime & end);

#endif //MY_TIME_H
