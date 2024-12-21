from typing import List
import uuid
import time
import struct
from classes import Client, Message

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
    
    def build_public_message_sent_response(self, to_client_id, ID):
        payload = bytes.fromhex(to_client_id)
        id_bytes = ID.to_bytes(4, byteorder='little', signed=False)
        payload += id_bytes
        return self.build_response(ServerCodes.MESSAGE_SENT, payload)
        
        
    def build_awaiting_messages_response(self, messages: List[Message]):
        payload = b''
        for message in messages:
            from_client_bytes = message.FromClient.encode('utf-8')[:16].ljust(16, b'\0')
            payload += from_client_bytes
            payload += struct.pack('>I', message.ID)
            payload += struct.pack('>B', message.Type)
            content_bytes = message.Content.encode('utf-8')
            payload += struct.pack('>I', len(content_bytes))
            payload += content_bytes
        return self.build_response(ServerCodes.RETURNED_AWAITING_MESSAGES, payload)
        
    def build_error_response(self):
        return self.build_response(ServerCodes.ERROR, b'')
    
    def build_response(self, code, payload):
        header = struct.pack('<BHI', ServerCodes.VERSION, code, len(payload))
        return header + payload
    