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
        clientID, version, code, payload_size = struct.unpack('<16sBHI', data['request'][:ProtocolLengths.HEADER])
        total_size = ProtocolLengths.HEADER + payload_size
        if len(data['request']) < total_size:
            return
        payload = data['request'][ProtocolLengths.HEADER:total_size]
        request = {
            'clientID': clientID,
            'version': version,
            'code': code,
            'payload_size': payload_size,
            'payload': payload,
        }
        response = self.parse_request(request, db)
        data['response'] += response
        data['request'] = data['request'][total_size:]
    
    def parse_request(self, request, db):
        clientID = request['clientID']
        code = request['code']
        version = request['version']
        payload = request['payload']
        if code == ClientCodes.REGISTRATION: 
            return self.request_parser.registration(payload, db)
        elif code == ClientCodes.CLIENT_LIST:
            return self.request_parser.client_list(payload, db)
        else:
            return