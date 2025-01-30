# WebServ

⚠️ **NOTE**: This project is currently under active development

WebServ is a lightweight, high-performance web server designed for flexibility and scalability. This project is currently in progress, with a focus on handling HTTP requests efficiently while maintaining ease of configuration.

## Features (Planned)
- **HTTP 1.1 Support**: Handle GET, POST, and other standard HTTP methods.
- **Static & Dynamic Content Serving**: Serve HTML, CSS, JS, and execute CGI scripts.
- **Configurable Routing**: Support for custom routes and middleware.
- **Logging & Monitoring**: Basic request logging and performance tracking.
- **Security Features**: Basic authentication, CORS handling, and request validation.

## Installation
```sh
# Clone the repository
git clone https://github.com/ismailassil/webserv.git
cd webserv

# Build the project
make
```

## Usage
```sh
# Run the server
./webserv [config_file]
```
By default, the server will look for a configuration file (e.g., `webserv.conf`) to set up routes and behavior.

## Configuration
WebServ will use a configuration file format similar to Nginx. Example:
```nginx
server {
    listen 8080;
    root ./public;
    location /api {
        fastcgi_pass unix:/run/fcgiwrap.socket;
    }
}
```
