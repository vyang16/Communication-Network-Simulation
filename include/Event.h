#ifndef EVENT_H
#define EVENT_H

class Event{
public:
    Event(int _type, int _id, float _time);
    ~Event(){};
    int type; //1 for initiation, 2 for handover, 3 for termination
    float time; //when it happens
    int id;
    int baseStation;
    int dir;
    float speed;
    float duration;
    bool operator <(const Event& e);
};


struct CompareEvent {
    bool operator()(Event const& e1, Event const& e2)
    {
        return e1.time < e2.time;
    }
};
#endif
