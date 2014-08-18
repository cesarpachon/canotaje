#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <gameplay.h>
#include <SerialStream.h>
#include  "tinythread.h"

class Boat;

/**
 * author: cesar pachon, cesarpachon@gmail.com
 * take care of serial communication.
 * configuration is readed from /res/serial.ini file
 * 
 */
class Serial{
  
private:
  gameplay::Properties * m_properties;
  LibSerial::SerialStream m_serialport; 
  tthread::thread * m_thread;
  Boat * m_boat;
  
public:
  Serial();
  virtual ~Serial();
  
  /**
   * this method is intended to be invoked by the thread. use run instead. 
   */
  void init();
  
  /**
   * run calls init */
  void run(Boat * boat);  
  
  int getSleepTime();
  
  /**
   * this method is intended to be invoked only by the thread. 
   */
  void update();
  
};




#endif //_SERIAL_H_