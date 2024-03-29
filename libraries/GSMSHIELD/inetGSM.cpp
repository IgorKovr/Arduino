#include "inetGSM.h"
#define _GSM_CONNECTION_TOUT_ 5
#define _TCP_CONNECTION_TOUT_ 20
#define _GSM_DATA_TOUT_ 10
#define DEBUG_ON 1

int InetGSM::httpGET(const char* server, int port, const char* path, const char*header, char* result, int resultlength)
{
  boolean connected=false;
  int n_of_at=0;
  int length_write;
  char end_c[2];
  end_c[0]=0x1a;
  end_c[1]='\0';

  /*
  Status = ATTACHED.
  if(gsm.getStatus()!=GSM::ATTACHED)
    return 0;
  */
  while(n_of_at<3){
	  if(!connectTCP(server, port)){
	  	#ifdef DEBUG_ON
			Serial.println("DB:NOT CONN");
		#endif	
	    	n_of_at++;
	  }
	  else{
		connected=true;
		n_of_at=3;
	}
  }

    if(!connected) return 0;

  gsm.SimpleWrite("GET ");
  gsm.SimpleWrite(path);
  gsm.SimpleWrite(" HTTP/1.1\nHost: ");
  gsm.SimpleWrite(server);
  gsm.SimpleWrite("\n");
  gsm.SimpleWrite("User-Agent: Arduino");
//  gsm.SimpleWrite("\n\n");
    gsm.SimpleWrite("\n");
    gsm.SimpleWrite(header);
    gsm.SimpleWrite("\n\n");
  gsm.SimpleWrite(end_c);

  switch(gsm.WaitResp(10000, 100, "SEND")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }

  	#ifdef DEBUG_ON
		Serial.println("DB:SENT");
        Serial.println(result);
	#endif	
  int res= gsm.read(result, resultlength);

    if (!res)
//    Serial.println("can't read data");

  //gsm.disconnectTCP();
//        disconnectTCP();

  //int res=1;
  return res;
}

int InetGSM::httpPUT(const char* server, int port, const char* path, char* parameters, const char*header, char* result, int resultlength)
{
    boolean connected=false;
    int n_of_at=0;
    char itoaBuffer[8];
    int num_char;
    char end_c[2];
    end_c[0]=0x1a;
    end_c[1]='\0';

    while(n_of_at<3) {
        if(!connectTCP(server, port)) {
#ifdef DEBUG_ON
            Serial.println("DB:NOT CONN");
#endif
            n_of_at++;
        } else {
            connected=true;
            n_of_at=3;
        }
    }

    if(!connected) return 0;

    gsm.SimpleWrite("PUT ");
    gsm.SimpleWrite(path);
    gsm.SimpleWrite(" HTTP/1.1\r\nHost: ");
    gsm.SimpleWrite(server);
    gsm.SimpleWrite("\r\n");
    gsm.SimpleWrite("User-Agent: Arduino\r\n");
    gsm.SimpleWrite("Content-Type: application/x-www-form-urlencoded\r\n");
    gsm.SimpleWrite("X-ApiKey: oeyxzwo2JWX3mnKFA9JuzCZ5mJSF8vB4flLfAW0hUYEXsh3O\r\n");
    gsm.SimpleWrite("Content-Length: ");
    itoa(strlen(parameters),itoaBuffer,10);
    gsm.SimpleWrite(itoaBuffer);
    gsm.SimpleWrite("\r\n\r\n");
    gsm.SimpleWrite(parameters);
    gsm.SimpleWrite("\r\n\r\n");
    gsm.SimpleWrite(end_c);

    switch(gsm.WaitResp(10000, 10, "SEND OK")) {
        case RX_TMOUT_ERR:
            return 0;
            break;
        case RX_FINISHED_STR_NOT_RECV:
            return 0;
            break;
    }

    delay(50);
#ifdef DEBUG_ON
    Serial.println("DB:SENT");
#endif

    int res= gsm.read(result, resultlength);
    //gsm.disconnectTCP();
    return res;
}

