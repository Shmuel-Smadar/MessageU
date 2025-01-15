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
        name_end = payload.find(b'\0')
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
        requested_client_id = payload[:ProtocolByteSizes.CLIENT_ID].hex()
        print(requested_client_id)
        public_key = db.get_client_by_id(requested_client_id).PublicKey
        if not public_key:
            print('error')
        return self.response_builder.build_public_key_response(public_key)
        
    def message_sent(self, client_id, payload, db: Database):
        if len(payload) < ProtocolByteSizes.MESSAGE_HEADER:
            raise ValueError("Payload too short to contain required fields.")
        sent_client_id = payload[:ProtocolByteSizes.CLIENT_ID].hex()
        message_type = payload[ProtocolByteSizes.CLIENT_ID]
        
        message_size_start = ProtocolByteSizes.CLIENT_ID + ProtocolByteSizes.MESSAGE_TYPE
        message_size_end = message_size_start + ProtocolByteSizes.MESSAGE_LENGTH
        message_size = int.from_bytes(payload[message_size_start:message_size_end], byteorder='little')
        
        expected_length = ProtocolByteSizes.MESSAGE_HEADER + message_size
        if len(payload) < expected_length:
            raise ValueError("Payload does not contain enough bytes for the specified message size.")
        content = payload[ ProtocolByteSizes.MESSAGE_HEADER:expected_length]
        
        message_id = uuid.uuid4().int & (1 << ProtocolByteSizes.MESSAGE_ID * 8) - 1
        message = Message(message_id, sent_client_id, client_id, message_type, content)
        db.add_message(message)
        return self.response_builder.build_public_message_sent_response(sent_client_id, message.ID) 

    def awaiting_messages(self, client_id, db: Database):
       messages = db.get_messages_for_client(client_id)
       return self.response_builder.build_awaiting_messages_response(messages)
   
    def invalid_code(self):
        return self.response_builder.build_error_response()
       
       