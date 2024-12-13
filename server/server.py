import socket
import selectors
import struct
import os
from database import Database
from parser import parse_request

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
                print("Invalid port number in myport.info. Using default port 1357.")
    else:
        print(f"Port file {PORT_FILE} not found. Using default port {DEFAULT_PORT}.")
    return DEFAULT_PORT

def main():
    port = get_port()
    server_address = (IP, port)
    print(f"Starting server on port {port}...")

    db = Database()
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
                    service_connection(key, mask, sel, db) # Existing connection
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

def service_connection(key, mask, sel, db):
    sock = key.fileobj
    data = key.data
    if mask & selectors.EVENT_READ:
        handle_read(sock, data, sel, db)
    if mask & selectors.EVENT_WRITE:
        handle_write(sock, data, sel)

def handle_read(sock, data, sel, db):
    recv_data = sock.recv(4096)
    if recv_data:
        data['request'] += recv_data
        process_requests(data, db)
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

# Maybe should be moved to protocol.py
def process_requests(data, db):
    while True:
        if len(data['request']) < 7:
            break
        version, code, payload_size = struct.unpack('<BHI', data['request'][:7])
        total_size = 7 + payload_size
        if len(data['request']) < total_size:
            break
        payload = data['request'][7:total_size]
        request = {
            'version': version,
            'code': code,
            'payload_size': payload_size,
            'payload': payload,
        }
        response = parse_request(request, db)
        data['response'] += response
        data['request'] = data['request'][total_size:]


if __name__ == "__main__":
    main()
