import mysql.connector
import getpass

def check_connection(host, user, database, port=3306):
    try:
        password = getpass.getpass(f"Password for {user}@{host}: ")
        
        print(f"Connecting to {host}...")
        conn = mysql.connector.connect(
            host=host,
            user=user,
            password=password,
            database=database,
            port=port,
            use_pure=True
        )
        cursor = conn.cursor()
        
        # Simple query to verify access
        query = "SELECT * FROM coca_cola_prices"
        print(f"Executing: {query}")
        cursor.execute(query)
        rows = cursor.fetchall()
        
        print(f"Success! Number of rows in table: {len(rows)}")
        print(f"First 5 rows: \n {rows[:5]}\n") # rajoute des \n entre les valeurs
        
    except mysql.connector.Error as err:
        print(f"\n[MySQL Error] {err}")
    except Exception as e:
        print(f"\n[General Error] {e}")
    finally:
        if 'conn' in locals() and conn.is_connected():
            cursor.close()
            conn.close()
            print("Connection closed.")

if __name__ == "__main__":
    HOST = "mysql-guemoune4.alwaysdata.net"
    USER = "guemoune4"
    DATABASE = "guemoune4_euronext"
    
    check_connection(HOST, USER, DATABASE)
