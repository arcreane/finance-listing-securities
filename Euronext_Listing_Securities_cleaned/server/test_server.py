"""
Unit tests for the Network User backend server.
Run with: pytest test_server.py -v
"""

import pytest
import time
from server import app, DOWNLOAD_SIZE


@pytest.fixture
def client():
    """Create a test client for the Flask app."""
    app.config['TESTING'] = True
    with app.test_client() as client:
        yield client


class TestPingEndpoint:
    """Tests for the /ping endpoint."""

    def test_ping_returns_200(self, client):
        """Ping endpoint should return HTTP 200."""
        response = client.get('/ping')
        assert response.status_code == 200

    def test_ping_returns_json(self, client):
        """Ping endpoint should return JSON."""
        response = client.get('/ping')
        assert response.content_type == 'application/json'

    def test_ping_contains_pong_message(self, client):
        """Ping response should contain 'pong' message."""
        response = client.get('/ping')
        data = response.get_json()
        assert data['message'] == 'pong'
        assert data['status'] == 'pong'

    def test_ping_contains_timestamp(self, client):
        """Ping response should contain a valid timestamp."""
        response = client.get('/ping')
        data = response.get_json()
        assert 'timestamp' in data
        assert isinstance(data['timestamp'], float)
        # Timestamp should be recent (within last 10 seconds)
        assert abs(data['timestamp'] - time.time()) < 10


class TestDownloadSpeedEndpoint:
    """Tests for the /speedtest/download endpoint."""

    def test_download_returns_200(self, client):
        """Download endpoint should return HTTP 200."""
        response = client.get('/speedtest/download')
        assert response.status_code == 200

    def test_download_returns_binary(self, client):
        """Download endpoint should return binary data."""
        response = client.get('/speedtest/download')
        assert response.content_type == 'application/octet-stream'

    def test_download_returns_correct_size(self, client):
        """Download endpoint should return exactly DOWNLOAD_SIZE bytes."""
        response = client.get('/speedtest/download')
        assert len(response.data) == DOWNLOAD_SIZE

    def test_download_payload_content(self, client):
        """Download payload should be filled with 'X' characters."""
        response = client.get('/speedtest/download')
        # Check first and last bytes
        assert response.data[0:1] == b'X'
        assert response.data[-1:] == b'X'


class TestUploadSpeedEndpoint:
    """Tests for the /speedtest/upload endpoint."""

    def test_upload_returns_200(self, client):
        """Upload endpoint should return HTTP 200."""
        response = client.post('/speedtest/upload', data=b'test data')
        assert response.status_code == 200

    def test_upload_returns_json(self, client):
        """Upload endpoint should return JSON."""
        response = client.post('/speedtest/upload', data=b'test data')
        assert response.content_type == 'application/json'

    def test_upload_returns_ok(self, client):
        """Upload response should indicate success."""
        response = client.post('/speedtest/upload', data=b'test data')
        data = response.get_json()
        assert data['ok'] is True

    def test_upload_counts_bytes(self, client):
        """Upload response should report correct byte count."""
        test_data = b'X' * 1000
        response = client.post('/speedtest/upload', data=test_data)
        data = response.get_json()
        assert data['bytes_received'] == 1000

    def test_upload_empty_data(self, client):
        """Upload should handle empty data."""
        response = client.post('/speedtest/upload', data=b'')
        data = response.get_json()
        assert data['ok'] is True
        assert data['bytes_received'] == 0

    def test_upload_large_data(self, client):
        """Upload should handle large payloads (1 MB)."""
        large_data = b'X' * (1024 * 1024)
        response = client.post('/speedtest/upload', data=large_data)
        data = response.get_json()
        assert data['ok'] is True
        assert data['bytes_received'] == 1024 * 1024


class TestRootEndpoint:
    """Tests for the / (root) endpoint."""

    def test_root_returns_200(self, client):
        """Root endpoint should return HTTP 200."""
        response = client.get('/')
        assert response.status_code == 200

    def test_root_returns_server_info(self, client):
        """Root endpoint should return server information."""
        response = client.get('/')
        data = response.get_json()
        assert data['name'] == 'Network User Backend'
        assert data['status'] == 'running'
        assert 'version' in data

    def test_root_lists_endpoints(self, client):
        """Root endpoint should list available endpoints."""
        response = client.get('/')
        data = response.get_json()
        assert 'endpoints' in data
        assert '/ping' in data['endpoints']
        assert '/speedtest/download' in data['endpoints']
        assert '/speedtest/upload' in data['endpoints']


class TestCORS:
    """Tests for CORS headers."""

    def test_cors_headers_present(self, client):
        """Response should include CORS headers."""
        response = client.get('/ping')
        assert 'Access-Control-Allow-Origin' in response.headers


class TestErrorHandling:
    """Tests for error handling."""

    def test_404_on_unknown_endpoint(self, client):
        """Unknown endpoints should return 404."""
        response = client.get('/nonexistent')
        assert response.status_code == 404

    def test_method_not_allowed_download(self, client):
        """POST to download endpoint should fail."""
        response = client.post('/speedtest/download')
        assert response.status_code == 405

    def test_method_not_allowed_upload(self, client):
        """GET to upload endpoint should fail."""
        response = client.get('/speedtest/upload')
        assert response.status_code == 405


if __name__ == '__main__':
    pytest.main([__file__, '-v'])
