# filename: request_parser.py
# this file is responsible for extracting data from the requests on the client,
# doing various operation on them and calling the apropiate function to create the response

import uuid
from constants import ProtocolByteSizes
from constants import ServerCodes
from response_builder import ResponseBuilder
from database import Database
from classes import Message
class RequestParser:
    
    def __init__(self):
        self.response_builder = ResponseBuilder()
        
    def registration(self, payload, db: Database):
        name_end = payload.find(b'\0') #look for the end of the name
        if name_end == -1:
            raise ValueError("Invalid name in registration payload")
        name = payload[:name_end].decode('ascii')
        public_key = payload[ProtocolByteSizes.NAME:]
        if len(public_key) != ProtocolByteSizes.PUBLIC_KEY:
            raise ValueError("Invalid public key length in registration payload")
        if db.client_exists(name):
            return self.response_builder.build_error_response()
        return self.response_builder.build_registration_response(name, public_key, db)
        
    
    def client_list(self, client_id, db: Database):
        client = db.get_client_by_id(client_id)
        db.update_last_seen(client_id)
        if not client:
            return self.response_builder.build_error_response()
        return self.response_builder.build_client_list(client_id, db)
        
    def public_key(self, client_id, payload, db: Database):
        db.update_last_seen(client_id)
        requested_client_id = payload[:ProtocolByteSizes.CLIENT_ID].hex()
        public_key = db.get_client_by_id(requested_client_id).PublicKey
        return self.response_builder.build_public_key_response(public_key)
        
    def message_sent(self, client_id, payload, db: Database):
        if len(payload) < ProtocolByteSizes.MESSAGE_HEADER:
            return self.invalid_request()
        db.update_last_seen(client_id)
        sent_client_id = payload[:ProtocolByteSizes.CLIENT_ID].hex()
        message_type = payload[ProtocolByteSizes.CLIENT_ID]
        
        message_size_start = ProtocolByteSizes.CLIENT_ID + ProtocolByteSizes.MESSAGE_TYPE
        message_size_end = message_size_start + ProtocolByteSizes.MESSAGE_LENGTH
        message_size = int.from_bytes(payload[message_size_start:message_size_end], byteorder='little')
        
        expected_length = ProtocolByteSizes.MESSAGE_HEADER + message_size
        if len(payload) < expected_length:
            return self.invalid_request()
        db.update_last_seen(client_id)
        content = payload[ ProtocolByteSizes.MESSAGE_HEADER:expected_length]
        
        message_id = uuid.uuid4().int & (1 << ProtocolByteSizes.MESSAGE_ID * 8) - 1
        message = Message(message_id, sent_client_id, client_id, message_type, content)
        db.add_message(message)
        return self.response_builder.build_public_message_sent_response(sent_client_id, message.ID) 

    def awaiting_messages(self, client_id, db: Database):
       db.update_last_seen(client_id)
       messages = db.get_messages_for_client(client_id)
       db.delete_messages_for_client(client_id)
       return self.response_builder.build_awaiting_messages_response(messages)
   
    def invalid_request(self):
        return self.response_builder.build_error_response()
       
       