int InetGSM::httpPOSTWithFile(const char* server, int port, const char* path, const char* parameters, File myFile,  char* result, int resultlength){
    boolean connected=false;
    int n_of_at=0;
    char itoaBuffer[8];
    int num_char;
    char end_c[2];
    end_c[0]=0x1a;
    end_c[1]='\0';

    while(n_of_at<3) {
        if(!connectTCP(server, port)) {
#ifdef DEBUG_ON
            Serial.println("DB:NOT CONN");
#endif
            n_of_at++;
        } else {
            connected=true;
            n_of_at=3;
        }
    }

    if(!connected) return 0;

    gsm.SimpleWrite("POST /upload?upload_progress_id=12344");
    gsm.SimpleWrite(path);
    gsm.SimpleWrite(" HTTP/1.1\r\nHost: ");
    gsm.SimpleWrite(server);
    gsm.SimpleWrite("\r\n");
    gsm.SimpleWrite("User-Agent: Arduino\r\n");
    gsm.SimpleWrite("Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryePkpFF7tjBAqx29L\r\n");
    gsm.SimpleWrite("X-ApiKey: oeyxzwo2JWX3mnKFA9JuzCZ5mJSF8vB4flLfAW0hUYEXsh3O\r\n");
    gsm.SimpleWrite("Content-Length: ");
    itoa(strlen(parameters),itoaBuffer,10);

    ///
    //gsm.SimpleWrite(itoaBuffer);

        gsm.SimpleWrite(55000);

    gsm.SimpleWrite("\r\n\r\n");
    gsm.SimpleWrite(parameters);
    gsm.SimpleWrite("\r\n\r\n");
    gsm.SimpleWrite(end_c);

    if (myFile) {
        gsm.SimpleWrite("------WebKitFormBoundaryePkpFF7tjBAqx29L \n Content-Disposition: form-data; name=""MAX_FILE_SIZE"" \n\n100000\n");
        gsm.SimpleWrite("------WebKitFormBoundaryePkpFF7tjBAqx29L \n Content-Disposition: form-data; name=""uploadFile""; filename=""1.jpg""\n");
        gsm.SimpleWrite("Content-Type: application/x-object\n\n");

        Serial.println("Uploading 1.jpg:");
        // read from the file until there's nothing else in it:
        while (myFile.available()) {
            gsm.SimpleWrite(myFile.read());
        }
        // close the file:
        myFile.close();
        gsm.SimpleWrite("\n\n------WebKitFormBoundaryePkpFF7tjBAqx29L--");

        Serial.println("Uploaded 1.jpg:");
    }
    else {
        // if the file didn't open, print an error:
        Serial.println("error opening File");
    }

    switch(gsm.WaitResp(10000, 10, "SEND OK")) {
        case RX_TMOUT_ERR:
            return 0;
            break;
        case RX_FINISHED_STR_NOT_RECV:
            return 0;
            break;
    }

    delay(50);
#ifdef DEBUG_ON
    Serial.println("DB:SENT");
#endif

    int res= gsm.read(result, resultlength);
    //gsm.disconnectTCP();
    return res;
}

int InetGSM::httpPOST(const char* server, int port, const char* path, const char* parameters, char* result, int resultlength)
{
    boolean connected=false;
    int n_of_at=0;
    char itoaBuffer[8];
    int num_char;
    char end_c[2];
    end_c[0]=0x1a;
    end_c[1]='\0';

    while(n_of_at<3) {
        if(!connectTCP(server, port)) {
#ifdef DEBUG_ON
            Serial.println("DB:NOT CONN");
#endif
            n_of_at++;
        } else {
            connected=true;
            n_of_at=3;
        }
    }

    if(!connected) return 0;

    gsm.SimpleWrite("POST ");
    gsm.SimpleWrite(path);
    gsm.SimpleWrite(" HTTP/1.1\r\nHost: ");
    gsm.SimpleWrite(server);
    gsm.SimpleWrite("\r\n");
    gsm.SimpleWrite("User-Agent: Arduino\r\n");
    gsm.SimpleWrite("Content-Type: application/x-www-form-urlencoded\r\n");
    gsm.SimpleWrite("X-ApiKey: oeyxzwo2JWX3mnKFA9JuzCZ5mJSF8vB4flLfAW0hUYEXsh3O\r\n");
    gsm.SimpleWrite("Content-Length: ");
    itoa(strlen(parameters),itoaBuffer,10);
    gsm.SimpleWrite(itoaBuffer);
    gsm.SimpleWrite("\r\n\r\n");
    gsm.SimpleWrite(parameters);
    gsm.SimpleWrite("\r\n\r\n");
    gsm.SimpleWrite(end_c);

    switch(gsm.WaitResp(10000, 10, "SEND OK")) {
        case RX_TMOUT_ERR:
            return 0;
            break;
        case RX_FINISHED_STR_NOT_RECV:
            return 0;
            break;
    }

    delay(50);
#ifdef DEBUG_ON
    Serial.println("DB:SENT");
#endif

    int res= gsm.read(result, resultlength);
    //gsm.disconnectTCP();
    return res;
}
/*
int InetGSM::tweet(const char* token, const char* msg)
{
  //    gsm.httpPOST("arduino-tweet.appspot.com",80,"/update", "token=15514242-eWYAlMwjRQfrhnZxQiOfDXUpaYwjbSvMl1Nm5Qyg&status=Spam", buffer, 200);
  char shortbuf[200];
  strcpy(shortbuf,"token=");
  strcat(shortbuf,token);
  strcat(shortbuf,"&status=");
  strcat(shortbuf, msg);
  httpPOST("arduino-tweet.appspot.com",80,"/update",shortbuf, shortbuf, BUFFERSIZE);
}*/

