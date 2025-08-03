# MessageU
## Prerequisites
You need python to run the server.
You C++ 17 or above to run the client.  
You need to use Boost and CryptoPP libraries to run the client.
Boost: https://www.boost.org/
CryptoPP: https://github.com/weidai11/cryptopp
(Boost is also published as  a NuGet)

## Building Client
```
git clone https://github.com/Shmuel-Smadar/MessageU.git
cd MessageU-main
(compile with your favorite C++ compiler)
./main
```
## Building Server
```
git clone https://github.com/Shmuel-Smadar/MessageU.git
cd MessageU-main
cd server
py server.py
```

You can configure the server to use a different port by changing the 'myport.info' file.
the database will be saved to a file named 'defensive.db'.
