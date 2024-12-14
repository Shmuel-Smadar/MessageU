import uuid
import time
import struct

from constants import ProtocolLengths
from constants import ClientCodes
from constants import ServerCodes

class ResponseBuilder:
    
    def build_response(self, code, payload):
        payload_size = len(payload)
        header = struct.pack('<BHI', ServerCodes.VERSION, code, payload_size)
        return header + payload