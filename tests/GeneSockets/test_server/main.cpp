
#include <Chat_Server.h>
#include <Chat_Session.h>
#include <Chat_SessionSSL.h>
#include <Chat_SessionAsync.h>

void serverDefault(Chat_Server* server){
    Chat_Session session;

    Chat_Client* peer = new Chat_Client();
    while (1) {
        int rc = server->accept(peer, 5);
        if (rc < 0)
            std::cout << "Acceptor error!" << std::endl;
        else if (rc == 0)
            std::cout << "Acceptor timeout!" << std::endl;
        else {
            std::cout << "New connection!" << std::endl;
            session.add(peer); 
            peer = new Chat_Client();
        }            
    }
}

void serverAsync(Chat_Server* server){
    Chat_SessionAsync session;

    Chat_ClientAsync* peer = new Chat_ClientAsync();
    while (1) {
        int rc = server->accept(peer, 5);
        if (rc < 0)
            std::cout << "Acceptor error!" << std::endl;
        else if (rc == 0)
            std::cout << "Acceptor timeout!" << std::endl;
        else {
            std::cout << "New connection!" << std::endl;
            session.add(peer); 
            peer = new Chat_ClientAsync();
        }            
    }
}

void serverSSL(Chat_Server* server){
    
    Chat_SessionSSL session;
    Chat_ClientSSL* peer = new Chat_ClientSSL();
    while (1) {
        int rc = server->accept(peer, 5);
        if (rc < 0)
            std::cout << "Acceptor error!" << std::endl;
        else if (rc == 0)
            std::cout << "Acceptor timeout!" << std::endl;
        else {
            std::cout << "New connection!" << std::endl;
            session.add(peer); 
            peer = new Chat_ClientSSL();
        }            
    }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc < 3)
    {
      std::cerr << "Usage: chat_server <port> <Standar|SSL|Async>\n";
      return 1;
    }

    using namespace std; // For atoi.
    Chat_Server* server( new Chat_Server() );
    if ( server->listen(atoi(argv[1])) ) {
        
        if (strcmp(argv[2],"SSL") == 0) {
            serverSSL(server);
        } else if (strcmp(argv[2],"Async")==0) {
            serverAsync(server);
        } else if (strcmp(argv[2],"Standar")==0) { 
            serverDefault(server);
        }
    }
    
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
