#include <iostream>
#include <random>
#include <string>
#include <queue>
#include <vector>

#include <Event.h>


using namespace std;

const float WIDTH = 40000.0f; //observed area
const float CH_WIDTH = 2000.0f; //width of a basestation
const int NR_CH = 10; //number of channel per basestation
const int NR_BS = 20; // number of basestation
const int NR_HO = 1; //number of channels reserved for handover
const int CALL_PER_SIM = 10;

int countTotalCalls;
int countDroppedCalls;
int countBlockedCalls;
int nextID;
float currentTime;
vector<int> baseStationList;
priority_queue<Event, vector<Event>, CompareEvent> eventlist;

void init(priority_queue<Event, vector<Event>, CompareEvent>& list){
  currentTime = 0;
  countTotalCalls = 0;
  countBlockedCalls = 0;
  countDroppedCalls = 0;
  nextID = 1;

  for(int i = 0; i < NR_BS; i++){
    baseStationList[i] = 0;
  }

  Event firstEvent = Event(1, countTotalCalls, currentTime);
  eventlist.push(firstEvent);
}

int calculateNextStation(int bs, bool direction){
  if(direction){
    return ++bs;
  }else{
    return --bs;
  }
}

void generateCallInitiation(int time){
  Event e = Event(1, nextID, time);
  eventlist.push(e);
  nextID++;
}
void generateCallHandover(int id, float time, int bs, bool dir, float speed, float duration){
  Event e = Event(2, id, time, bs, dir, speed, duration);
  eventlist.push(e);
}

void generateCallTermination(int id, float time, int bs){
  Event e = Event(3, id, time, bs);
  eventlist.push(e);
}

void processCallInitiation(int id, float now){
  countTotalCalls++;
  //Generate random VARIABLE for base station
  int bs = rand()%21 + 1;


  //decide what the next event is: drop, handover or termination
  if(baseStationList[bs] <= NR_HO){
    //drop call
    countDroppedCalls++;
  }else{
    //generate other random variables
    bool dir = 1;
    float position = 0.1;
    float speed = 0.1;
    float duration = 0.1;
    baseStationList[bs]--;
    //decide if termination or handover
    int nextStation = calculateNextStation(bs, dir);
    float timeToSwitchLines = (position*3.6)/speed;
    if(timeToSwitchLines > duration || nextStation >= NR_BS || nextStation < 0){
      generateCallTermination(id, now+duration, bs);
    }else{
      generateCallHandover(id, now+timeToSwitchLines, bs, dir, speed, duration - timeToSwitchLines);
    }
  }
  //determine when the next call initiation happens and put it in the eventlist
  float intArTime = 0.1;
  generateCallInitiation(currentTime + intArTime);
}

void processCallHandover(float now, int id, int baseStation, bool dir, float speed, float duration){
  //
}

void processCallTermination(int baseStation){
  baseStationList[baseStation]++;
}

void process(Event& event){
  currentTime = event.time;
  switch(event.type){
    case 1:
      processCallInitiation(event.id, currentTime);
      break;
    case 2:
      processCallHandover(currentTime, event.id, event.baseStation, event.dir, event.speed, event.duration);
    case 3:
      processCallTermination(event.baseStation);
      break;
    default:
      std::cout<<"Do not know this event type!\n";
  }
}

//Main functionprocessCallInitiation()
int main(int argc, char *argv[]){

  //Create eventlist
  init(eventlist); //insert first element, set all variables
  while(!eventlist.empty() && countTotalCalls < CALL_PER_SIM){
    Event e = eventlist.top();
    process(e);
    eventlist.pop();
  }
  return 0;
}
