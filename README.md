# Minitalk
This is a small data exchange program using UNIX signals.

#### ./server
When launched, this program with print it's PID to STDOUT and wait for incoming signals. The server will remain running until killed.

#### ./client
The client program will take a PID and a string as arguments and send the string to the PID using UNIX signals. The client will not terminate until is has received notification that the sent message has been recieved by the client.

Both programs are unicode compatible.

----
### Installation
```bash
git clone https://github.com/AuroraSloan/minitalk.git
```
----
## Usage
First, create the executables with make
```bash
make - compiles the server and client executables
make clean - removes all .o files
make fclean - removes all .o files and the executables
make re - removes all .o files and all executables before recompiling the programs
```

#### server
Run the server program and the PID list will be printed. I recommend to run the client and server in separate terminals, however they can be run in the same terminal.
>separate
```bash
./server
```
>together
```bash
./server&
```
In this case, the server will be running in the background. If you forget the PID, you can see is by typing the command:
```bash
jobs -p
```
#### client
Run the client program with the PID and a string.
```bash
./client <PID> <string>
```

To terminate the server, you can either press ``CTRL+C`` into the terminal where the server is running if it is not in the background.

Otherwise, you can kill the server with the command ``kill <PID>`` either in the terminal that the client is in, or if the server is running in the background.
