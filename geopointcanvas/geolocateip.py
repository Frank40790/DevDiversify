import re
import argparse
import webbrowser
import folium
from datetime import datetime
import requests

class GeopointCanvas:
    def __init__(self):
        pass
    def int_to_ip(integer):
        octet1 = (integer >> 24) & 0xFF
        octet2 = (integer >> 16) & 0xFF
        octet3 = (integer >> 8) & 0xFF
        octet4 = integer & 0xFF
        return f"{octet1}.{octet2}.{octet3}.{octet4}"

    def extract_unique_ips_from_file(self, file_path):
        with open(file_path, 'r') as file:
            text = file.read()
            ip_pattern = r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}'
            ips = set(re.findall(ip_pattern, text))
        return ips

    def create_html_map(self, ip_info, output_map_path):
        m = folium.Map(location=[0, 0], zoom_start=2)
        for info in ip_info:
            lat, lon = info['latitude'], info['longitude']
            try:
                lat = float(lat)
                lon = float(lon)
                info=f"IP: {info['ip']}<br>City: {info['city']}<br>Region: {info['region']}<br>Region Code: {info['region_code']}<br>Country Code: {info['country_code']}<br>Country Code ISO3: {info['country_code_iso3']}<br>Country Name: {info['country_name']}<br>Country Capital: {info['country_capital']}<br>Country TLD: {info['country_tld']}<br>Continent Code: {info['continent_code']}<br>In EU: {info['in_eu']}<br>Postal: {info['postal']}<br>Latitude: {info['latitude']}<br>Longitude: {info['longitude']}<br>Timezone: {info['timezone']}<br>UTC offset: {info['utc_offset']}<br>Country Calling Code: {info['country_calling_code']}<br>Currency: {info['currency']}<br>Currency Name: {info['currency_name']}<br>Languages: {info['languages']}<br>ASN: {info['asn']}<br>ISP: {info['org']}<br>"                
                marker = folium.Marker(location=[lat, lon])
                popup = folium.Popup(info, max_width=300)
                marker.add_child(popup)
                marker.add_to(m)

            except (TypeError, ValueError):
                print(f"Skipping marker for IP {info['ip']} due to invalid latitude or longitude. {lat},{lon}")
        m.save(output_map_path)

    def geolocate_and_generate_map(self, input_file, output_map=None):
        unique_ips = self.extract_unique_ips_from_file(input_file)
        ip_info = self.batch_get_location(unique_ips)

        if output_map is None:
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            output_map = f"{timestamp}.html"

        self.create_html_map(ip_info, output_map)

        print(f"Unique IP addresses and their geolocation information have been marked on the map. Map saved as {output_map}.")

        webbrowser.open(output_map)

    def batch_get_location(self, ips):
        location_data = []
        for ip in ips:
            location_data.append(self.get_location(ip))
        return location_data
    def get_location(self, ip):
        response = requests.get(f'https://ipapi.co/{ip}/json/').json()
        location_data = {
            "ip": response.get("ip"),
            "city": response.get("city"),
            "region": response.get("region"),
            "region_code": response.get("region_code"),
            "country_code": response.get("country_code"),
            "country_code_iso3": response.get("country_code_iso3"),
            "country_name": response.get("country_name"),
            "country_capital": response.get("country_capital"),
            "country_tld": response.get("country_tld"),
            "continent_code": response.get("continent_code"),
            "in_eu": response.get("in_eu"),
            "postal": response.get("postal"),
            "latitude": response.get("latitude"),
            "longitude": response.get("longitude"),
            "timezone": response.get("timezone"),
            "utc_offset": response.get("utc_offset"),
            "country_calling_code": response.get("country_calling_code"),
            "currency": response.get("currency"),
            "currency_name": response.get("currency_name"),
            "languages": response.get("languages"),
            "asn": response.get("asn"),
            "org": response.get("org")
        }


        return location_data

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Geolocate IP addresses and generate an HTML map.")
    parser.add_argument("input_file", type=str, help="Path to the input file containing IP addresses.")
    parser.add_argument("output_map", type=str, nargs='?', default=None, help="Path to the output HTML map (optional). Defaults to a timestamp-based filename if not specified.")
    args = parser.parse_args()

    ip_geolocator = GeopointCanvas()
    ip_geolocator.geolocate_and_generate_map(args.input_file, args.output_map)




