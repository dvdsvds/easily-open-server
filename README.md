# ! A project under development

# easily-open-server

A simple command-line program for opening any type of server.

## Features
- Launch different types of servers from a single CLI command
- Easy-to-use syntax
  - **Linux**
    ```bash
    git clone https://github.com/dvdsvds/easily-open-server.git
    cd easily-open-server
    mkdir build
    cmake . && make
    ./open
  - **Windows**
    - Using open.exe
      Simply double-click open.exe to launch the server with default settings or configuration file.
    - Using Command Line(CMD or PowerShell)
      ```PowerShell
      .\open
     
- Customizable options like max clients, buffer size, and timeout
- Modular design for adding new server types

## Options
 - Server performance varies depending on the configured option values
 - [Option Guide](https://dvdsvds.github.io/easily-open-server/options.html)

  ### Server Type
    - All server types are specified using abbreviations<br>
    - All server settings are reset when the server shuts down<br>
    - [Server Type Guide](https://dvdsvds.github.io/easily-open-server/server_type.html)




