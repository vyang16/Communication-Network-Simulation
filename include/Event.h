#ifndef EVENT_H
#define EVENT_H

class Event{
public:
    Event(int _type, int _id, double _time); //for Call INITIATION
    Event(int _type, int _id, double _time, int _baseStation, bool _dir, double _speed, double _duration); //for Call HANDOVER
    Event(int _type, int _id, double _time, int _baseStation); //for Call TERMINATION
    ~Event(){};
    int type; //1 for initiation, 2 for handover, 3 for termination
    int id;
    double time; //when it happens
    int baseStation;
    bool dir;
    double speed;
    double duration;
    bool operator <(const Event& e);
};


struct CompareEvent {
    bool operator()(Event const& e1, Event const& e2)
    {
        return e1.time < e2.time;
    }
};
#endif
