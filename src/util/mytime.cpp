#include "mytime.hpp"

#if CV_TIME
   myTime myTimeInit(){
       return cv::getTickCount();
   }

   double myTimeDurationMs(myTime & begin, myTime & end){
       double measureTime = cv::getTickFrequency();
      double duration = ((double)(end - begin))/measureTime*1000;
      return duration;
   } 
#elif WIN32
   myTime myTimeInit(){
      return clock();
   }

   double myTimeDurationMs(myTime & begin, myTime & end){
      return (begin - end) * 1000 / CLOCKS_PER_SEC;
   }
#else
   myTime myTimeInit(){
      return std::chrono::system_clock::now();
   }

   double myTimeDurationMs(myTime & begin, myTime & end){
      return std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
   }
#endif
