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
#define PORTEE     23555 
#define PORTM      24555
#define MAXLINE 1024

using namespace::std;

struct sockaddr_in     serverMaddr;
int sockfd; 
char buffer[MAXLINE]; 
const char *hello = "Hello from EE SERVER"; 
struct sockaddr_in servaddr, cliaddr; 

void startUDP(){
    //printf("hello");
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
    servaddr.sin_port = htons(PORTEE);

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
    cout<<"The ServerEE is up and running using UDP on port 23555"<<endl;
}
//______________________________________________________
//---------------read text file------------------------
int size;

vector<string> classes;
vector<string> courseNumber;
vector<string> credit;
vector<string> professor;
vector<string> days;
vector<string> courseName;
vector<string> temp2;
void getClasses(){//read from ee.txt and parse data into vectors
    ifstream in("ee.txt");
    string temp;
    while(getline(in,temp)){
        if(temp.size()>0){
            classes.push_back(temp);
            stringstream ss(temp);
            while(ss.good()){
                string substr;
                getline(ss,substr,',');
                temp2.push_back(substr);
            }
            courseNumber.push_back(temp2.at(0));
            credit.push_back(temp2.at(1));
            professor.push_back(temp2.at(2));
            days.push_back(temp2.at(3));
            courseName.push_back(temp2.at(4));
            temp2.clear();
        }
    }
    //debugging
    // for(size_t i = 0; i<courseNumber.size();i++){
    //        cout<<courseNumber.at(i)<<endl;
    //        cout<<credit.at(i)<<endl;
    //        cout<<professor.at(i)<<endl;
    //        cout<<days.at(i)<<endl;
    //        cout<<courseName.at(i)<<endl;
    //        cout<<"\n\n";
    //    }
}
string courseQuery = "EE520";
string tempCourse;
// const char *responseM = "hello";
string responseM;
vector <string> parsed;
void parseCourseCode(){//parse recieved message into course and category
    parsed.clear();//clear
    string temp = buffer;
    
    stringstream ss(temp);
    while(ss.good()){
        string substr;
        getline(ss,substr,',');
        parsed.push_back(substr);
        //cout<<"SUBSTR"<<substr<<endl;
    }

}

void checkMatch(){//does the course you requested exist? if so, return the data category requested
    parseCourseCode();
    cout<<"The ServerEE received a request from the Main Server about the "<<parsed.at(1)<<" of "<<courseQuery<<"."<<endl;
       
     for(size_t i = 0; i<classes.size();i++){
        tempCourse = courseNumber.at(i);
        size_t index = tempCourse.find(courseQuery);
        //cout<<tempCourse<<endl;
        if(index != string::npos){
            // cout<<classes.at(i)<<endl;
            //cout<<"QEURY TYPE"<<parsed.at(1)<<endl;
            string queryType = parsed.at(1);
            // cout<<"queryType"<<queryType<<"."<<endl;
            string c = "hello there";
            if(queryType=="Credit"){
                // cout<<"getting credit"<<endl;
                c = credit.at(i);
            }else if(queryType=="Professor"){
                // cout<<"getting Professor"<<endl;
                c = professor.at(i);
            }else if(queryType=="Days"){
                // cout<<"getting Days"<<endl;
                c = days.at(i);
            }else if(queryType=="CourseName"){
                // cout<<"getting CourseName"<<endl;
                //c = courseName.at(i).substr(0,courseName.at(i).size()-1);
                c = courseName.at(i);
            }else{
                c = "invalid category";
            }
            
            if(queryType=="CourseName"){//something weird with \n character in coursename, so just remove it
                c = c.substr(0,c.size()-1);
            }
            string ts = c;
            responseM = c.c_str();
            
           // cout<<"responseM is now "<<responseM<<"."<<endl;
           string tx = "The course information has been found: The "+parsed.at(1)+" of "+courseQuery+" is "+ts+".";
           cout<<tx<<endl;
            //cout<<"The course information has been found: The "<<parsed.at(1)<<" of "<<courseQuery<<" is "<<ts<<"."<<endl;
            break;//if found, break
        }else if(i==classes.size()-1){//if not found and end of vector, it doesn't exist
            //course code not found
            responseM = "course not found";
            cout<<"Didn't find the couse: "<<courseQuery<<"."<<endl;
        }
        
     }
    
}


//___________________________________________________
int n; 
socklen_t len;
// char sendData[1024];
// const char* c_str() const ;
void MComms(){//communicate with serverM
    len = sizeof(serverMaddr);  //len is value/result 
    memset(&buffer, 0, sizeof(buffer));
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &serverMaddr, 
                &len); 
    buffer[n] = '\0'; 
    // printf("ServerM : %s\n", buffer);
    courseQuery = buffer;
    courseQuery = courseQuery.substr(0,5);
    checkMatch();

    string s = responseM;
    int n = s.length();
    char char_array[n+1];
    strcpy(char_array,s.c_str());
    // for(int i = 0; i<n;i++){
    //     cout<<char_array[i];
    // }
    
    int x = sendto(sockfd, (const char *)char_array, strlen(char_array),  
        MSG_CONFIRM, (const struct sockaddr *) &serverMaddr, 
            len); 
    // cout<<"sent "<<x<<"bytes"<<endl;
    // printf("Hello message sent.\n");
    //  cout<<"The course information has been found: The "<<parsed.at(1)<<" of "<<courseQuery<<" is "<<char_array<<"."<<endl;
    cout<<"The ServerEE finished sending the response to the Main Server."<<endl;
}

// Driver code 
int main() { 
    startUDP(); 
    getClasses();    
    while(1){
        MComms();//continuously wait for serverM to say something so serverEE can generate a response
    }
    
        
    return 0; 
}