int InetGSM::openmail(char* server, char* loginbase64, char* passbase64, char* from, char* to, char* subj)
{
    boolean connected=false;
    int n_of_at=0;
    char end_c[2];
    end_c[0]=0x1a;
    end_c[1]='\0';

/// !!! SMTP PORT

    while(n_of_at<3){
        if(!connectTCP(server, 587)){
#ifdef DEBUG_ON
			Serial.println("DB:NOT CONN");
#endif
	    	n_of_at++;
        }
        else{
            connected=true;
            n_of_at=3;
        }
    }

    if(!connected) return 0;
	delay(100);
    gsm.SimpleWrite("HELO ");  gsm.SimpleWrite(server);  gsm.SimpleWrite("\n");
	gsm.SimpleWrite(end_c);
	gsm.WaitResp(5000, 100, "OK");
	if(!gsm.IsStringReceived("SEND OK"))
		return 0;
    delay(500);
	gsm.WaitResp(5000, 100);

	delay(100);
	gsm.SimpleWriteln("AT+CIPSEND");
	switch(gsm.WaitResp(5000, 200, ">")){
		case RX_TMOUT_ERR:
			return 0;
			break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
			break;
	}
    gsm.SimpleWrite("AUTH LOGIN\n");
	gsm.SimpleWrite(end_c);
	gsm.WaitResp(5000, 100, "OK");
	if(!gsm.IsStringReceived("OK"))
		return 0;
    delay(500);
	gsm.WaitResp(5000, 100);

	delay(100);
	gsm.SimpleWriteln("AT+CIPSEND");
	switch(gsm.WaitResp(5000, 200, ">")){
		case RX_TMOUT_ERR:
			return 0;
			break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
			break;
	}
    gsm.SimpleWrite(loginbase64);gsm.SimpleWrite("\n");
	gsm.SimpleWrite(end_c);
	gsm.WaitResp(5000, 100, "OK");
	if(!gsm.IsStringReceived("OK"))
		return 0;
    delay(500);
	gsm.WaitResp(5000, 100);

	delay(100);
	gsm.SimpleWriteln("AT+CIPSEND");
	switch(gsm.WaitResp(5000, 200, ">")){
		case RX_TMOUT_ERR:
			return 0;
			break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
			break;
	}
    gsm.SimpleWrite(passbase64);gsm.SimpleWrite("\n");
	gsm.SimpleWrite(end_c);
	gsm.WaitResp(5000, 100, "OK");
	if(!gsm.IsStringReceived("OK"))
		return 0;
    delay(500);
	gsm.WaitResp(5000, 100);


	delay(100);
	gsm.SimpleWriteln("AT+CIPSEND");
	switch(gsm.WaitResp(5000, 200, ">")){
		case RX_TMOUT_ERR:
			return 0;
			break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
			break;
	}
    gsm.SimpleWrite("MAIL From: <");gsm.SimpleWrite(from);gsm.SimpleWrite(">\n");
	gsm.SimpleWrite(end_c);
	gsm.WaitResp(5000, 100, "OK");
	if(!gsm.IsStringReceived("OK"))
		return 0;
    delay(500);
	gsm.WaitResp(5000, 100, "");

	delay(100);
	gsm.SimpleWriteln("AT+CIPSEND");
	switch(gsm.WaitResp(5000, 200, ">")){
		case RX_TMOUT_ERR:
			return 0;
			break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
			break;
	}
    gsm.SimpleWrite("RCPT TO: <");gsm.SimpleWrite(to);gsm.SimpleWrite(">\n");
	gsm.SimpleWrite(end_c);
	gsm.WaitResp(5000, 100, "OK");
	if(!gsm.IsStringReceived("OK"))
		return 0;
    delay(500);
	gsm.WaitResp(5000, 100, "");

    delay(100);
	gsm.SimpleWriteln("AT+CIPSEND");
	switch(gsm.WaitResp(5000, 200, ">")){
		case RX_TMOUT_ERR:
			return 0;
			break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
			break;
	}
    gsm.SimpleWrite("Data\n");
	gsm.SimpleWrite(end_c);
	gsm.WaitResp(5000, 100, "OK");
	if(!gsm.IsStringReceived("OK"))
		return 0;
    delay(500);
	gsm.WaitResp(5000, 100, "");

	delay(100);
	gsm.SimpleWriteln("AT+CIPSEND");
	switch(gsm.WaitResp(5000, 200, ">")){
		case RX_TMOUT_ERR:
			return 0;
			break;
		case RX_FINISHED_STR_NOT_RECV:
			return 0;
			break;
	}
    gsm.SimpleWrite("Subject: ");gsm.SimpleWrite(subj);gsm.SimpleWrite("\n\n");

    return 1;
}


