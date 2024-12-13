import struct

class Protocol:
    HEADER_FORMAT = '<BHI'  # Version - 1 byte, Code - 2 bytes, Payload size - 4 bytes

    @staticmethod
    def parse_request(data):
        if len(data) < 7:
            return None
        version, code, payload_size = struct.unpack('<BHI', data[:7])
        if len(data) < 7 + payload_size:
            return None
        payload = data[7:7+payload_size]
        return {
            'version': version,
            'code': code,
            'payload_size': payload_size,
            'payload': payload,
        }

    @staticmethod
    def build_response(version, code, payload):
        payload_size = len(payload)
        header = struct.pack('<BHI', 1, code, payload_size)
        return header + payload
