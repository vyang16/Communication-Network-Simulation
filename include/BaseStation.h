#ifndef BASESTATION_H
#define BASESTATION_H


class BaseStation{

public:
  //Constructor
  BaseStation(int _availableChannel, int _reservedChannel);
  ~BaseStation(){};
  bool hasFreeChannel();
  int availableChannel;
  int reservedChannel;
};

#endif
