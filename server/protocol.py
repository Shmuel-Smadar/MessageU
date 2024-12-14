import struct
from constants import ProtocolLengths
from constants import ServerCodes
class Protocol:
    HEADER_FORMAT = '<BHI'  # Version - 1 byte, Code - 2 bytes, Payload size - 4 bytes

    @staticmethod
    def parse_request(data):
        if len(data) < ProtocolLengths.HEADER:
            return None
        version, code, payload_size = struct.unpack('<BHI', data[:ProtocolLengths.HEADER])
        if len(data) < ProtocolLengths.HEADER + payload_size:
            return None
        payload = data[ProtocolLengths.HEADER:ProtocolLengths.HEADER+payload_size]
        return {
            'version': version,
            'code': code,
            'payload_size': payload_size,
            'payload': payload,
        }

    @staticmethod
    def build_response(code, payload):
        payload_size = len(payload)
        header = struct.pack('<BHI', ServerCodes.VERSION, code, payload_size)
        return header + payload
