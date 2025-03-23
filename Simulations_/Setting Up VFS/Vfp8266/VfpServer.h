#pragma once

#include <core.h>
#include <IPAddress.h>

#include "Bridge.h"

#define VFPSERVER 1

namespace Vfp8266
 { class Control 
    { friend class Server;
      public:
         Control (const char *_id);
         virtual void reload() { }
         virtual StreamList &parseArgs(StreamList &argList) { return argList; }
         void attachReloadHandler(void (*isr)()) { reloadHandler=isr;  }
         void attachEventHandler(void (*isr)()) { eventHandler=isr; }
         
      protected:
    	  	const char *id;
    	   void (*reloadHandler)();
         void (*eventHandler)();
         Control *next; 	  	      
    };

   class Server 
    { public:
         Server();
         void begin (int port=80);
         void setTitle (const String &title); 
         bool operator()(const String &object) { return requestObject.equalsIgnoreCase(object); }
         PrintList &call(const char *object, const __FlashStringHelper *method);
         PrintList &call(const String &object, const String &method);
         void set(const char *object, const __FlashStringHelper *key, const String &value);
         PrintList &set(const char *object, const __FlashStringHelper *key);
         PrintList &set(const String &object, const String &key);
         void create(const char *object, const __FlashStringHelper *key);
         void record(const char *object, bool flag=true);
         void erase(const char *object);
         void authorize(bool flag);
         bool isAuthorized() { return authorized; }
         bool isRequestObject(const char *object) { return requestObject.equalsIgnoreCase(object); }      
         
         void awaitRequest(uint32_t delay) { waitForRequests(delay); }
         void waitForRequests(uint32_t delay);          
         void waitForTimeServer();
 	      PrintList &debug() { return debugList.begin(true, &Console); }      
         
         void poll();
   
         static void addControl(Control *instance);
         static void attachReloadHandler(void (*isr)()) { reloadHandler = isr; }
         static void attachLoginHandler(void (*isr)()) { loginHandler=isr;  }
         static void attachLogoutHandler(void (*isr)()) { logoutHandler=isr;  }
         static void attachRequestHandler(void (*isr)()) { requestHandler = isr; }
   
      private:
         void callReloadHandlers();
         void callEventHandlers();
         
         struct ControlInfo
          { Control *control;
          	ControlInfo *next;
          };
         
      private:
         static void (*reloadHandler)();      
    	   static void (*loginHandler)();
    	   static void (*logoutHandler)();
         static void (*requestHandler)();   
         static Control *controls;
         IPAddress ipaddress;
         bool authorized;
         Process process;
         String requestObject;
         StreamList requestList;
         PrintList statusList;      
 	      PrintList debugList;         
    };
      
 };

typedef Vfp8266::Control Control;
extern  Vfp8266::Server VFP;

 

