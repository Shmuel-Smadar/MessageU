# filename: protocol.py
# this file is responsible for handling the flow that requests from the client will go through.

import struct
from constants import ProtocolByteSizes
from request_parser import RequestParser
from constants import ClientCodes
class Protocol:
    def __init__(self):
        self.request_parser = RequestParser()
        
    def process_requests(self, data, db):
        try:
            request = data['request']
            if len(request) < ProtocolByteSizes.HEADER:
                raise ValueError("Invalid header")
            client_id_end = ProtocolByteSizes.CLIENT_ID
            version_end = client_id_end + ProtocolByteSizes.VERSION
            code_end = version_end + ProtocolByteSizes.CODE
            payload_size_end = code_end + ProtocolByteSizes.PAYLOAD_SIZE
            
            client_id = request[:client_id_end]
            version = request[client_id_end]
            code = int.from_bytes(request[version_end:code_end], byteorder='little')
            payload_size = int.from_bytes(request[code_end:payload_size_end], byteorder='little')
            total_size = ProtocolByteSizes.HEADER + payload_size
            if len(request) < total_size:
                raise ValueError("Invalid request data")
            payload = data['request'][ProtocolByteSizes.HEADER:total_size]
            if code == ClientCodes.REGISTRATION: 
                response = self.request_parser.registration(payload, db)
            elif code == ClientCodes.CLIENT_LIST:
                response = self.request_parser.client_list(client_id.hex(), db) 
            elif code == ClientCodes.PUBLIC_KEY:
                response = self.request_parser.public_key(client_id.hex(), payload, db)
            elif code == ClientCodes.MESSAGE_SENT:
                response = self.request_parser.message_sent(client_id.hex(), payload, db)
            elif code == ClientCodes.AWAITING_MESSAGES:
                response = self.request_parser.awaiting_messages(client_id.hex(), db)
            else:
                response = self.request_parser.invalid_request()
        except:
            response = self.request_parser.invalid_request()
        data['response'] += response
        data['request'] = data['request'][total_size:]
        