//#define SERVER
#ifdef SERVER
#include "Server.h"
#else
#include "Client.h"
#endif

int main()
{
    #ifdef SERVER
        return serverMain();
    #else
        return clientMain();
    #endif

    return 0;
}