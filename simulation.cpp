#include <iostream>
#include <random>
#include <string>
#include <queue>
#include <vector>
#include <fstream>

#include <Event.h>
#include <Random.h>

using namespace std;
//debugging
vector<int> wuvec;

//output statistics
vector<int> droppedCallsVector;
vector<int> blockedCallsVector;

//System Properties
const double WIDTH = 40000.0f; //observed area
const double CH_WIDTH = 2000.0f; //width of a basestation
const int NR_CH = 10; //number of channel per basestation
const int NR_BS = 20; // number of basestation
const int NR_HO = 0; //number of channels reserved for handover
const int WARMUP = 2000;
const int CALL_PER_SIM = 10000 + WARMUP;
const int NR_SIMULATION = 100;
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
  //cout<<"INITIATION ID" <<nextID<<" time is " <<time<<endl;
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
  //one call is processed.
  countTotalCalls++;



  //decide what the next event is: drop, handover or termination
  if(baseStationList[nextEvent.baseStation] <= NR_HO){
    //drop call
    countBlockedCalls++;
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
  //determine when the next call initiation happens and put it in the eventlist
  double intArTime = getRandomIntArTime();
  generateCallInitiation(e.time + intArTime);

}

void processCallHandover(const Event& e){
  //release previous stations
  Event nextEvent = e;
  baseStationList[nextEvent.baseStation]++;
  nextEvent.baseStation = calculateNextStation(nextEvent.baseStation, e.dir);
  if(baseStationList[nextEvent.baseStation] <= 0){
    countDroppedCalls++;
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
  currentTime = event.time;
  switch(event.type){
    case 1:
      processCallInitiation(event);
      //cout<<"INITIATION EVENT ID "<<event.id<<" EVENT TIME "<<event.time<<endl;
      break;
    case 2:
      processCallHandover(event);
      //cout<<"HANDOVER EVENT ID "<<event.id<<" EVENT TIME "<<event.time<<endl;
      break;
    case 3:
      processCallTermination(event);
      //cout<<"TERMINATION EVENT ID "<<event.id<<" FINISHED AT "<<event.time<<endl;
      break;
    default:
      std::cout<<"Do not know this event type!\n";
      break;
  }
}
void printVector(const vector<int>& path){
  int result = 0;
  for (std::vector<int>::const_iterator i = path.begin(); i != path.end(); ++i){
    result += *i;
  }
  wuvec.push_back(result);
    //cout<<"\n\n";
    //cout << *i << ' ';
    // if(*i < 5){
    //   cout <<*i<<" ";
    //   cout<<"\n\n";
    // }

}

void writeVector(const vector<int>& vec, string filename){
  ofstream myfile;
  string path = "/home/viviane/Programming/Communication-Network-Simulation/log/";
  path.append(filename);
  myfile.open(path.c_str());
  for(std::vector<int>::const_iterator i = vec.begin(); i != vec.end(); ++i){
    myfile<<*i<<",";
  }
  myfile.close();
}

void resetCounter(){
  countTotalCalls = 0;
  countBlockedCalls = 0;
  countDroppedCalls = 0;
}

void runSimulation(){
  init();

  generateCallInitiation(currentTime);
  int counter = 0;
  while(!eventlist.empty() && countTotalCalls < CALL_PER_SIM){
    if(counter == WARMUP){
      //cout<<"Warmup reached"<<endl;
      resetCounter();
    }
    Event e = eventlist.top();
    eventlist.pop();
    process(e);
    counter++;
    //printVector(baseStationList);
    //cout<<"Event ID: "<<e.id<< " Event Type: "<<e.type<<" Eventtime: "<<e.time<<endl;
  }
  cout<<countTotalCalls<<" "<<countBlockedCalls<<" "<<countDroppedCalls<<endl;
  blockedCallsVector.push_back(countBlockedCalls);
  droppedCallsVector.push_back(countDroppedCalls);
}

//Main functionprocessCallInitiation()
int main(int argc, char *argv[]){
  for(int i = 0; i < NR_SIMULATION; i++){
    runSimulation();
    init();
  }
  writeVector(blockedCallsVector, "blockedCalls.csv");
  writeVector(droppedCallsVector, "droppedCalls.csv");
  return 0;
}
