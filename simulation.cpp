#include <iostream>
#include <random>
#include <string>
#include <queue>
#include <vector>


#include <Event.h>
#include <CompareEvents.h>
#include <BaseStation.h>

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
double currentTime;
vector<BaseStation> baseStation;

void init(priority_queue<Event, vector<Event>, CompareEvent>& list){
  currentTime = 0;
  countTotalCalls = 0;
  countBlockedCalls = 0;
  countDroppedCalls = 0;

  for(int i = 0; i < NR_BS; i++){
    baseStation.push_back(BaseStation(10, NR_HO));
  }

  Event firstEvent = Event(1, currentTime, countTotalCalls);
  list.push(firstEvent);
}


void generateCallInitiation(double now){
  countTotalCalls++;
  //Generate random VARIABLES


  //decide what the next event is: drop, handover or termination

  //Generate next call initiation
}

void generateCallHandover(double now, int id, int baseStation, int dir, float speed, float duration){
  //
}

void generateCallTermination(int baseStation){

}

void process(Event& event){
  currentTime = event.time;
  switch(event.type){
    case 1:
      generateCallInitiation(currentTime);
      break;
    case 2:
      generateCallHandover(currentTime, event.id, event.baseStation, event.dir, event.speed, event.duration);
    case 3:
      generateCallTermination(event.baseStation);
      break;
    default:
      std::cout<<"Do not know this event type!\n";
  }
}

//Main function
int main(int argc, char *argv[]){

  //Create eventlist
  priority_queue<Event, vector<Event>, CompareEvent> eventlist;
  init(eventlist); //insert first element, set all variables
  while(!eventlist.empty() && countTotalCalls < CALL_PER_SIM){
    Event e = eventlist.top();
    process(e);
    eventlist.pop();
  }

}
