# easily-open-server

A simple command-line program for opening any type of server.

## Features

- Launch different types of servers from a single CLI command
- Easy-to-use syntax: `./open [server-type] [port] [options...]`
- Customizable options like max clients, buffer size, and timeout
- Modular design for adding new server types

## command
<b>open [server-type] [port] [options]</b>
* Available Options
| Option | Type | Description | Example |
|--------|--------|--------|--------|
| --max-client=n   | Integer | Maximum number of clients that can connect to the server | --max-client=10
| --buffer-size=n  | Integer | Buffer size for receiving messages (in bytes)            | --buffer-size=2048
| --max-req-size=  | Integer | (HTTP) Maximum allowed size of a request (in bytes)	    | --max-req-size=8192
| --log=true/false | Boolean | Enable or disable server-side logging	                | --log=true
| --timeout=n      | Integer | Timeout for inactive connections (in seconds)	        | --timeout=60


