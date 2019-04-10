#ifndef COMPAREEVENTS_H
#define COMPAREEVENTS_H

#include <Event.h>

class CompareEvents{
public:
  bool operator() (const Event& event1, const Event& event2) {
    //comparison code here
    return(event1.time < event2.time);
  }
};
#endif
