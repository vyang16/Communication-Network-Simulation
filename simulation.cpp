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

void generateCallInitiation(int time){
  cout<<"init"<<endl;
  Event e = Event(1, nextID, time);
  eventlist.push(e);
  nextID++;
}
void generateCallHandover(int id, double time, int bs, bool dir, double speed, double duration){
  Event e = Event(2, id, time, bs, dir, speed, duration);
  eventlist.push(e);
}

void generateCallTermination(int id, double time, int bs){
  Event e = Event(3, id, time, bs);
  eventlist.push(e);
}

void processCallInitiation(int id, double now){
  countTotalCalls++;
  //Generate random VARIABLE for base station
  int bs = getRandomBaseStation();
  //decide what the next event is: drop, handover or termination

  if(baseStationList[bs] <= NR_HO){
    //drop call
    countDroppedCalls++;
  }else{
    //generate other random variables
    bool dir = getRandomBool();
    double position = getRandomPosition();
    double speed = getRandomSpeed();
    double duration = getRandomDuration();
    baseStationList[bs]--;
    //decide if termination or handover
    int nextStation = calculateNextStation(bs, dir);
    double timeToSwitchLines = (position*3.6)/speed;
    if(timeToSwitchLines > duration || nextStation >= NR_BS || nextStation < 0){
      cout<<"termination"<<endl;
      generateCallTermination(id, now+duration, bs);
    }else{
      cout<<"handover"<<endl;
      generateCallHandover(id, now+timeToSwitchLines, bs, dir, speed, duration - timeToSwitchLines);
    }
  }
  //determine when the next call initiation happens and put it in the eventlist
  //double intArTime = getRandomIntArTime();
  //generateCallInitiation(currentTime + intArTime);
}

void processCallHandover(int id, double now, int bs, bool dir, double speed, double duration){
  //release previous stations
  baseStationList[bs]++;
  bs = calculateNextStation(bs, dir);
  if(baseStationList[bs] <= NR_HO){
    countBlockedCalls++;
  }else{
    baseStationList[bs]--;
    int nextStation = calculateNextStation(bs, dir);
    int timeToSwitchLines = (CH_WIDTH*3.6)/speed;
    if(timeToSwitchLines < duration){
      if(nextStation < NR_BS && nextStation >= 0){
        generateCallHandover(id, now+timeToSwitchLines, bs, dir, speed, duration - timeToSwitchLines);
      }else{
        //else leaves observed area
        generateCallTermination(id, now+timeToSwitchLines, bs);
      }
    }else{
      generateCallTermination(id, now+duration, bs);
    }
  }
}

void processCallTermination(int baseStation){
  baseStationList[baseStation]++;
  cout<<"terminated"<<endl;
}

void process(Event& event){
  currentTime = event.time;
  switch(event.type){
    case 1:
      processCallInitiation(event.id, currentTime);
      break;
    case 2:
      processCallHandover(event.id, currentTime, event.baseStation, event.dir, event.speed, event.duration);
      break;
    case 3:
      processCallTermination(event.baseStation);
      break;
    default:
      std::cout<<"Do not know this event type!\n";
      break;
  }
}

//Main functionprocessCallInitiation()
int main(int argc, char *argv[]){
  //Create eventlist
  init();
  generateCallInitiation(currentTime);
  for(int i=0; i<100; i++){
    if(eventlist.empty())
      break;
    Event e = eventlist.top();
    eventlist.pop();
    cout<<"Event ID: "<<e.id<< " Event Type: "<<e.type<<"currentTime: "<<currentTime<<endl;
    process(e);
  }
  generateCallInitiation(currentTime);
  for(int i=0; i<100; i++){
    if(eventlist.empty())
      break;
    Event e = eventlist.top();
    eventlist.pop();
    cout<<"Event ID: "<<e.id<< " Event Type: "<<e.type<<"currentTime: "<<currentTime<<endl;
    process(e);
  }

  return 0;
}