int InetGSM::closemail()
{
	char end_c[2];
	end_c[0]=0x1a;
	end_c[1]='\0';
    
	gsm.SimpleWrite("\n.\n");
	gsm.SimpleWrite(end_c);
	disconnectTCP();
	return 1;
}

 

int InetGSM::attachGPRS(char* domain, char* dom1, char* dom2)
{
   int i=0;
   delay(5000);
   
  //gsm._tf.setTimeout(_GSM_DATA_TOUT_);	//Timeout for expecting modem responses.
  gsm.WaitResp(50, 50);
  gsm.SimpleWriteln("AT+CIFSR");
  Serial.println("TEST");
  if(gsm.WaitResp(5000, 50, "ERROR")!=RX_FINISHED_STR_RECV){
  	#ifdef DEBUG_ON
		Serial.println("DB:ALREADY HAVE AN IP");
	#endif
	  gsm.SimpleWriteln("AT+CIPCLOSE");
	gsm.WaitResp(5000, 50, "ERROR");
	delay(2000);
	gsm.SimpleWriteln("AT+CIPSERVER=0");
	gsm.WaitResp(5000, 50, "ERROR");
	return 1;
  }
  else{

	#ifdef DEBUG_ON
		Serial.println("DB:STARTING NEW CONNECTION");
	#endif
  
  gsm.SimpleWriteln("AT+CIPSHUT");
  
  switch(gsm.WaitResp(500, 50, "SHUT OK")){

	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
	#ifdef DEBUG_ON
		Serial.println("DB:SHUTTED OK");
	#endif
	 delay(1000);
	 
  gsm.SimpleWrite("AT+CSTT=\"");
  gsm.SimpleWrite(domain);
  gsm.SimpleWrite("\",\"");
  gsm.SimpleWrite(dom1);
  gsm.SimpleWrite("\",\"");
  gsm.SimpleWrite(dom2);
  gsm.SimpleWrite("\"\r");  

  
  switch(gsm.WaitResp(500, 50, "OK")){

	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
	#ifdef DEBUG_ON
		Serial.println("DB:APN OK");
	#endif
	 delay(5000);
	  
	gsm.SimpleWriteln("AT+CIICR");  

  switch(gsm.WaitResp(10000, 50, "OK")){
	case RX_TMOUT_ERR: 
		return 0; 
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
  	#ifdef DEBUG_ON
		Serial.println("DB:CONNECTION OK");
	#endif

  delay(1000);


 gsm.SimpleWriteln("AT+CIFSR");
 if(gsm.WaitResp(5000, 50, "ERROR")!=RX_FINISHED_STR_RECV){
	#ifdef DEBUG_ON
		Serial.println("DB:ASSIGNED AN IP");
	#endif
	gsm.setStatus(gsm.ATTACHED);
	return 1;
}
	#ifdef DEBUG_ON
		Serial.println("DB:NO IP AFTER CONNECTION");
	#endif
 return 0;
 }
}

int InetGSM::dettachGPRS()
{
  if (gsm.getStatus()==gsm.IDLE) return 0;
   
  //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);

  //_cell.flush();

  //GPRS dettachment.
  gsm.SimpleWriteln("AT+CGATT=0");
  if(gsm.WaitResp(5000, 50, "OK")!=RX_FINISHED_STR_NOT_RECV)
  {
    gsm.setStatus(gsm.ERROR);
    return 0;
  }
  delay(500);
  
  // Commented in initial trial code!!
  //Stop IP stack.
  //_cell << "AT+WIPCFG=0" <<  _DEC(cr) << endl;
  //	if(!gsm._tf.find("OK")) return 0;
  //Close GPRS bearer.
  //_cell << "AT+WIPBR=0,6" <<  _DEC(cr) << endl;

  gsm.setStatus(gsm.READY);
  return 1;
}

int InetGSM::connectTCP(const char* server, int port)
{
  //gsm._tf.setTimeout(_TCP_CONNECTION_TOUT_);

  //Status = ATTACHED.
  //if (getStatus()!=ATTACHED)
    //return 0;

  //_cell.flush();
  
  //Visit the remote TCP server.
   gsm.SimpleWrite("AT+CIPSTART=\"TCP\",\"");
   gsm.SimpleWrite(server);
   gsm.SimpleWrite("\",");
   gsm.SimpleWriteln(port);
  
  switch(gsm.WaitResp(1000, 200, "OK")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }
  #ifdef DEBUG_ON
	Serial.println("DB:RECVD CMD");
  #endif	

  switch(gsm.WaitResp(15000, 200, "OK")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }

  #ifdef DEBUG_ON
	Serial.println("DB:OK TCP");
  #endif

  delay(3000);
  gsm.SimpleWriteln("AT+CIPSEND");
  switch(gsm.WaitResp(5000, 200, ">")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }

  #ifdef DEBUG_ON
	Serial.println("DB:>");
  #endif
  delay(4000);
  return 1;
}

int InetGSM::disconnectTCP()
{
  //Status = TCPCONNECTEDCLIENT or TCPCONNECTEDSERVER.
  /*
  if ((getStatus()!=TCPCONNECTEDCLIENT)&&(getStatus()!=TCPCONNECTEDSERVER))
     return 0;
  */
  //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);


  //_cell.flush();

  //Switch to AT mode.
  //_cell << "+++" << endl;
  
  //delay(200);
  
  //Close TCP client and deact.
  gsm.SimpleWriteln("AT+CIPCLOSE");

  //If remote server close connection AT+QICLOSE generate ERROR
  /*if(gsm._tf.find("OK"))
  {
    if(getStatus()==TCPCONNECTEDCLIENT)
      gsm.setStatus(ATTACHED);
    else
      gsm.setStatus(TCPSERVERWAIT);
    return 1;
  }
  gsm.setStatus(ERROR);
  
  return 0;    */
  if(gsm.getStatus()==gsm.TCPCONNECTEDCLIENT)
      	gsm.setStatus(gsm.ATTACHED);
   elsehttp://www.facebook.com/photo.php?fbid=2486533357849&set=a.2316317142550.2138694.1088100793&type=1&ref=nf
        gsm.setStatus(gsm.TCPSERVERWAIT);   
    return 1;
}

int InetGSM::connectTCPServer(int port)
{
//
//  if (getStatus()!=ATTACHED)
//     return 0;
//
//gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);

//_cell.flush();

  // Set port

    Serial.println("TCP CONNECTING");
  
  gsm.SimpleWrite("AT+CIPSERVER=1,");
  gsm.SimpleWriteln(port);

//  switch(gsm.WaitResp(5000, 50, "OK")){
//	case RX_TMOUT_ERR: 
//		return 0;
//	break;
//	case RX_FINISHED_STR_NOT_RECV: 
//		return 0; 
//	break;
//  }
//
//  switch(gsm.WaitResp(5000, 50, "SERVER")){ //Try SERVER OK
//	case RX_TMOUT_ERR: 
//		return 0;
//	break;
//	case RX_FINISHED_STR_NOT_RECV: 
//		return 0; 
//	break;
//  }
        Serial.println("TCP CONNECTED");

  delay(200);

  return 2;

}

boolean InetGSM::connectedClient()
{
  /*
  if (getStatus()!=TCPSERVERWAIT)
     return 0;
  */
  
   gsm.SimpleWriteln("AT+CIPSTATUS");
  // Alternative: AT+QISTAT, although it may be necessary to call an AT 
  // command every second,which is not wise
  /*
  switch(gsm.WaitResp(1000, 200, "OK")){
	case RX_TMOUT_ERR: 
		return 0;
	break;
	case RX_FINISHED_STR_NOT_RECV: 
		return 0; 
	break;
  }*/
  //gsm._tf.setTimeout(1);
  if(gsm.WaitResp(5000, 50, "CONNECT OK")!=RX_FINISHED_STR_RECV)
  {
    gsm.setStatus(gsm.TCPCONNECTEDSERVER);
    return true;
  }
  else
    return false;
 }

