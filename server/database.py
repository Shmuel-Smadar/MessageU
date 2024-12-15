import sqlite3
from typing import List, Optional
from classes import Client

DATABASE_FILE = 'MessageU-server.db'

class Database:
    def __init__(self):
        self.conn = sqlite3.connect(DATABASE_FILE)
        self.conn.row_factory = sqlite3.Row
        self.create_tables()

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

    def client_exists(self, username: str) -> bool:
        with self.conn:
            result = self.conn.execute(
                'SELECT 1 FROM clients WHERE UserName = ?', (username,)
            ).fetchone()
        return result is not None

    def add_client(self, client: Client):
        with self.conn:
            self.conn.execute('''
                INSERT INTO clients (ID, UserName, PublicKey, LastSeen)
                VALUES (?, ?, ?, ?)
            ''', (client.ID, client.UserName, client.PublicKey, client.LastSeen))

    def get_clients(self, exclude_id: Optional[str] = None) -> List[Client]:
        query = 'SELECT * FROM clients'
        params = ()
        if exclude_id:
            query += ' WHERE ID != ?'
            params = (exclude_id,)
        with self.conn:
            rows = self.conn.execute(query, params).fetchall()
        return [Client(**row) for row in rows]

    def print_clients_table(self):
        with self.conn:
            cursor = self.conn.execute('SELECT * FROM clients')
            rows = cursor.fetchall()
            columns = [description[0] for description in cursor.description]
            print("\t".join(columns))
            print("-" * 40)
            for row in rows:
                print("\t".join(str(item) for item in row))
    
    def get_client_by_id(self, client_id: str) -> Client:
        print(f"Looking for client with ID: {client_id}")
        with self.conn:
            row = self.conn.execute(
                'SELECT * FROM clients WHERE ID = ?', (client_id,)
            ).fetchone()
        return Client(**row) if row else None

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

    def close(self):
        self.conn.close()
