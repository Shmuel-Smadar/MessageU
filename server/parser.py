import uuid
import time
import struct
from classes import Client
from constants import ClientCodes
from constants import ProtocolLengths
from constants import ServerCodes

def parse_request(request, db):
    clientID = request['clientID']
    code = request['code']
    version = request['version']
    payload = request['payload']
    if code == ClientCodes.REGISTRATION: 
        return handle_registration(payload, db)
    elif code == ClientCodes.CLIENT_LIST:
        return send_client_list(payload, db)

    else:
        return
    
def handle_registration(payload, db):
    try:
        name_end = payload.find(b'\0')
        if name_end == -1:
            raise ValueError("Invalid name in registration payload")
        
        name = payload[:name_end].decode('ascii')
        public_key = payload[ProtocolLengths.NAME:]
        if len(public_key) != ProtocolLengths.PUBLIC_KEY:
            raise ValueError("Invalid public key length in registration payload")
        if db.client_exists(name):
            return build_response(ServerCodes.ERROR, b'')

        client_id = uuid.uuid4().hex[:32]
        client = Client(ID=client_id, UserName=name, PublicKey=public_key, LastSeen=time.time())
        db.add_client(client)
        payload = bytes.fromhex(client_id)
        return build_response(ServerCodes.REGISTRATION_SUCCESSFUL, payload)
    except Exception as e:
        print(f"Error in handle_registration: {e}")
        return build_response(ServerCodes.ERROR, b'')
    
def send_client_list(payload, db):
    client_id = payload[:16].hex()
    client = db.get_client_by_id(client_id)
    if not client:
        return build_response(ServerCodes.ERROR, b'')
    db.update_last_seen(client_id)
    clients = db.get_clients(client_id)
    # Build payload
    response_payload = b''
    for c in clients:
        if c.ID == client.ID:
            continue
        client_id_bytes = bytes.fromhex(c.ID)
        name_bytes = c.UserName.encode('ascii') + b'\0'
        name_padded = name_bytes.ljust(255, b'\0')
        response_payload += client_id_bytes + name_padded
    return build_response(ServerCodes.RETURNED_CLIENT_LIST, response_payload)

def build_response(code, payload):
        payload_size = len(payload)
        header = struct.pack('<BHI', ServerCodes.VERSION, code, payload_size)
        return header + payload