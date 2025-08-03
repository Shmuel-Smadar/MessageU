# MessageU
## Prerequisites
You need python to run the server.
You C++ 17 or above to run the client.  
You need to use Boost and CryptoPP libraries to run the client.  
Boost: https://www.boost.org/ (Boost is also published as a NuGet)  
CryptoPP: https://github.com/weidai11/cryptopp


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
The database will be saved to a file named 'defensive.db'.

## Usage
<img width="541" height="322" alt="image" src="https://github.com/user-attachments/assets/7c357f0a-4845-4005-85d8-6d8b2caf6f5d" />

For two clients (X and Y) to be able to communciate with each other, they need to follow these steps:  
1. Both clients need to register on the same server.
2. Both client need to retrieve each other's public key from the server.
3. Client X sends a request for a secret key, using Y's public key.
4. Client Y check their messages, and sends a new secret key to X, encrypted with X's public key.
5. Client X checks their messages, and retrieves the secret key.
6. The two clients now can commumicate using the shared secret key.
