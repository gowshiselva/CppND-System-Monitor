#include <string>
#include <sstream>
#include <iostream>
#include "format.h"
#include <iomanip>

using std::string;




string Format::ElapsedTime(long times) { 
    std::ostringstream time_;
    long hours=times /3600;
    long remainder=times % 3600;
    long minutes=remainder / 60;
    long seconds=remainder % 60;
    
    time_ << std::setfill('0') << std::setw(2) << hours  << ":"  << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;

    return time_.str();
 }