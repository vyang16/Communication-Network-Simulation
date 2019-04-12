#include <Event.h>

Event::Event(int _type, int _id, double _time, int _baseStation, bool _dir, double _speed, double _duration)
: type(_type), id(_id), time(_time), baseStation(_baseStation), dir(_dir), speed(_speed), duration(_duration)
{
};
