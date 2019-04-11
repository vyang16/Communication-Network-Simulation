#include <Event.h>

Event::Event(int _type, int _id, float _time): type(_type), id(_id), time(_time)
{
};

Event::Event(int _type, int _id, float _time, int _baseStation, bool _dir, float _speed, float _duration)
: type(_type), id(_id), time(_time), baseStation(_baseStation), dir(_dir), speed(_speed), duration(_duration)
{
};

Event::Event(int _type, int _id, float _time, int _baseStation): type(_type), id(_id), time(_time), baseStation(_baseStation)
{
};

bool Event::operator <(const Event& e){
  if(this->time < e.time){
    return true;
  }else{
    return false;
  }
}
