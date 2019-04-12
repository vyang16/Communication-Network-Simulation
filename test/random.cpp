#include <random>
#include <cstdlib>
#include <math.h>
#include <iostream>
//Input anaylsis constants
//const double BETA_INTARTIME = 1.3698169264765276;
const double BETA_INTARTIME = 0.01;
const double BETA_CALLDURATION = 109.8359007387477;
const double MU_VEL = 120.07209801685764;
const double VAR_VEL = 81.34340536561687;


//Functions for generating random numbers
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> uni(0.0,1.0);
std::exponential_distribution<double> iat(1/BETA_INTARTIME);
std::exponential_distribution<double> cd(1/BETA_CALLDURATION);
std::normal_distribution<double> vel{MU_VEL,sqrt(VAR_VEL)};

bool getRandomBool(){
  return uni(mt) > 0.5;
}

int getRandomBaseStation(){
  return rand()%20;
}

double getRandomPosition(){
  return uni(mt)*2000.0;
}

double getRandomSpeed(){
  return vel(mt);
}

double getRandomDuration(){
  return cd(mt);
}

double getRandomIntArTime(){
  return iat(mt);
}

int main(int argc, char const *argv[]) {
  for(int i = 0; i < 20; i++){
    std::cout << getRandomIntArTime() << std::endl;
  }
  return 0;
}
/*
init();
generateCallInitiation(currentTime);
while(!eventlist.empty() && countTotalCalls < CALL_PER_SIM){
  if(eventlist.empty()){
    cout<<"empty"<<endl;
    break;
  }
  Event e = eventlist.top();
  eventlist.pop();
  process(e);
  printVector(baseStationList);
  //cout<<"Event ID: "<<e.id<< " Event Type: "<<e.type<<" Eventtime: "<<e.time<<endl;
}
//cout<<"Number Blocked Calls: "<<countBlockedCalls<<endl;
//cout<<"Number Dropped Calls: "<<countDroppedCalls<<endl;
return 0;


init();
generateCallInitiation(0.0);
for(int i=0; i<100; i++){
  if(eventlist.empty())
    break;
  Event e = eventlist.top();
  eventlist.pop();
  cout<<"Event ID: "<<e.id<< " Event Type: "<<e.type<<" currentTime: "<<currentTime<<endl;
  process(e);
}
*/
