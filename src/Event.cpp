#include <Event.h>

Event::Event(int _type, int _id, float _time): type(_type), id(_id), time(_time)
{
};

bool Event::operator <(const Event& e){
  if(this->time < e.time){
    return true;
  }else{
    return false;
  }
}
