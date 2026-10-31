# Static file server that sends the cross-origin isolation headers
# required for SharedArrayBuffer (Emscripten pthreads).
#
# Run from the folder that contains index.html and build/:
#     python3 serve.py            (serves on port 8000)
#     python3 serve.py 9000       (custom port)
# Then open http://localhost:8000/

import sys
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer

class Handler(SimpleHTTPRequestHandler):
    extensions_map = {
        **SimpleHTTPRequestHandler.extensions_map,
        ".wasm": "application/wasm",
        ".js": "text/javascript",
    }

    def end_headers(self):
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        self.send_header("Cache-Control", "no-store")  # avoid stale .data/.wasm while developing
        super().end_headers()


if __name__ == "__main__":
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 8000
    print(f"Serving on http://localhost:{port}/")
    ThreadingHTTPServer(("", port), Handler).serve_forever()
