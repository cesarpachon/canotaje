#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <SerialStream.h>

using namespace LibSerial; 

SerialStream m_serialport; 
 
const int BUFFER_SIZE = 4 ;

int main( int argc, char *argv[] )
{
 if(argc != 2){
 	printf("debe pasar como argumento el puerto serial virtual al que se quiere conectar.");
 	exit(1);
 }
 
 const char * port = argv[1];
 
 printf("conectando a %s\n", port);
 
  m_serialport.Open( port ) ;
     if ( ! m_serialport.good() )
     {
      printf( "cant open port %s\n", port );
      exit(1);
     }
     
     m_serialport.SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
     if ( ! m_serialport.good() )
     {
      printf("cant set baudrate %s\n", port );
      exit(1);
     }
     
     m_serialport.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
     if ( ! m_serialport.good() )
     {
      printf("cant set charsize %s\n", port );
      exit(1);
     }
     
      m_serialport.SetParity( SerialStreamBuf::PARITY_NONE ) ;
     if ( ! m_serialport.good() )
     {
      printf( "cant set parity %s\n", port );
         exit(1) ;
     }
     
		char buffroll[BUFFER_SIZE] ; 
		char buffyaw[BUFFER_SIZE] ; 
     while(1){
     	usleep(100);
		 m_serialport.read( buffroll, BUFFER_SIZE ) ;
		 m_serialport.read( buffyaw, BUFFER_SIZE ) ;
      printf( "readed: %s %s\n", buffroll, buffyaw );

     }       
 
 exit(0);
}
