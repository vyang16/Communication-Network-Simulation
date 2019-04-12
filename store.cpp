#include <iostream>
#include <random>
#include <string>
#include <queue>
#include <vector>

#include <Event.h>
#include <Random.h>

using namespace std;

//System Properties
const double WIDTH = 40000.0f; //observed area
const double CH_WIDTH = 2000.0f; //width of a basestation
const int NR_CH = 10; //number of channel per basestation
const int NR_BS = 20; // number of basestation
const int NR_HO = 0; //number of channels reserved for handover
const int CALL_PER_SIM = 10;

//Program variables
int countTotalCalls;
int countDroppedCalls;
int countBlockedCalls;
int nextID;
double currentTime;
vector<int> baseStationList;
priority_queue<Event, vector<Event>, CompareEvent> eventlist;

void init(){
  currentTime = 0;
  countTotalCalls = 0;
  countBlockedCalls = 0;
  countDroppedCalls = 0;
  nextID = 1;

  for(int i = 0; i < NR_BS; i++){
    baseStationList.push_back(NR_CH);
  }
}

int calculateNextStation(int bs, bool direction){
  if(direction){
    return ++bs;
  }else{
    return --bs;
  }
}

void generateCallInitiation(double time){
  //generate random variables
  int bs = getRandomBaseStation();
  bool dir = getRandomBool();
  double position = getRandomPosition();
  double speed = getRandomSpeed();
  double duration = getRandomDuration();
  Event e = Event(1, nextID, time, bs, dir, speed, duration);
  eventlist.push(e);
  cout<<"INITIATION ID" <<nextID<<" time is " <<time<<endl;
  nextID++;
}

void generateCallHandover(int id, double time, int bs, bool dir, double speed, double duration){
  Event e = Event(2, id, time, bs, dir, speed, duration);
  eventlist.push(e);
}

void generateCallTermination(int id, double time, int bs, bool dir, double speed, double duration){
  Event e = Event(3, id, time, bs, dir, speed, duration);
  eventlist.push(e);
}

void processCallInitiation(const Event& e){
  Event nextEvent = e;
  //determine when the next call initiation happens and put it in the eventlist
  double intArTime = getRandomIntArTime();
  generateCallInitiation(nextEvent.time + intArTime);
  //one call is processed.
  countTotalCalls++;

  //decide what the next event is: drop, handover or termination
  if(baseStationList[nextEvent.baseStation] <= NR_HO){
    //drop call
    countDroppedCalls++;
  }else{
    baseStationList[nextEvent.baseStation]--;
    //decide if termination or handover
    int nextStation = calculateNextStation(nextEvent.baseStation, nextEvent.dir);
    double position = getRandomPosition();
    double timeToSwitchLines = (position*3.6)/nextEvent.speed;
    if(timeToSwitchLines > nextEvent.duration || nextStation >= NR_BS || nextStation < 0){
      //termination
      nextEvent.time += nextEvent.duration;
      nextEvent.type = 3;
    }else{
      //handover
      nextEvent.type = 2;
      nextEvent.time += timeToSwitchLines;
      nextEvent.duration -= timeToSwitchLines;
    }
    eventlist.push(nextEvent);
  }

}

void processCallHandover(const Event& e){
  //release previous stations
  Event nextEvent = e;
  baseStationList[nextEvent.baseStation]++;
  nextEvent.baseStation = calculateNextStation(nextEvent.baseStation, e.dir);
  if(baseStationList[nextEvent.baseStation] <= NR_HO){
    countBlockedCalls++;
  }else{
    baseStationList[nextEvent.baseStation]--;
    int nextStation = calculateNextStation(nextEvent.baseStation, e.dir); //another handover?
    int timeToSwitchLines = (CH_WIDTH*3.6)/nextEvent.speed;
    if(timeToSwitchLines < nextEvent.duration){
      if(nextStation < NR_BS && nextStation >= 0){
        //handover
        nextEvent.type = 2;
        nextEvent.time += timeToSwitchLines;
        nextEvent.duration -= timeToSwitchLines;
      }else{
        //else leaves observed area
        nextEvent.type = 3;
        nextEvent.time += timeToSwitchLines;
      }
    }else{
      //termination
      nextEvent.type = 3;
      nextEvent.time += nextEvent.duration;
    }
    eventlist.push(nextEvent);
  }
}

void processCallTermination(const Event& e){
  baseStationList[e.baseStation]++;
}

void process(const Event& event){
  if(currentTime > event.time){
    //cout<<"TIME ERROR"<<endl;
  }

  currentTime = event.time;
  switch(event.type){
    case 1:
      processCallInitiation(event);
      break;
    case 2:
      processCallHandover(event);
      break;
    case 3:
      processCallTermination(event);
      break;
    default:
      std::cout<<"Do not know this event type!\n";
      break;
  }
}

//Main functionprocessCallInitiation()
int main(int argc, char *argv[]){
  init();

  eventlist.push(Event(1, 1,0.1, 1, true, 0.32, 32.4));
  eventlist.push(Event(1, 2, 0.2, 1, true, 0.32, 32.4));
  eventlist.push(Event(2, 1, 0.4, 2, true, 0.32, 32.4));

  while(!eventlist.empty()){
    Event e = eventlist.top();
    eventlist.pop();
    cout<<"Event ID: "<<e.id<< " Event Type: "<<e.type<<" currentTime: "<<currentTime<<endl;
  }
  return 0;
}
