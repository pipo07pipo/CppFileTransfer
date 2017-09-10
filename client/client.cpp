#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include "md5.h"
#include <openssl/md5.h>
using namespace std;



int fileSEND(char *server, int PORT, char *lfile, char *rfile){
    int socketDESC;
    struct sockaddr_in serverADDRESS;
    struct hostent *hostINFO;
    //FILE * file_to_send;
    char ch;
    char toSEND[1];
    char remoteFILE[4096];
    int nextPercent=1,currentPercent=1, percent;
 
 
    hostINFO = gethostbyname(server);
    if (hostINFO == NULL) {
        printf("Problem interpreting host\n");
        return 1;
    }
 
    socketDESC = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDESC < 0) {
        printf("Cannot create socket\n");
        return 1;
    }
 
    serverADDRESS.sin_family = hostINFO->h_addrtype;
    memcpy((char *) &serverADDRESS.sin_addr.s_addr, hostINFO->h_addr_list[0], hostINFO->h_length);
    serverADDRESS.sin_port = htons(PORT);
                 
    if (connect(socketDESC, (struct sockaddr *) &serverADDRESS, sizeof(serverADDRESS)) < 0) {
        printf("Cannot connect\n");
        return 1;
    }
    std::fstream file_to_send;
    file_to_send.open (lfile, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!file_to_send) {
        printf("Error opening file\n");
        close(socketDESC);
        return 0;
        } else {
    long fileSIZE;
    std::string str;
    file_to_send.seekg(0,std::ios::end);
    str.reserve(file_to_send.tellg());
    str.assign( (std::istreambuf_iterator<char>(file_to_send) ),
                        (std::istreambuf_iterator<char>()    )  );
    

    fileSIZE = file_to_send.tellg();
    file_to_send.seekg(0,std::ios::beg);
    
    
    sprintf(remoteFILE,"FBEGIN:%s:%d\r\n", rfile, fileSIZE);
    send(socketDESC, remoteFILE, sizeof(remoteFILE), 0);
 
    percent = fileSIZE / 100;
    while(file_to_send.get(ch)){
        toSEND[0] = ch;
        send(socketDESC, toSEND, 1, 0);
        if( nextPercent == currentPercent ) {
            printf("33[0;0H");
            printf( "\33[2J");
            printf("Filename: %s\n", lfile);
            printf("Filesize: %d Kb\n", fileSIZE / 1024);
            printf("Percent : %d%% ( %d Kb)\n",nextPercent / percent ,nextPercent / 1024);
            nextPercent+=percent;
        }
        currentPercent++;
 
    }
    cout << "md5 of this file : " << md5(str) << endl;
    }
    file_to_send.close();
    close(socketDESC);
 
    return 0;
}
 
int main()
{
     
    fileSEND("localhost", 31337, "4kpic.jpg", "4kpic.jpg");
    return 0;
}
