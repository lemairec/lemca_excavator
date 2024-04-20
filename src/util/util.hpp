#ifndef UTIL_H
#define UTIL_H

#include "environnement.hpp"
#include <fstream>
#include <memory>
#include <list>
#include <string>
#include <vector>
#include <time.h>
#include <unistd.h>

#include "log.hpp"

const double c_pi = 3.14159265;

#if WIN32
#define mysprintf sprintf_s
#else
#define mysprintf snprintf
#endif


void mySleep(int durationMilliSecond);
int myRound(double value);
void callDebugger();
double my_map(double x, double in_min, double in_max, double out_min, double out_max);

std::string execute2(std::string cmd);

double my_angle(double x1, double y1, double x2, double y2);
double angleBetweenMPiPi(double a);
double angleBetweenMDemiPiDemiPi(double a);

std::wstring string_to_w (const std::string& str);
std::string w_to_string (const std::wstring& str);

int getHexIntWithChar(char c);
char getHex(int a);


template<typename ... Args>
std::string strprintf( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

#endif
