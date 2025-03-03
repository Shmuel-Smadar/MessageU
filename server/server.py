# filename: server.py
# this file is responsible for creating the server and listening for connections from clients
# gathering the data, sending it for parsing and sending the response.

import socket
import selectors
import os
from database import Database
from protocol import Protocol

DEFAULT_PORT = 1357
IP = '127.0.0.1'
PORT_FILE = 'myport.info'

def get_port():
    if os.path.exists(PORT_FILE):
        with open(PORT_FILE, 'r') as f:
            try:
                port = int(f.read().strip())
                return port
            except ValueError:
                print(f"Invalid port number in myport.info. Using default port {DEFAULT_PORT}.")
    else:
        print(f"Port file {PORT_FILE} not found. Using default port {DEFAULT_PORT}.")
    return DEFAULT_PORT

def main():
    port = get_port()
    server_address = (IP, port)
    print(f"Starting server on port {port}...")

    db = Database()
    protocol = Protocol()
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
            for key, mask in events:
                if key.data is None:
                    accept_connection(key.fileobj, sel) # New connection
                else:
                    service_connection(key, mask, sel, db, protocol) # Existing connection
    except KeyboardInterrupt:
        print("Server is shutting down.")
    finally:
        sel.close()
        db.close()

def accept_connection(sock, sel):
    conn, address = sock.accept()
    print(f"Accepted connection from {address}")
    conn.setblocking(False)
    data = {'address': address, 'request': b'', 'response': b''}
    sel.register(conn, selectors.EVENT_READ | selectors.EVENT_WRITE, data=data)

def service_connection(key, mask, sel, db, protocol):
    sock = key.fileobj
    data = key.data
    if mask & selectors.EVENT_READ:
        handle_read(sock, data, sel, db, protocol)
    if mask & selectors.EVENT_WRITE:
        handle_write(sock, data, sel)

def handle_read(sock, data, sel, db, protocol):
    recv_data = sock.recv(4096)
    if recv_data:
        data['request'] += recv_data
        protocol.process_requests(data, db)
    else:
        print(f"Closing connection to {data['address']}")
        sel.unregister(sock)
        sock.close()

def handle_write(sock, data, sel):
    if data['response']:
        try:
            sent = sock.send(data['response'])
            data['response'] = data['response'][sent:]
        except ConnectionResetError:
            print(f"Connection reset by {data['address']}")
            sel.unregister(sock)
            sock.close()


if __name__ == "__main__":
    main()
