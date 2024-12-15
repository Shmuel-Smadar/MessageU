from constants import ProtocolLengths
from constants import ServerCodes
from response_builder import ResponseBuilder
from database import Database
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
        requested_client_id = payload[0:32].decode('utf-8')
        public_key = db.get_client_by_id(requested_client_id).PublicKey
        return self.response_builder.build_public_key_response(public_key)
        
        

