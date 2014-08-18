#include <string>
#include <sstream>
#include <stdio.h>
#include "mathutil.h"
#include "serial.h"
#include "boat.h"

using namespace gameplay;
using namespace LibSerial;
using namespace tthread;

void networkThread(void * aArg)
{  
   Logger::log(Logger::LEVEL_INFO, "starting network thread\n");
   Serial * serial = static_cast<Serial *> (aArg);
   serial->init();
   int sleeptime = serial->getSleepTime();
   
   //wait 3 seconds before starting polling
   usleep(3000);
   
   while(true){
     usleep(sleeptime);
     //Logger::log(Logger::LEVEL_INFO, "hello thread!" );
     serial->update();
  }
   
}

Serial::Serial(){
    
};
  
Serial::~Serial(){
    SAFE_DELETE(m_properties);
    if(m_thread) delete(m_thread);
};

void Serial::update()
{
  int roll = (int)(m_boat->getRoll()*M_RAD_TO_DEG);
  int yaw = (int)(m_boat->getYaw()*M_RAD_TO_DEG);
  char buffroll[4], buffyaw[4];
  std::sprintf(buffroll, "%d", roll);
  std::sprintf(buffyaw, "%d", yaw);  
  m_serialport.write(buffroll, 4);
  m_serialport.write(buffyaw, 4);
    Logger::log(Logger::LEVEL_INFO, "Serial::update %s %s %d %d %f %f\n", buffroll, buffyaw, roll, yaw, m_boat->getRoll(), m_boat->getYaw());
};

void Serial::run(Boat * boat){
  m_boat = boat;
  m_thread = new thread(networkThread, this);
};
  
void Serial::init(){
    m_properties = Properties::create("serial.config");    

    const char * port = m_properties->getString("port");
    Logger::log(Logger::LEVEL_INFO, "serial port is %s\n", port );
    
    m_serialport.Open( port ) ;
     if ( ! m_serialport.good() )
     {
      Logger::log(Logger::LEVEL_ERROR, "cant open port %s\n", port );
      exit(1);
     }
     
     m_serialport.SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
     if ( ! m_serialport.good() )
     {
      Logger::log(Logger::LEVEL_ERROR, "cant set baudrate %s\n", port );
      exit(1);
     }
     
     m_serialport.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
     if ( ! m_serialport.good() )
     {
      Logger::log(Logger::LEVEL_ERROR, "cant set charsize %s\n", port );
      exit(1);
     }
     
      m_serialport.SetParity( SerialStreamBuf::PARITY_NONE ) ;
     if ( ! m_serialport.good() )
     {
      Logger::log(Logger::LEVEL_ERROR, "cant set parity %s\n", port );
         exit(1) ;
     }       
};

int Serial::getSleepTime()
{
  return m_properties->getInt("sleep");
};
