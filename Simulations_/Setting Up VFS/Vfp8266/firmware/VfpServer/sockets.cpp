/********************************************************************/
/********************************************************************/
/*****                                                          *****/
/*****        L A B C E N T E R    E L E C T R O N I C S        *****/
/*****                                                          *****/
/*****                  VFP Server for ESP8266                  *****/
/*****                                                          *****/
/*****                   Bridge TCP Sockets                     *****/
/*****                                                          *****/
/********************************************************************/
/********************************************************************/

#include "Bridge.h"
#include "Sockets.h"

// N.B. Unlike the VSM model, the data strings contain the command code at byte 0,
//      so the socket id parameter is data[1], not data[0] etc.

Sockets::Sockets (CommandProcessor *cp) : Runner()
 { cp->registerCommand('N', this, (CommandHandler)&Sockets::LISTEN_Command);
   cp->registerCommand('k', this, (CommandHandler)&Sockets::ACCEPT_Command);
   cp->registerCommand('K', this, (CommandHandler)&Sockets::READ_Command); 
   cp->registerCommand('l', this, (CommandHandler)&Sockets::WRITE_Command);
   cp->registerCommand('L', this, (CommandHandler)&Sockets::CONNECTED_Command);
   cp->registerCommand('j', this, (CommandHandler)&Sockets::CLOSE_Command);
   cp->registerCommand('c', this, (CommandHandler)&Sockets::CONNECTING_Command);
   cp->registerCommand('C', this, (CommandHandler)&Sockets::CONNECT_Command);
   cp->registerCommand('b', this, (CommandHandler)&Sockets::WRITE_TO_ALL_Command);   
 }

Sockets::~Sockets ()
 { reset();
 }

void Sockets::reset ()
 { for (int i=0; i<MAX_SOCKETS; ++i)
      if (isSocket(i))
         sockets[i].stop();
    delete server;
    server = nullptr;         
 }
 
String Sockets::LISTEN_Command (const String &data)
// Listen for connections on a specified port.
// Only one server is supported and we assume localhost (addr is ignored)
 { int port = getPort(data); 
   if (server)
      delete server;
   server = new WiFiServer(port);
   server->begin();
   return byteResult(server->status() != CLOSED); // 1 good, 0 bad

 }

String Sockets::ACCEPT_Command (const String &data)
// Accept a connection to the server. This creates a new client/socket
 { int id = -1;
   if (server->hasClient())
    { for (int i=0; i<MAX_SOCKETS; ++i)
         if (!isSocket(i))
          { sockets[id=i] = server->available();
            break;
          }
    }
   return id != -1 ? byteResult(id) : voidResult();
 }

String Sockets::CONNECT_Command (const String &data)
// Create a new client socket and attempt to connect to a url/port
 { int port = getPort(data);
   String url = data.substring(3);
   
   int id = -1;
   for (int i=0; i<MAX_SOCKETS; ++i)
      if (!isSocket(i))
       { sockets[id=i].connect(url, port);
         break;
       }
   
   debug.println("OPEN: "+url+":"+String(port)+" as "+String(id));
         
   return id != -1 ? byteResult(id) : voidResult();
 }

String Sockets::CONNECTING_Command (const String &data)
// Returns true/false if the specified socket is in the process of connecting.
// N.B. not clear if URL resolution is asynchronous, assumed not so connecting must mean we are in the SYN/handshake phases.
// If this has issues, we could just check that the socket id is in use.
 { int id = getId(data);
   bool connecting = false;
   if (isSocket(id))
    { uint8_t state = sockets[id].status();
      connecting = (state == SYN_SENT || state == SYN_RCVD);
    }
   return byteResult(connecting);
 }

String Sockets::CONNECTED_Command (const String &data)
// Returns true/false if the specified socket is connected.
 { int id = getId(data);
   bool connected = false;
   if (isSocket(id))
       connected = sockets[id].connected();
   return byteResult(connected);
 }

String Sockets::CLOSE_Command (const String &data)
// Close/delete the specified socket - the application is done with it.
 { int id = getId(data);
   if (isSocket(id))
    { debug.println("CLOSE: "+String(id));
      sockets[id].stop();
    }
   return voidResult();
 }

String Sockets::READ_Command (const String &data)
// Read up to maxlen bytes from the socket.
 { int id = getId(data);
   int maxlen = data[2], count;
   String result;
   if (isSocket(id) && sockets[id].available())
    { uint8_t *buffer = (uint8_t *)malloc(maxlen);
      count = sockets[id].read(buffer,  maxlen);
      for (int i=0; i<count; ++i)
         result += char(buffer[i]);
      free(buffer);    
      debug.println("READ: "+String(count)+" bytes");
    }
   return result;
 }

String Sockets::WRITE_Command (const String &data)
// Write data to the socket.
 { int id = getId(data);
   int count = data.length()-2; 
   if (isSocket(id))
    { debug.println("WRITE: ("+String(count)+" bytes");
      sockets[id].write((uint8_t *)data.c_str()+2, count);
    }
   return voidResult();
 }

String Sockets::WRITE_TO_ALL_Command (const String &data)
// Write data to all active connections.
 { int count = data.length()-1;   
   debug.println("WRITE_TO_ALL: ("+String(count)+" bytes");
   for (int i=0; i<MAX_SOCKETS; ++i)
      if (isSocket(i))
         sockets[i].write((uint8_t *)data.c_str()+1, count);
   return voidResult();
 }
