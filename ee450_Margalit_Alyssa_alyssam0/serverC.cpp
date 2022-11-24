//udp server for 23555
//udp client for 24555

// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>


//----------------------UDP SETUP-------------------------  
#define PORTC     21555 
#define PORTM      24555
#define MAXLINE 1024

using namespace::std;

struct sockaddr_in     serverMaddr;
int sockfd; 
char buffer[MAXLINE];
const char *response = ""; 
const char *hello = "Hello from C2 SERVER"; 
struct sockaddr_in servaddr, cliaddr; 

void startUDP(){
    // printf("hello");
    // Create socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    //make room for incoming and outgoing messages
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 

    // Define address of serverEE
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORTC);

    //make room for messages about serverM
    memset(&serverMaddr, 0, sizeof(serverMaddr)); 
    // Define address of serverM
    serverMaddr.sin_family = AF_INET; 
    serverMaddr.sin_port = htons(PORTM); 
    serverMaddr.sin_addr.s_addr = INADDR_ANY;


    // Bind socket file descriptor to serverEE port 23555
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 

        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
}
//______________________________________________________
//---------------read text file------------------------
vector<string> credentials1;
vector<string> username;
vector<string> password;
int size;
void getCredentials(){
    ifstream in("cred.txt");
    string temp;
    while(getline(in,temp)){
        if(temp.size()>0){
            credentials1.push_back(temp);
        }
    }
    // for(size_t i = 0; i<credentials1.size();i++){
    //     cout<<credentials1.at(i)<<endl;
    // }
}
void splitCredentials(){//parse credentials into user and pass vectors
    for(size_t i = 0; i<credentials1.size();i++){
        vector<string> split;
        string temp = credentials1.at(i);
        // cout<<"temp: "<<temp<<endl;
        stringstream x(temp);
        while(x.good()){
            string y;
            getline(x,y,',');
            split.push_back(y);
        }
        // cout<<"username"<<i<<": "<<split.at(0)<<endl;
        // cout<<"password"<<i<<": "<<split.at(1)<<endl;
        username.push_back(split.at(0));
        password.push_back(split.at(1));
    }
}
string total;
void checkCredentials(){//check if credentials are correct or not
    //////checking creds/////
    total = buffer;
    int index = total.find(',');
    //  cout<<"found comma at: "<<index<<endl;
    string usernameInput = total.substr(0,index);
    string passwordInput = total.substr(index+1,total.size()-1);
    // cout<<"username: "<<usernameInput<<endl;
    // cout<<"password: "<<passwordInput<<endl;

    for(size_t i = 0; i<username.size();i++){
        if(username.at(i)==usernameInput){
            if(password.at(i).substr(0,password.at(i).size()-1)==passwordInput){//if username found and password matches
            response = "correct";
            break;
            }else{
                response = "wrong_pass";//username found, pass doesn't match
                break;
            }
        }else if(i==username.size()-1){//end of array, username not found
            response = "wrong_username";
        }
    }
    // cout<<"response is: "<<response<<endl;
}
//___________________________________________________
int n; 
socklen_t len;

void MComms(){//send and receive with serverM
    len = sizeof(serverMaddr);  //len is value/result 
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &serverMaddr, 
                &len); 
    buffer[n] = '\0'; 
    // printf("Client : %s\n", buffer);
    cout<<"The ServerC received an authentication request from the Main Server."<<endl;
    string x = "stuff";
    x = buffer; 
    checkCredentials();
    if(x=="hello"){
        sendto(sockfd, (const char *)"hi there", strlen("hi there"),  
            MSG_CONFIRM, (const struct sockaddr *) &serverMaddr, 
                len); 
        // printf("Hello message sent.\n");
    }else{
        // cout<<"response is"<<buffer<<endl;
        sendto(sockfd, (const char *)response, strlen(response),  
            MSG_CONFIRM, (const struct sockaddr *) &serverMaddr, 
                len); 
        // printf("Hello message sent.\n\n");
    }
    cout<<"The ServerC finished sending the response to the Main Server."<<endl;

}

// Driver code 
int main() { 
    startUDP(); 
    getCredentials();
    splitCredentials();  
    cout<<"The ServerC is up and running using UDP on port 21555."<<endl; 


    while(1){
        MComms();
    }
    
        
    return 0; 
}