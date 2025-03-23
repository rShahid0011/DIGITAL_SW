/********************************************************************/
/********************************************************************/
/*****                                                          *****/
/*****        L A B C E N T E R    E L E C T R O N I C S        *****/
/*****                                                          *****/
/*****                  VFP Server for ESP8266                  *****/
/*****                                                          *****/
/*****                    Bridge TCP Sockets                    *****/
/*****                                                          *****/
/********************************************************************/
/********************************************************************/

#pragma once

#include <arduino.h>
#include <WiFiServer.h>
#include <WiFiClient.h>


#include "bridge.h"
#include "command.h"


#define MAX_SOCKETS 4

class Sockets : public Runner
 { public:
      Sockets(CommandProcessor *cp);
      ~Sockets ();
      void reset();
      
      // CommandHandlers
      String LISTEN_Command (const String &data);
      String ACCEPT_Command (const String &data);
      String CONNECT_Command (const String &data);
      String CONNECTED_Command (const String &data);
      String CONNECTING_Command (const String &data);
      String READ_Command (const String &data);
      String WRITE_Command (const String &data);
      String WRITE_TO_ALL_Command (const String &data);
      String CLOSE_Command (const String &data);

      
   private:
      int getId (const String &data) { return (unsigned char)(data[1]); }
      int getPort (const String &data) { return ((unsigned char)(data[1])<<8) + (unsigned char)(data[2]); }
      bool isSocket (int id) { return id >= 0 && id < MAX_SOCKETS && sockets[id]; }
      
   private:
       WiFiServer *server = nullptr;
       WiFiClient sockets[MAX_SOCKETS];
         
 };
 
