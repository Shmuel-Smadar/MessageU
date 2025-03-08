import socket
import selectors
import os
import traceback
from database import Database
from protocol import Protocol

DEFAULT_PORT = 1357
IP = '127.0.0.1'
PORT_FILE = 'myport.info'

def get_port():
    try:
        with open(PORT_FILE, 'r') as f:
            port = int(f.read().strip())
            return port
    except FileNotFoundError:
        print(f"Port file '{PORT_FILE}' not found. Using default port {DEFAULT_PORT}.")
    except ValueError:
        print(f"Invalid port number in '{PORT_FILE}'. Using default port {DEFAULT_PORT}.")
    return DEFAULT_PORT

def unregister_socket(sel, sock):
    try:
        sel.unregister(sock)
    except Exception:
        pass

def main():
    port = get_port()
    server_address = (IP, port)
    print(f"Starting server on port {port}...")

    db = Database()
    protocol = Protocol()
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(server_address)
        sock.listen()
        sock.setblocking(False)
    except Exception as e:
        print(f"Failed to start the server: {e}")
        return

    sel = selectors.DefaultSelector()
    sel.register(sock, selectors.EVENT_READ, data=None)
    print("Server is listening for incoming connections...")

    try:
        while True:
            try:
                events = sel.select(timeout=None)
            except Exception as e:
                print(f"Selector error: {e}")
                continue

            for key, mask in events:
                try:
                    if key.data is None:
                        accept_connection(key.fileobj, sel)
                    else:
                        service_connection(key, mask, sel, db, protocol)
                except Exception as e:
                    print(f"Error handling event: {e}")
                    continue
    except KeyboardInterrupt:
        print("Server is shutting down.")
    finally:
        sel.close()
        db.close()

def accept_connection(sock, sel):
    try:
        conn, address = sock.accept()
        print(f"Accepted connection from {address}")
        conn.setblocking(False)
        data = {'address': address, 'request': b'', 'response': b''}
        sel.register(conn, selectors.EVENT_READ | selectors.EVENT_WRITE, data=data)
    except Exception as e:
        print(f"Error accepting connection: {e}")
        traceback.print_exc()

def service_connection(key, mask, sel, db, protocol):
    sock = key.fileobj
    data = key.data
    try:
        if mask & selectors.EVENT_READ:
            handle_read(sock, data, sel, db, protocol)
        if mask & selectors.EVENT_WRITE:
            handle_write(sock, data, sel)
    except Exception as e:
        print(f"Error servicing connection: {e}")
        unregister_socket(sel, sock)
        sock.close()

def handle_read(sock, data, sel, db, protocol):
    try:
        recv_data = sock.recv(4096)
        if recv_data:
            data['request'] += recv_data
            protocol.process_requests(data, db)
        else:
            print(f"Closing connection to {data['address']}")
            unregister_socket(sel, sock)
            sock.close()
    except Exception as e:
        print(f"Error while reading data: {e}")
        unregister_socket(sel, sock)
        sock.close()

def handle_write(sock, data, sel):
    if data['response']:
        try:
            sent = sock.send(data['response'])
            data['response'] = data['response'][sent:]
        except ConnectionResetError:
            print(f"Connection reset by {data['address']}")
            unregister_socket(sel, sock)
            sock.close()
        except Exception as e:
            print(f"Error writing data: {e}")
            unregister_socket(sel, sock)
            sock.close()

if __name__ == "__main__":
    main()
