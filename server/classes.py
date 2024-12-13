
import time

class Client:
    def __init__(self, ID, UserName, PublicKey, LastSeen=None):
        self.ID = ID
        self.UserName = UserName
        self.PublicKey = PublicKey
        self.LastSeen = LastSeen or time.time()
