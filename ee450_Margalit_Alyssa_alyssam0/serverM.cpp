//tcp server for 25555
//udp server for 24555
//udp client for 21555,22555,23555



// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
    
#define PORTC     21555 
#define PORTCS    22555
#define PORTEE    23555
#define PORTM     24555
#define MAXLINE 1024

#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <bits/stdc++.h>


using namespace std; 
//-----------------------UDP SETUP--------------------------
int sockfd; 
char buffer[MAXLINE]; 
char bufferC[MAXLINE];
char bufferCS[MAXLINE];
char bufferEE[MAXLINE];
const char *hello = "Hello from CLIENT M"; 
struct sockaddr_in     serverCaddr; 
struct sockaddr_in     serverCSaddr; 
struct sockaddr_in     serverEEaddr;
struct sockaddr_in     serverMaddr;

int n;
int CSMessage;
int EEMessage;
int CMessage;
socklen_t clientLen; 

void startUDPClient(){//setup udp and all addresses to communicate with
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    memset(&serverCaddr, 0, sizeof(serverCaddr)); 
    // credential server info 
    serverCaddr.sin_family = AF_INET; 
    serverCaddr.sin_port = htons(PORTC); 
    serverCaddr.sin_addr.s_addr = INADDR_ANY; 

    memset(&serverCSaddr, 0, sizeof(serverCSaddr)); 
    // cs server info
    serverCSaddr.sin_family = AF_INET; 
    serverCSaddr.sin_port = htons(PORTCS); 
    serverCSaddr.sin_addr.s_addr = INADDR_ANY;     

    memset(&serverEEaddr, 0, sizeof(serverEEaddr)); 
    // ee server info
    serverEEaddr.sin_family = AF_INET; 
    serverEEaddr.sin_port = htons(PORTEE); 
    serverEEaddr.sin_addr.s_addr = INADDR_ANY;  

    memset(&serverMaddr, 0, sizeof(serverMaddr)); 
    // self info 
    serverMaddr.sin_family = AF_INET; 
    serverMaddr.sin_port = htons(PORTM); 
    serverMaddr.sin_addr.s_addr = INADDR_ANY;

    if ( bind(sockfd, (const struct sockaddr *)&serverMaddr,  
            sizeof(serverMaddr)) < 0 ) 
    { 

        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
}
//__________________________________________________________________________
//--------------------------TCP SETUP---------------------------------
#define TCPPORT 25555

bool auth = false;
//buffer to send and receive messages with
char msg[1500];
sockaddr_in servAddr, newSockAddr;
int serverSd;
int bindStatus;
int newSd;
socklen_t newSockAddrSize;
//also keep track of the amount of data sent as well
int bytesRead, bytesWritten = 0;


void startTCP(){//based off of code from "https://github.com/bozkurthan/Simple-TCP-Server-Client-CPP-Example"
    // cout<<"starting TCP"<<endl;
    //clear space in memory and define address
    memset((char*)&servAddr, 0,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(TCPPORT);
    serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    //bind the socket to its local address
    bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    // cout << "Waiting for a client to connect..." << endl;
    cout << "The main server is up and running." << endl;
    listen(serverSd, 5);
    newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    
}
//________________________________________________________________________

//--------------------Encryption-----------------------
char substr;
char unencrypted[102];
const char* encrypted;

void encrypt(char* message){
   // cout<<"original message: "<<message<<endl;

    for(int i = 0; i<(unsigned)strlen(message);i++){
        substr = message[i];
        if(isalpha(substr)){
            int asciiVal = int(substr);
            if(asciiVal>=97){
                //is lowercase
                message[i]-=97;
                message[i]+=4;
                message[i]=message[i]%26;
                message[i]+=97;
            }else{
                //is uppercase
                message[i]-=65;
                message[i]+=4;
                message[i]=message[i]%26;
                message[i]+=65;
            }
        }else if(isdigit(substr)){
            //is digit
            message[i]-=48;
            message[i]+=4;
            message[i]=message[i]%10;
            message[i]+=48;
        }
     }
     encrypted = message;
    //  cout<<"encrypted value: "<< encrypted<<endl;

}

//_______________________
//------------------------server comms-----------------------
const char *responseC = "hello";
void CComs(){//send and receive with credential server
    sendto(sockfd, (const char *)responseC, strlen(responseC), 
        MSG_CONFIRM, (const struct sockaddr *) &serverCaddr,  
            sizeof(serverEEaddr)); 
    cout<<"The main server sent an authentication request to serverC."<<endl; 
    CMessage = recvfrom(sockfd, (char *)bufferC, MAXLINE,  
            MSG_WAITALL, (struct sockaddr *) &serverCaddr, 
            &clientLen); 
    cout<<"The main server received the result of the authentication request from ServerC using UDP over port 21555."<<endl;

    bufferC[CMessage] = '\0'; 
    // printf("ServerC : %s\n", bufferC);
}
const char *responseCS = "hello";
void CSComs(){//send and receive with server CS
    sendto(sockfd, (const char *)responseCS, strlen(responseCS), 
        MSG_CONFIRM, (const struct sockaddr *) &serverCSaddr,  
            sizeof(serverEEaddr));  
    
    cout<<"The main server sent a request to server CS."<<endl;
    CSMessage = recvfrom(sockfd, (char *)bufferCS, MAXLINE,  
            MSG_WAITALL, (struct sockaddr *) &serverCSaddr, 
            &clientLen);
    cout<<"The main server received the response from server CS using UDP over port 22555."<<endl; 

    bufferCS[CSMessage] = '\0'; 
    // printf("ServerCS : %s\n", bufferCS);
}
const char *responseEE = "hello";
void EEComs(){//send and receive with serverEE
    sendto(sockfd, (const char *)responseEE, strlen(responseEE), 
        MSG_CONFIRM, (const struct sockaddr *) &serverEEaddr,  
            sizeof(serverEEaddr));  
    cout<<"The main server sent a request to server EE."<<endl;
    EEMessage = recvfrom(sockfd, (char *)bufferEE, MAXLINE,  
            MSG_WAITALL, (struct sockaddr *) &serverEEaddr, 
            &clientLen); 
    cout<<"The main server received the response from server EE using UDP over port 23555."<<endl; 

    bufferEE[EEMessage] = '\0'; 
    // printf("ServerEE : %s\n", bufferEE);
}

//__________________________________________
//-------------split messages for console output----------
string username;
void splitUsername(){
    vector<string> split1;
    split1.clear();
    string temp = msg;
    stringstream x(temp);
    while(x.good()){
        string y;
        getline(x,y,',');
        split1.push_back(y);
    }
    username = split1.at(0);
}
string courseCode;
string category;
void splitRequest(){

    vector<string> split2;
    split2.clear();
    string temp = msg;
    stringstream x(temp);
    while(x.good()){
        string y;
        getline(x,y,',');
        split2.push_back(y);
    }
    courseCode = split2.at(0);
    category = split2.at(1);
    // cout<<"courseCode: "<<courseCode<<endl;
    // cout<<"category: "<<category<<endl;
}

//_______________





string state = "getcreds";
// state machine for communicating with everybody 
int main() { 
    startUDPClient();
    startTCP();
    while(1){
        // cout<<"\n\n starting state "<<state<<endl;
        if(state=="getcreds"){//recieve credentials from client and encrypt them
            //recieving authentication request from client
            // cout<<"waiting for authentication request from client";
            memset(&msg, 0, sizeof(msg));//clear the buffer
            bytesRead = recv(newSd, (char*)&msg, sizeof(msg), 0);//recieve unencrypted creds from client
            if(bytesRead){
                splitUsername();
                cout << "The main server received the authentication for: " << username << " using TCP over port 25555."<< endl;
                
                encrypt(msg);//encrypt message
                responseC = msg;//put encrypted message into send buffer
                state = "verifyCreds";
            }
        }else if(state=="verifyCreds"){//send encrypted creds to serverC and recieve a response
            CComs();
            state = "relaytoclient";
        }else if(state == "relaytoclient"){
            //relay response from serverC to client
            bytesWritten = send(newSd, (char*)&bufferC, strlen(bufferC), 0);
            cout<<"The main server sent the authentication result to the client."<<endl;
            string checker = bufferC;
            if(checker!="correct"){//if credentials were wrong, go back to the getcreds state
                state = "getcreds";
            }else{
                state = "getCourseQuery";
            }
            
        }else if(state=="getCourseQuery"){//get course query from client
            // cout<<"waiting for course query from client"<<endl;
            memset(&msg, 0, sizeof(msg));//clear the buffer
            bytesRead = recv(newSd, (char*)&msg, sizeof(msg), 0);
            if(bytesRead){
            splitRequest();
                cout<<"The main server received from "<<username<<" to query course "<<courseCode<<" about category "<<category<<" using TCP over port 25555."<<endl;
                // cout<<"recieved course query of "<<msg<<" from client"<<endl;
                state = "relayQuery";
            }

        }else if(state=="relayQuery"){    
            //check if EE or CS
            string temp = msg;
            if(temp.substr(0,2)=="EE"){
                responseEE = msg;
                EEComs();
            }else if(temp.substr(0,2)=="CS"){
                responseCS = msg;
                CSComs();
            }else{
                cout<<"not valid, testing "<<temp.substr(0,2)<<endl;
            } //then send to EE
            //else if CS, send to CS  
            state = "deliverResults";
        }else if(state=="deliverResults"){
            //send results to client
            string temp = msg;
            if(temp.substr(0,2)=="EE"){
                bytesWritten = send(newSd, (char*)&bufferEE, strlen(bufferEE), 0);
            }else if(temp.substr(0,2)=="CS"){
                bytesWritten = send(newSd, (char*)&bufferCS, strlen(bufferCS), 0);
            }
            cout<<"The main server sent the query information to the client."<<endl;
            state = "getCourseQuery";
            
        }else if(state=="end"){
            cout<<"end"<<endl;
            break;
        }

    }




    close(sockfd); 
    return 0; 
}