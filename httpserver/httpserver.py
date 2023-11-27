import os
import argparse
from flask import Flask, render_template, send_from_directory

app = Flask(__name__)

def parse_arguments():
    parser = argparse.ArgumentParser(description="Simple Flask server for sharing files.")
    parser.add_argument('--host', default="0.0.0.0", help="Host IP address")
    parser.add_argument('--port', type=int, default=1234, help="Port number")
    parser.add_argument('--folder', default='./share', help="Path to the shared folder")
    return parser.parse_args()

def create_app(shared_folder):
    app = Flask(__name__)

    @app.route('/share/<filename>')
    def share_file(filename):
        return send_from_directory(shared_folder, filename)

    @app.route('/')
    def index():
        shared_files = os.listdir(shared_folder)
        return render_template('httpserver.html', shared_files=shared_files)

    return app

if __name__ == '__main__':
    args = parse_arguments()
    shared_folder = args.folder

    if not os.path.exists(shared_folder):
        print(f"Error: The specified shared folder '{shared_folder}' does not exist.")
    else:
        app = create_app(shared_folder)
        app.run(host=args.host, port=args.port)
