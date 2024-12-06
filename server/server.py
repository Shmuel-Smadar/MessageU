import socket
import selectors
import struct
import os


DEFAULT_PORT = 1357
PORT_FILE = 'myport.info'
IP = '127.0.0.1'

def get_port():
    if os.path.exists(PORT_FILE):
        with open(PORT_FILE, 'r') as f:
            try:
                port = int(f.read().strip())
                return port
            except ValueError:
                print("Invalid port number in myport.info. Using default port {DEFAULT_PORT}.")
    else:
        print(f"Port file {PORT_FILE} not found. Using default port {DEFAULT_PORT}.")
    return DEFAULT_PORT

def main():
    port = get_port()
    server_address = (IP, port)
    print(f"Starting server on port {port}...")

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(server_address)
    sock.listen()
    sock.setblocking(False)

    sel = selectors.DefaultSelector()
    sel.register(sock, selectors.EVENT_READ, data=None)
    print("Server is listening for incoming connections...")
    try:
        while True:
            events = sel.select(timeout=None)

    except KeyboardInterrupt:
        print("Server is shutting down.")
    finally:
        sel.close()

if __name__ == "__main__":
    main()