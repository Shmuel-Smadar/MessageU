import struct
from constants import ProtocolLengths
from constants import ServerCodes
from request_parser import RequestParser
from constants import ClientCodes
class Protocol:
    def __init__(self):
        self.request_parser = RequestParser()
        
    def process_requests(self, data, db):
        if len(data['request']) < ProtocolLengths.HEADER:
            return
        client_id, version, code, payload_size = struct.unpack('<16sBHI', data['request'][:ProtocolLengths.HEADER])
        total_size = ProtocolLengths.HEADER + payload_size
        if len(data['request']) < total_size:
            return
        payload = data['request'][ProtocolLengths.HEADER:total_size]
        if code == ClientCodes.REGISTRATION: 
            response = self.request_parser.registration(payload, db)
        elif code == ClientCodes.CLIENT_LIST:
            response = self.request_parser.client_list(client_id.hex(), db) 
        else:
            return
        data['response'] += response
        data['request'] = data['request'][total_size:]
        