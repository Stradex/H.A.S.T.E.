#ifndef __WEB_MASTERSERVER_H__
#define __WEB_MASTERSERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <curl/curl.h>

/*
DB Info:

authPass: RBzLKom9_3ul
example: http://www.stradexengine.com/masterserver/addServer.php?pass=RBzLKom9_3ul&port=10670
*/

#define MASTERSERVER_TIMEOUT		60

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = static_cast<char*>(realloc(mem->memory, mem->size + realsize + 1));
  if(ptr == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

typedef struct serverDataL {
	char address[64];
};
class idWebMasterServer {
	public:
		idWebMasterServer( void );
		~idWebMasterServer( void );
		int uncodeData(const char* dataStrign, serverDataL* &svDataList);

		void refreshServerList( void );
		serverDataL* publicServers;

		int	  getServerCount( void );
		bool  addServer( int port );
		char masterUrl[512];
	private:

		int	 serversCount;
		char authPass[32];
		//char addServerUrl[512];

};

extern idWebMasterServer htmlMasterServer; //added by Stradex

#endif /* !__WEB_MASTERSERVER_H__ */
