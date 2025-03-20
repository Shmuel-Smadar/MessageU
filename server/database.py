# filename: database.py
# this file manages all database operations

import sqlite3
from typing import List, Optional
from classes import Client, Message

DATABASE_FILE = 'defensive.db'

class Database:
    def __init__(self):
        self.conn = sqlite3.connect(DATABASE_FILE)
        self.conn.row_factory = sqlite3.Row
        self.create_tables()

    # a function that creates the tables for the clients and the messages if they dont exist yet.
    def create_tables(self):
        with self.conn:
            self.conn.execute('''
                CREATE TABLE IF NOT EXISTS clients (
                    ID TEXT PRIMARY KEY,
                    UserName TEXT UNIQUE NOT NULL,
                    PublicKey BLOB NOT NULL,
                    LastSeen TIMESTAMP NOT NULL
                )
            ''')
            self.conn.execute('''
            CREATE TABLE IF NOT EXISTS messages (
                ID INTEGER PRIMARY KEY,
                ToClient TEXT NOT NULL,
                FromClient TEXT NOT NULL,
                Type INTEGER NOT NULL,
                Content BLOB NOT NULL,
                FOREIGN KEY (ToClient) REFERENCES clients(ID),
                FOREIGN KEY (FromClient) REFERENCES clients(ID)
            )
        ''')
    # a function that checks for an existance of a client.
    def client_exists(self, username: str) -> bool:
        with self.conn:
            result = self.conn.execute(
                'SELECT 1 FROM clients WHERE UserName = ?', (username,)
            ).fetchone()
        return result is not None
    # a function that adds a client
    def add_client(self, client: Client):
        with self.conn:
            self.conn.execute('''
                INSERT INTO clients (ID, UserName, PublicKey, LastSeen)
                VALUES (?, ?, ?, ?)
            ''', (client.ID, client.UserName, client.PublicKey, client.LastSeen))
    # a function that returns all the clients except for one that can be excluded.
    def get_clients(self, exclude_id: Optional[str] = None) -> List[Client]:
        query = 'SELECT * FROM clients'
        params = ()
        if exclude_id:
            query += ' WHERE ID != ?'
            params = (exclude_id,)
        with self.conn:
            rows = self.conn.execute(query, params).fetchall()
        return [Client(**row) for row in rows]
    # a function that returns a client by its Id
    def get_client_by_id(self, client_id: str) -> Client:
        with self.conn:
            row = self.conn.execute(
                'SELECT * FROM clients WHERE ID = ?', (client_id,)
            ).fetchone()
        return Client(**row) if row else None
    # a function that returns a client by its username
    def get_client_by_username(self, username: str) -> Optional[Client]:
        with self.conn:
            row = self.conn.execute(
                'SELECT * FROM clients WHERE UserName = ?', (username,)
            ).fetchone()
        return Client(**row) if row else None

    def update_last_seen(self, client_id: str):
        with self.conn:
            self.conn.execute('''
                UPDATE clients
                SET LastSeen = CURRENT_TIMESTAMP
                WHERE ID = ?
            ''', (client_id,))

    def add_message(self, message: Message):
        with self.conn:
            self.conn.execute('''
                INSERT INTO messages (ID, ToClient, FromClient, Type, Content)
                VALUES (?, ?, ?, ?, ?)
            ''', (message.ID, message.ToClient, message.FromClient, message.Type, message.Content))

    def get_messages_for_client(self, client_id: str) -> List[Message]:
        with self.conn:
            rows = self.conn.execute(
                'SELECT * FROM messages WHERE ToClient = ? ORDER BY Type ASC', (client_id,)
            ).fetchall()
        return [Message(**row) for row in rows]

    def delete_message(self, message_id: int):
        with self.conn:
            self.conn.execute('DELETE FROM messages WHERE ID = ?', (message_id,))

    def delete_messages_for_client(self, client_id: str):
        with self.conn:
            self.conn.execute('DELETE FROM messages WHERE ToClient = ?', (client_id,))

    def close(self):
        self.conn.close()
