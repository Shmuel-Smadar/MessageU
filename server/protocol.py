# filename: protocol.py
# this file is responsible for handling the flow that requests from the client will go through.

import struct
from constants import ProtocolByteSizes, ClientCodes, SupportedClientVersion
from request_parser import RequestParser
from database import Database

class Protocol:
    def __init__(self):
        self.request_parser = RequestParser()

    def process_requests(self, data, db: Database):
        request = data['request']
        try:
            header = self.extract_header(request)
            self.validate_client(header['client_id'], header['version'], header['code'], db)
            response = self.get_response(header['client_id'], header['code'], header['payload'], db)
        except Exception as e:
            # in case of any error during parsing, return an invalid request response.
            response = self.request_parser.invalid_request()

        data['response'] += response
        data['request'] = data['request'][len(request):]

    # extracts and returns the header fields from the given request.
    def extract_header(self, request: bytes) -> dict:
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

        payload = request[ProtocolByteSizes.HEADER:total_size]
        return {
            'client_id': client_id.hex(),
            'version': version,
            'code': code,
            'payload_size': payload_size,
            'payload': payload
        }
    
    # validates that the client version is supported and the client is registered.
    def validate_client(self, client_id: bytes, version: int, code: int, db: Database):
        if version not in SupportedClientVersion.versions:
            raise ValueError("Unsupported client version.")
        if db.get_client_by_id(client_id) is None and code != ClientCodes.REGISTRATION:
            raise ValueError("Unregistered client.")

    def get_response(self, client_id, code: int, payload: bytes, db: Database) -> bytes:

        if code == ClientCodes.REGISTRATION:
            return self.request_parser.registration(payload, db)
        elif code == ClientCodes.CLIENT_LIST:
            return self.request_parser.client_list(client_id, db)
        elif code == ClientCodes.PUBLIC_KEY:
            return self.request_parser.public_key(client_id, payload, db)
        elif code == ClientCodes.MESSAGE_SENT:
            return self.request_parser.message_sent(client_id, payload, db)
        elif code == ClientCodes.AWAITING_MESSAGES:
            return self.request_parser.awaiting_messages(client_id, db)
        else:
            return self.request_parser.invalid_request()