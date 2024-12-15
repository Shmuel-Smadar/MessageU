import uuid
import time
import struct
from classes import Client

from constants import ProtocolLengths
from constants import ClientCodes
from constants import ServerCodes

class ResponseBuilder:
    
    def build_registration_response(self, name, public_key, db) :
        try:
            client_id = uuid.uuid4().hex[:32]
            client = Client(ID=client_id, UserName=name, PublicKey=public_key, LastSeen=time.time())
            db.add_client(client)
            payload = bytes.fromhex(client_id)
            return self.build_response(ServerCodes.REGISTRATION_SUCCESSFUL, payload)
        except Exception as ex:
            print(f"Error in handle_registration: {ex}")
            return self.build_response(ServerCodes.ERROR, b'')
    
    def build_client_list(self, client_id, db):
        clients = db.get_clients(client_id)
        response_payload = b''
        for c in clients:
            if c.ID == client_id:
                continue
            client_id_bytes = bytes.fromhex(c.ID)
            name_bytes = c.UserName.encode('ascii') + b'\0'
            name_padded = name_bytes.ljust(255, b'\0')
            response_payload += client_id_bytes + name_padded
        return self.build_response(ServerCodes.RETURNED_CLIENT_LIST, response_payload)
    
    def build_public_key_response(self, public_key):
        return self.build_response(ServerCodes.RETURNED_PUBLIC_KEY, public_key)
        
    def build_error_response(self):
        return self.build_response(ServerCodes.ERROR, b'')
    
    def build_response(self, code, payload):
        payload_size = len(payload)
        header = struct.pack('<BHI', ServerCodes.VERSION, code, payload_size)
        return header + payload
    