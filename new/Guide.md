### The Architecture 

1. First we read the Map (default.conf) With : #ConfigParser#
    We read default.conf, parse it, and hand a vector of ServerConfig obj to the serverManager 

2. Building the Serve : 
    With setupServer, We loop through those configs, open the non-blocking newtork sockets, 
    and save them in our FD maps. 
3
