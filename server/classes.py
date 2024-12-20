
import time

class Client:
    def __init__(self, ID, UserName, PublicKey, LastSeen=None):
        self.ID = ID
        self.UserName = UserName
        self.PublicKey = PublicKey
        self.LastSeen = LastSeen or time.time()
class Message:
    def __init__(self, ID, ToClient, FromClient, Type, Content):
        self.ID = ID
        self.ToClient = ToClient
        self.FromClient = FromClient
        self.Type = Type
        self.Content = Content
