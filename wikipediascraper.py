import requests
from bs4 import BeautifulSoup

# URL of the Wikipedia page
url = "https://en.wikipedia.org/wiki/Wikipedia:Popular_pages"

# Send a GET request to the page
response = requests.get(url)
response.raise_for_status()  # Raise an exception for HTTP errors

# Parse the HTML content using BeautifulSoup
soup = BeautifulSoup(response.text, 'html.parser')

# Initialize a list to store the top 100 pages
top_pages = []

# Find all tables in the page
tables = soup.find_all('table', {'class': 'wikitable'})

# Iterate through tables to find the one containing the top 100 list
for table in tables:
    rows = table.find_all('tr')
    for row in rows[1:]:  # Skip the header row
        cols = row.find_all(['td', 'th'])
        if len(cols) >= 2:
            rank = cols[0].get_text(strip=True)
            title = cols[1].get_text(strip=True)
            link_tag = cols[1].find('a')
            link = f"https://en.wikipedia.org{link_tag['href']}" if link_tag else None
            top_pages.append({'rank': rank, 'title': title, 'link': link})
        if len(top_pages) >= 100:
            break
    if len(top_pages) >= 100:
        break

# Output the results
for page in top_pages:
    print(page['link'])
