"""
Network User - Backend Server 2
A simple Flask server providing endpoints for the Network User Qt application.
Running on port 8081 as the secondary server.
"""

from flask import Flask, request, jsonify
from flask_cors import CORS
import time

app = Flask(__name__)
CORS(app)  # Allow cross-origin requests from the Qt app

# =============================================================================
# PING ENDPOINT
# =============================================================================
@app.route('/ping', methods=['GET'])
def ping():
    """
    Simple ping endpoint to measure latency.
    Returns a JSON response with status 'pong'.
    """
    return jsonify({
        "status": "pong",
        "message": "pong",
        "timestamp": time.time()
    })


# Download test payload size (in bytes)
DOWNLOAD_SIZE = 1 * 1024 * 1024  # 1 MB

@app.route('/speedtest/download', methods=['GET'])
def speedtest_download():
    """
    Download speed test endpoint.
    Returns a large payload (1 MB by default) for the client to download.
    The client measures how long it takes to receive this data.
    """
    # Generate payload of specified size
    payload = b'X' * DOWNLOAD_SIZE
    return payload, 200, {'Content-Type': 'application/octet-stream'}


@app.route('/speedtest/upload', methods=['POST'])
def speedtest_upload():
    """
    Upload speed test endpoint.
    Accepts data uploaded by the client and returns success.
    The client measures how long it takes to upload the data.
    """
    # Read the uploaded data (we don't need to do anything with it)
    data = request.get_data()
    bytes_received = len(data)

    return jsonify({
        "ok": True,
        "bytes_received": bytes_received,
        "message": f"Received {bytes_received} bytes"
    })


# =============================================================================
# SERVER STATUS
# =============================================================================
@app.route('/', methods=['GET'])
def index():
    """
    Root endpoint - returns server status information.
    """
    return jsonify({
        "name": "Network User Backend 2",
        "version": "1.0.0",
        "status": "running",
        "endpoints": {
            "/ping": "GET - Latency test",
            "/speedtest/download": "GET - Download speed test (1 MB payload)",
            "/speedtest/upload": "POST - Upload speed test"
        }
    })


# =============================================================================
# MAIN
# =============================================================================
if __name__ == '__main__':
    print("=" * 60)
    print("  Network User Backend Server 2")
    print("=" * 60)
    print()
    print("  Endpoints:")
    print("    GET  /              - Server status")
    print("    GET  /ping          - Latency test")
    print("    GET  /speedtest/download - Download speed test")
    print("    POST /speedtest/upload   - Upload speed test")
    print()
    print("  Starting server on http://127.0.0.1:8081")
    print("=" * 60)

    app.run(host='127.0.0.1', port=8081, debug=True)
