import uuid
import time
from protocol import Protocol
from classes import Client

def parse_request(request, db):
    code = request['code']
    version = request['version']
    payload = request['payload']
    if code == 600: #TODO: check if python has nanums or alternative for the codes
        return handle_registration(version, payload, db)
    else:
        return
    
def handle_registration(version, payload, db):
    try:
        name_end = payload.find(b'\0')
        if name_end == -1:
            raise ValueError("Invalid name in registration payload")
        
        name = payload[:name_end].decode('ascii')
        public_key = payload[name_end+1:]
        if len(public_key) != 160:
            raise ValueError("Invalid public key length in registration payload")
        if db.client_exists(name):
            return Protocol.build_response(version, 9000, b'')

        client_id = uuid.uuid4().hex[:32]
        client = Client(ID=client_id, UserName=name, PublicKey=public_key, LastSeen=time.time())
        db.add_client(client)
        payload = bytes.fromhex(client_id)
        return Protocol.build_response(version, 2100, payload)
    except Exception as e:
        print(f"Error in handle_registration: {e}")
        return Protocol.build_response(version, 9000, b'')