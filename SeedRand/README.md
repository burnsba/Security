Simple proof of concept to look at random seed exploit.

A master application starts and listens for incoming connections on port 5000 (uses sockets). When a client 
connects, a random number is sent to the client once a second. The random generator is seeded once every second.

The client code connects to the master. When it receives a random number, it iterates over the last second
or so of time values (1100 ms) that the server could potentially be seeded by. If it finds a seed that generates the
same random number, the seed is printed.
