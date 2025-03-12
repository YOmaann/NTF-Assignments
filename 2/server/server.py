# Creating a server in Python for simplicity

from http.server import HTTPServer, BaseHTTPRequestHandler
from argparse import ArgumentParser
import os
import mimetypes

class server(BaseHTTPRequestHandler):
    def do_GET(self):
        __location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))

        if self.path == "/":
            self.path = "/index.html"
        print(os.path.join(__location__, "www", self.path[1:]))


        try:
            to_serve_loc = os.path.join(__location__, "www", self.path[1:])
            print(to_serve_loc)
            to_serve = open(to_serve_loc, 'rb').read()
            
            mimetype = mimetypes.MimeTypes().guess_type(to_serve_loc)
            self.send_response(200)

            if mimetype[0] != 'text/html':
                self.send_header('Content-type', mimetype)

        except Exception as e:
            print(e)
            to_serve_loc = os.path.join(__location__, "www", "404.html")
            to_serve = open(to_serve_loc, "rb").read()
                
            self.send_response(404)
        content_len = len(to_serve)
        self.send_header('Content-Length', str(content_len))
        self.end_headers()
        self.wfile.write(to_serve)

def main(hostname, port):
    print(f"Starting server at port {port}...") 
    httpd = HTTPServer((hostname, port), server)
    httpd.serve_forever()
    
    
if __name__ == "__main__":
    parser = ArgumentParser(description = "Lucky's Server")
    parser.add_argument('--port', nargs = "?", const = 8080, default = 8080, type = int)
    parser.add_argument('--hostname', nargs = "?", const = "localhost", default = "localhost")
    args = parser.parse_args()
    main(args.hostname, args.port)
