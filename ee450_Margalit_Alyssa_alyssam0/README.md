# 450Projectv2

a. Alyssa Margalit
b. 6127012555
c. I have written a program in which a client can connect to a server, login with a unique username and password, and then request information about different courses in EE and CS departments. 
d. client: communicates with serverM over TCP to give credentials and make requests.

    serverM: receives credentials and requests from client over tcp, passes them onto credential server, ee department server, and cs department server over udp and then relays the results from those servers back to the client over tcp. It also encrypts credentials before sending to credential server.
    
    serverC: credential server recieves encrypted credentials and returns whether they are a match to a given text file of encrypted usernames and passwords
    
    serverEE: reads from a txt file of classes and information about those classes, parses this data, returns correct portion of the data when a request is recieved by serverM.
    
    serverCS: same as serverEE, but with a CS class txt file instead of EE.
e. credentials from client to serverM: "username,password"
    course queries from client to serverM: "course,category"
    encrypted credentials from serverM to serverC: "username,password"
    response from serverC to serverM: "correct","wrong_pass", or "wrong_username"
    relay credential response from serverM to client: "correct","wrong_pass" or "wrong_username"
    course queries from serverM to serverEE or CS: "course,category"
    response from serverEE or CS to serverM: "[requested category data]"
    relay serverEE or CS response from serverM to client: "[requested category data]"
g. none
h. to setup tcp: borrowed some lines from:
        "https://github.com/bozkurthan/Simple-TCP-Server-Client-CPP-Example"
        client function: startClient()
        serverM function: startTCP()
