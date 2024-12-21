from constants import ProtocolLengths
from constants import ServerCodes
from response_builder import ResponseBuilder
from database import Database
from classes import Message
class RequestParser:
    
    def __init__(self):
        self.response_builder = ResponseBuilder()
        
    def registration(self, payload, db: Database):
        name_end = payload.find(b'\0')
        if name_end == -1:
            raise ValueError("Invalid name in registration payload")
        name = payload[:name_end].decode('ascii')
        public_key = payload[ProtocolLengths.NAME:]
        if len(public_key) != ProtocolLengths.PUBLIC_KEY:
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
        requested_client_id = payload[:16].hex()
        print(requested_client_id)
        public_key = db.get_client_by_id(requested_client_id).PublicKey
        if not public_key:
            print('error')
        return self.response_builder.build_public_key_response(public_key)
        
    def message_sent(self, client_id, payload, db: Database):
        if len(payload) < 21:
            raise ValueError("Payload too short to contain required fields.")
        sent_client_id = payload[:16].hex()
        message_type = payload[16]
        print(sent_client_id)
        message_size = int.from_bytes(payload[17:21], byteorder='little')
        expected_length = 21 + message_size
        if len(payload) < expected_length:
            raise ValueError("Payload does not contain enough bytes for the specified message size.")
        content = payload[21:21+message_size]
        #TODO:solve inconsistenry between request_parser and response_builder
        message = Message(1, client_id, sent_client_id, message_type, content)#TODO: Message ID generation
        db.add_message(message)
        return self.response_builder.build_public_message_sent_response(sent_client_id, message.ID) 

    def awaiting_messages(self, client_id, db: Database):
       messages = db.get_messages_for_client(client_id)
       return self.response_builder.build_awaiting_messages_response(messages)
       
       