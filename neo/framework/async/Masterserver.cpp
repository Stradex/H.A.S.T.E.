#include "Masterserver.h"

idWebMasterServer htmlMasterServer;

/*
============
idWebMasterServer::idWebMasterServer
============
*/
idWebMasterServer::idWebMasterServer( void ) {
	strcpy(this->authPass, "RBzLKom9_3ul");
	strcpy(this->masterUrl, "http://www.stradexengine.com/masterserver/"); //default html master server
	//strcpy(this->addServerUrl, "http://www.stradexengine.com/masterserver/addServer.php");
	//strcpy(this->masterUrl, "http://localhost/test.php");
	
}

/*
============
idWebMasterServer::idWebMasterServer
============
*/
idWebMasterServer::~idWebMasterServer( void ) {
}

/*
============
idWebMasterServer::uncodeData
============
*/
int idWebMasterServer::uncodeData(const char* dataStrign, serverDataL* &svDataList) {

	svDataList = static_cast<serverDataL*>(malloc((1)*sizeof(serverDataL)));

	  int i=0, z=0, s=0;
	  int chunkLen = strlen(dataStrign)+1;
	  int startPos = int(strstr(dataStrign, "::IPListStart::") + strlen("::IPListStart::") - dataStrign);  
	  int endPos = int(strstr(dataStrign, "::IPListEnd::") - dataStrign);  
	  if ((startPos > chunkLen) || (endPos > chunkLen)) {
		  printf("error, invalid string\n");
		  return 0;
	  }
	  char tmp[64];
	  for (i=startPos; i < endPos; i++){
		  if (dataStrign[i] == '\n' || dataStrign[i] == ' ' || dataStrign[i] == '\t') {
				continue;
		  }
		  if (dataStrign[i] == ';') {
			  tmp[z] = '\0';
			  strcpy(svDataList[s++].address, tmp);

			  svDataList = static_cast<serverDataL*>(realloc(svDataList, (s + 1)*sizeof(serverDataL)));
			  z=0;
		  } else if (z<64) {
			  tmp[z++] = dataStrign[i];
		  }
	  }

	 return s;
}
		
/*
============
idWebMasterServer::refreshServerList
============
*/
void idWebMasterServer::refreshServerList( void ) {

   //reset serverList, pointer null
   publicServers = 0;
   this->serversCount = 0;

  CURL *curl_handle;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = static_cast<char*>(malloc(1));  /* will be grown as needed by the realloc above */ 
  chunk.size = 0;    /* no data at this point */ 
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* specify URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, this->masterUrl);
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */ 
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
   //added by Stradex, this should not take more than 60 seconds to be done
  curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, MASTERSERVER_TIMEOUT);

  /* get it! */ 
  res = curl_easy_perform(curl_handle);
 
  /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */ 
	  this->serversCount = this->uncodeData(chunk.memory, publicServers);
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  free(chunk.memory);
 
  /* we're done with libcurl, so clean it up */ 
  curl_global_cleanup();

  return;
}

/*
============
idWebMasterServer::getServerList
============
*/
int idWebMasterServer::getServerCount( void ) {
	return this->serversCount;
}

bool idWebMasterServer::addServer(int port) {

  CURL *curl_handle;
  CURLcode res;
 
  struct MemoryStruct chunk;
 bool returnStatus=false;
 char portString[8];
 char urlToSendData[600];

 sprintf(urlToSendData, "%saddServer.php?pass=%s&port=%d", this->masterUrl, this->authPass, port); 
 //strcat(urlToSendData, this->addServerUrl);
 //strcat(urlToSendData, portString);

 chunk.memory = static_cast<char*>(malloc(1));  /* will be grown as needed by the realloc above */ 
  chunk.size = 0;    /* no data at this point */ 
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
	printf("Info a: %s\n", urlToSendData);
  /* specify URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, urlToSendData);
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */ 
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
  //added by Stradex, this should not take more than 60 seconds to be done
  curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, MASTERSERVER_TIMEOUT);

  /* get it! */ 
  res = curl_easy_perform(curl_handle);
 
  /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
	 returnStatus = true;
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  free(chunk.memory);
 
  /* we're done with libcurl, so clean it up */ 
  curl_global_cleanup();

  return  returnStatus;
}