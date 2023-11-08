# GeoPoint Canvas 🗺️
<div style="text-align:center">
  <img src="logo.png" alt="Logo" width="300" height="300" />
</div>

## Introduction 
GeoPoint Canvas is a Python program that helps you visualize the real-world locations of IP addresses on an interactive map.

## Features
### 1. Extract Unique IP Addresses: 
Find and keep unique IP addresses from your log or text file.

### 2. Geolocate IPs: 
Pinpoint the approximate locations (latitude and longitude) of IP addresses, making it easy to see where they are in the world.

### 3. Create Interactive Map
Generate a map with markers for each IP address, so you can explore their locations visually in a html file.

## Getting Started
### CLI
#### 1. Install required dependencies
```python
pip install -r requirements.txt
```
#### 2. Prepare your IP addresses in a text file (e.g., 'logs.txt') and put it under current directory.
#### 3. Run the program.
```python
python geolocateip.py input_file_name [output_file_name]
```
For Example: 
```python
python geolocateip.py log.txt location.html
```
- The input file should be a text file 
- The output file will be a HTML file
#### 4. That's it!

## Built With
<a href="https://www.python.org" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/python/python-original.svg" alt="python" width="40" height="40"/></a>

## TODO


## License

This project is licensed under the [MIT License](LICENSE.md).
