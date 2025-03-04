# filename: response_builder.py
# this file is responsible for building the responses for all the client requests.
# uses data given from request_parser.

from typing import List
import uuid
import time
import struct
from classes import Client, Message

from constants import ProtocolByteSizes
from constants import ServerCodes

class ResponseBuilder:
    
    def build_registration_response(self, name, public_key, db) :
        try:
            client_id = uuid.uuid4().hex
            client = Client(ID=client_id, UserName=name, PublicKey=public_key, LastSeen=time.time())
            db.add_client(client)
            payload = bytes.fromhex(client_id)
            return self.build_response(ServerCodes.REGISTRATION_SUCCESSFUL, payload)
        except Exception as ex:
            return self.build_response(ServerCodes.ERROR, b'')
    
    def build_client_list(self, client_id, db):
        clients = db.get_clients(client_id)
        response_payload = b''
        for c in clients:
            if c.ID == client_id:
                continue
            client_id_bytes = bytes.fromhex(c.ID)
            name_bytes = c.UserName.encode('ascii') + b'\0'
            name_padded = name_bytes.ljust(ProtocolByteSizes.NAME, b'\0')
            response_payload += client_id_bytes + name_padded
        return self.build_response(ServerCodes.RETURNED_CLIENT_LIST, response_payload)
    
    def build_public_key_response(self, public_key):
        return self.build_response(ServerCodes.RETURNED_PUBLIC_KEY, public_key)
    
    def build_public_message_sent_response(self, to_client_id, ID):
        payload = bytes.fromhex(to_client_id)
        id_bytes = ID.to_bytes(4, byteorder='little')
        payload += id_bytes
        return self.build_response(ServerCodes.MESSAGE_SENT, payload)
        
        
    def build_awaiting_messages_response(self, messages: List[Message]):
        payload = b''
        for message in messages:
            payload += bytes.fromhex(message.FromClient)
            payload += message.ID.to_bytes(ProtocolByteSizes.MESSAGE_ID, byteorder='little')
            payload += message.Type.to_bytes(ProtocolByteSizes.MESSAGE_TYPE, byteorder='little')
            payload += (len(message.Content)).to_bytes(ProtocolByteSizes.MESSAGE_LENGTH, byteorder='little')
            payload += message.Content
        return self.build_response(ServerCodes.RETURNED_AWAITING_MESSAGES, payload)
        
    def build_error_response(self):
        return self.build_response(ServerCodes.ERROR, b'')
    
def build_response(self, code, payload):
    version = ServerCodes.VERSION.to_bytes(ProtocolByteSizes.VERSION, byteorder='little')
    code_bytes = code.to_bytes(ProtocolByteSizes.CODE, byteorder='little')
    payload_size = len(payload).to_bytes(ProtocolByteSizes.PAYLOAD_SIZE, byteorder='little')

    header = version + code_bytes + payload_size
    return header + payload
