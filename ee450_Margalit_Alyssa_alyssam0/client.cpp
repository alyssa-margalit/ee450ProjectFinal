//communicates with serverM over tcp with dynamic local port assignment
//input a username and password, send to serverM @ tcp port 25555
//5 to 50 characters
//3 attempts to get the password right

//phase 2
//enter course code to query
//enter category
//send request to tcp server

//serverC serves on UDP port 21555
//serverCS serves on UDP port 22555
//serverEE serves on UDP port 23555
//serverM serves on UDP port 24555 and TCP port 25555
//Client serves on nothing, client for TCP port 2555


//to run
// g++ client.cpp -o client
// ./client

//send over tcp to port 25555

#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

#define PORT 25555 //tcp server port
using namespace std;

int debug = 1;
bool auth = false;//have the credentials been verified yet?
int tries = 1;
char total[101];
char username[50];
char password[50];

int sock = 0, valread, client_fd;
struct sockaddr_in serv_addr;
struct sockaddr_in my_addr;
unsigned int clientPort;

const char* hello = "0,Hello from client";//debugging message
char buffer[1024] = { 0 };
void inputCreds(){//function to input credentials from user- gets username and password and sends to serverM
        cout<<"Please enter username: ";
        cin.getline(username,50);
        cout<<"Please enter password: ";
        cin.getline(password,50); 
        // cout<<"username: "<<username<<endl;
        // cout<<"password: "<<password<<endl;
        strcpy(total,username);
        strcat(total,",");
        strcat(total,password);//concatenate into "username,password"
        // cout<<"total "<<total<<endl;
        send(sock,total,strlen(total),0);//send to serverM
        cout<<username<<" sent an autherntication request to the main server."<<endl;
        
        
}

char courseCode[30];
char category[30];
char totalQuery[60];

void inputCourseQuery(){//get course query from user and send to serverM
    cout<<"Please enter course code to query: "<<endl;
    cin.getline(courseCode,30);
    cout<<"Please enter category (Credit/Professor/Days/CourseName):"<<endl;
    cin.getline(category,30);
    strcpy(totalQuery,courseCode);
    strcat(totalQuery,",");
    strcat(totalQuery,category);//concatenate into "course,category"
    send(sock,totalQuery,strlen(totalQuery),0);//send to serverM
    cout<<username<<" sent a request to the main server."<<endl;

}
int startClient(){ //based off of code from "https://github.com/bozkurthan/Simple-TCP-Server-Client-CPP-Example"

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if ((client_fd
         = connect(sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    bzero(&my_addr, sizeof(my_addr));
    socklen_t len = sizeof(my_addr);
    getsockname(sock, (struct sockaddr *) &my_addr, &len);
    clientPort = ntohs(my_addr.sin_port);//since dynamically assigned, get tcp port number of client address
    // cout<<"port is: "<<my_addr.sin_port<<endl;
    
    return 0;
}


int main(){
    int x = startClient();//start up tcp

    if(x){
        cout<<"socket failed"<<endl;
    }else{
        cout<<"The client is up and running."<<endl;
    }
    char msg[1500];
    int n;
    string msgS;
    string msgAuth;
    while(1){
        memset(&msg, 0, sizeof(msg));//clear the buffer
        //if not authorize yet
        if(!auth){
            //3 total tries, start at try 1
            if(tries<=3){
                //input username and password and send to serverM
                inputCreds();
                //recieve response from serverM
                memset(&msg, 0, sizeof(msg)); //clear the buffer
                n = recv(sock, (char*)&msg, sizeof(msg), 0);
                // cout<<"authentication result: "<<msg<<endl;
                //if message is "correct" then auth = true
                msgAuth = msg;

                if(msgAuth=="correct"){//username and password correct
                    cout<<username<<" received the result of authentication using TCP over port "<<clientPort<<". Authentication Successful."<<endl;
                    auth = true;
                }else if(msgAuth == "wrong_pass"){//wrong password, try again
                      cout<<username<<" received the result of authentication using TCP over port "<<clientPort<<". Authentication Failed: Password does not match."<<endl;
                      cout<<"Attempts remaining: "<<3-tries<<endl;//how many tries left?
                   
                }else if(msgAuth =="wrong_username"){//username not found, try again
                      cout<<username<<" received the result of authentication using TCP over port "<<clientPort<<". Authentication Failed: Username Does not exist."<<endl;
                      cout<<"Attempts remaining: "<<3-tries<<endl;
                }

                //if the username or password was incorrect, increment tries
                tries++;

            }else{//tried 3 times? close socket and shut down
                cout<<"Authentication Failed for 3 attempts. Client will shut down."<<endl;
                close(client_fd);
                return 0;
            }

        }else{//authenticated already
            inputCourseQuery();
            memset(&msg, 0, sizeof(msg)); //clear the buffer
            n = recv(sock, (char*)&msg, sizeof(msg), 0);
            string response = msg;
            cout<<"The client received the response from the Main server using TCP over port "<<clientPort<<"."<<endl;
            if(response=="course not found"){
                cout<<"Didn't find the course: "<<courseCode<<"."<<endl;
            }else{
                cout<<"The "<<category<<" of "<<courseCode<<" is "<<msg<<"."<<endl;
                cout<<"\n\n--Start a new request--"<<endl;
            }

            //cout<<"query result: "<<msg<<endl;


        }
}


}
