# MessageU

## Prerequisites

- Python to run the server.
- C++ 17 or above to run the client.
- Boost and Crypto++ libraries to run the client.

Boost: https://www.boost.org/ (Boost is also published as a NuGet)

Crypto++: https://github.com/weidai11/cryptopp

## Building Client

```sh
git clone https://github.com/Shmuel-Smadar/MessageU.git
cd MessageU-main
cd client
nuget restore client.sln
msbuild client.sln /p:Configuration=Debug /p:Platform=x86
```

The client uses NuGet packages for Boost and Crypto++ and lets Visual Studio choose the installed default C++ platform toolset.
The `server.info` file is copied beside the executable during build and currently points to the deployed ECS server at `51.85.83.9:1357`.
Change `client/server.info` if the server endpoint changes.

## Building Server

```sh
git clone https://github.com/Shmuel-Smadar/MessageU.git
cd MessageU-main
cd server
py server.py
```

You can configure the server to use a different port by changing the `myport.info` file.

You can also configure the server with environment variables:

- `HOST`, default `127.0.0.1`
- `PORT`, default `1357`
- `DATABASE_FILE`, default `defensive.db`

The database will be saved to a file named `defensive.db`.

## Running Server With Docker

```sh
cd server
docker build -t messageu-server .
docker run --rm -p 1357:1357 -v messageu-data:/data messageu-server
```

The container listens on `0.0.0.0:1357` and stores the SQLite database at `/data/defensive.db`.

## Usage
<img width="541" height="322" alt="image" src="https://github.com/user-attachments/assets/7c357f0a-4845-4005-85d8-6d8b2caf6f5d" />

For two clients (X and Y) to be able to communicate with each other, they need to follow these steps:

1. Both clients need to register on the same server.
2. Both clients need to retrieve each other's public key from the server.
3. Client X sends a request for a secret key, using Y's public key.
4. Client Y checks their messages, and sends a new secret key to X, encrypted with X's public key.
5. Client X checks their messages, and retrieves the secret key.
6. The two clients can now communicate using the shared secret key.

Have fun, meaningful, and encrypted conversations :)
