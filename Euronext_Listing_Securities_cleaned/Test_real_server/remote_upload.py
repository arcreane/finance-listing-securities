import mysql.connector
import getpass
import os
import traceback
import sys

def upload_sql_file(host, user, database, sql_file, port=3306):
    try:
        password = getpass.getpass(f"Password for {user}@{host}: ")
        
        print(f"Connecting to {host}...")
        conn = mysql.connector.connect(
            host=host,
            user=user,
            password=password,
            database=database,
            port=port,
            use_pure=True  # Force pure Python implementation to avoid C extension errors
        )
        cursor = conn.cursor()
        
        print(f"Reading {sql_file}...")
        with open(sql_file, 'r', encoding='utf-8') as f:
            sql_queries = f.read()
            
        # Split by semicolon to execute separate statements
        # Note: This is a simple split. Complex SQL might need a better parser.
        statements = sql_queries.split(';')
        
        print(f"Executing statements...")
        for i, statement in enumerate(statements):
            if statement.strip():
                try:
                    cursor.execute(statement)
                except mysql.connector.Error as err:
                    print(f"Error in statement {i}: {err}")
        
        conn.commit()
        print("Upload successful!")
        
    except mysql.connector.Error as err:
        print(f"\n[MySQL Error] {err}")
        print(f"Error Code: {err.errno}")
        print(f"SQL State: {err.sqlstate}")
        print(f"Message: {err.msg}")
    except Exception as e:
        print(f"\n[General Error] {type(e).__name__}: {e}")
        print("-" * 60)
        traceback.print_exc(file=sys.stdout)
        print("-" * 60)
    finally:
        if 'conn' in locals() and conn.is_connected():
            cursor.close()
            conn.close()
            print("Connection closed.")

if __name__ == "__main__":
    # Parameters from user's attempted command
    HOST = "mysql-guemoune4.alwaysdata.net"
    USER = "guemoune4"
    DATABASE = "guemoune4_euronext" 
    SQL_FILE = "coca_cola_data.sql"
    PORT = 3306
    
    # Prompt for database name fix if needed
    print(f"Targeting Database: {DATABASE}")
    db_choice = input(f"Is '{DATABASE}' correct? (Y/n): ").strip().lower()
    if db_choice == 'n':
        DATABASE = input("Please enter the correct database name: ").strip()

    if os.path.exists(SQL_FILE):
        upload_sql_file(HOST, USER, DATABASE, SQL_FILE, PORT)
    else:
        print(f"Error: {SQL_FILE} not found.")
