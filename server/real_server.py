import os
import time
import socket
from flask import Flask, request, jsonify
from flask_cors import CORS

# Optional DB import
try:
    import mysql.connector
except ImportError:
    mysql = None

app = Flask(__name__)
CORS(app)

# --- AlwaysData production: always use "real" environment ---
CURRENT_ENV = "real"

ENV_CONFIG = {
    "local": {
        "name": "LOCAL SERVER",
        "db": None,
        "proof": "IDENTIFIER: [LOCAL_VIRTUAL_EMULATOR]",
        "color": "blue"
    },
    "real": {
        "name": "REAL SERVER (AlwaysData)",
        "db": {
            "host": "mysql-guemoune4.alwaysdata.net",
            "user": "guemoune4",
            "database": "guemoune4_euronext",
            "port": 3306
        },
        "proof": "IDENTIFIER: [ALWAYS_DATA_SECURE_REMOTE_v1.2]",
        "color": "green"
    }
}


def get_db_connection():
    config = ENV_CONFIG[CURRENT_ENV]["db"]
    if not config or not mysql:
        return None

    try:
        password = os.environ.get("DB_PASSWORD", "")
        conn = mysql.connector.connect(
            host=config["host"],
            user=config["user"],
            password=password,
            database=config["database"],
            port=config["port"],
            use_pure=True
        )
        return conn
    except Exception as e:
        print(f"DB Error: {e}")
        return None


@app.route('/status', methods=['GET'])
@app.route('/', methods=['GET'])
def status():
    db_ok = False
    conn = get_db_connection()
    if conn:
        db_ok = True
        conn.close()

    return jsonify({
        "status": "online",
        "env": CURRENT_ENV,
        "proof_of_identity": ENV_CONFIG[CURRENT_ENV]["proof"],
        "server_time": time.ctime(),
        "db_connected": db_ok
    })


@app.route('/ping', methods=['GET'])
def ping():
    cfg = ENV_CONFIG[CURRENT_ENV]
    server_host = cfg["db"]["host"] if cfg["db"] else "127.0.0.1"
    return jsonify({
        "status": "pong",
        "env": CURRENT_ENV,
        "server_host": server_host,
        "timestamp": time.time()
    })


@app.route('/speedtest/download', methods=['GET'])
def speedtest_download():
    # 1 MB payload
    payload = b'X' * (1024 * 1024)
    return payload, 200, {'Content-Type': 'application/octet-stream'}


@app.route('/speedtest/upload', methods=['POST'])
def speedtest_upload():
    data = request.get_data()
    return jsonify({
        "ok": True,
        "bytes_received": len(data),
        "env": CURRENT_ENV
    })


# --- WSGI entry point ---
# AlwaysData uses: real_server:app
# No __main__ block needed in production.
