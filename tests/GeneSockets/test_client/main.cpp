

#include <Chat_Client.h>
#include <Chat_ClientSSL.h>
#include <Chat_ClientAsync.h>

#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <stdexcept>

void* thread_func(void* pTr) {
    IChatClient* c = static_cast<IChatClient*>(pTr);
    while(c->isConn()) {
      Chat_Msg* msg = NULL;
      int rc = c->read(msg, 1);
      if (rc < 0) {
          std::cout << "Server close connection - Exitting!" << std::endl;
          c->disconn();
      } else if (rc > 0) {
          std::cout << msg->getMsg().c_str() << std::endl;
          delete msg;
      } else {
          usleep(200000);
      }
    }
    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: chat_client <host> <port> <Standar|SSL|Async>\n";
            return 1;
        }

        IChatClient* c;
        if (strcmp(argv[3],"SSL") == 0) {
            c = new Chat_ClientSSL();
            while (!((Chat_ClientSSL*)c)->connectToHost( argv[1], atoi(argv[2]), 5 )){
                std::cout << "Unable to connect to server - RETRYING!" << std::endl;
                sleep(2);
            }
            //clientSSL( argv[1], atoi(argv[2]) );
        } else if (strcmp(argv[3],"Async")==0) {
            c = new Chat_ClientAsync();
            while (!((Chat_ClientAsync*)c)->connectToHost( argv[1], atoi(argv[2]), 5 )){
                std::cout << "Unable to connect to server - RETRYING!" << std::endl;
                sleep(2);
            }            
            //clientAsync( argv[1], atoi(argv[2]) );
        } else if (strcmp(argv[3],"Standar")==0) { 
            c = new Chat_Client();
            while (!((Chat_Client*)c)->connectToHost( argv[1], atoi(argv[2]), 5 )){
                std::cout << "Unable to connect to server - RETRYING!" << std::endl;
                sleep(2);
            }                    
            //clientDefault( argv[1], atoi(argv[2]) );
        }    
        
        pthread_t   readThread;
        int rc = pthread_create (&readThread, NULL, thread_func, c);
        if ( rc != 0 ) {        
            throw std::runtime_error("Error in thread creation... (pthread_create())");
        } 

        int MAX_MSG_LENGTH = 512;

        char line[MAX_MSG_LENGTH + 1];
        while (std::cin.getline(line, MAX_MSG_LENGTH + 1))
        {
            if (strlen(line) > 0) {
                using namespace std; // For strlen and memcpy.
                Chat_Msg msg;
                msg.setMsg( line );
                if ((!c->isConn()) || (c->write(&msg, 1) < 0)) {
                    std::cout << "Server close connection - Exitting!" << std::endl;
                    break;
                }
            }else{
                break;
            }
        }
        c->disconn();
        pthread_join(readThread, NULL);        
        
    }
    catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
