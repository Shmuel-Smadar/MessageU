import struct
from constants import ProtocolLengths
from constants import ServerCodes
from parser import parse_request

class Protocol:

    def process_requests(data, db):
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
        response = parse_request(request, db)
        data['response'] += response
        data['request'] = data['request'][total_size:]
    
