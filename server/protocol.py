import struct
from constants import ProtocolByteSizes
from request_parser import RequestParser
from constants import ClientCodes
class Protocol:
    def __init__(self):
        self.request_parser = RequestParser()
        
    def process_requests(self, data, db):
        if len(data['request']) < ProtocolByteSizes.HEADER:
            return
        client_id, version, code, payload_size = struct.unpack('<16sBHI', data['request'][:ProtocolByteSizes.HEADER])
        total_size = ProtocolByteSizes.HEADER + payload_size
        if len(data['request']) < total_size:
            return
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
            return
        data['response'] += response
        data['request'] = data['request'][total_size:]
        