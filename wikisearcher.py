import sys
import requests

def get_wikipedia_url(title):
    # Encode spaces as underscores (Wikipedia's URL format)
    formatted_title = title.strip().replace(' ', '_')

    # Use the Wikipedia API to check if the page exists
    url = "https://en.wikipedia.org/w/api.php"
    params = {
        "action": "query",
        "titles": formatted_title,
        "format": "json"
    }

    response = requests.get(url, params=params)
    data = response.json()

    pages = data.get("query", {}).get("pages", {})
    page_id = next(iter(pages))

    if page_id != "-1":
        return f"https://en.wikipedia.org/wiki/{formatted_title}"
    else:
        return None

if __name__ == "__main__":
    file = sys.argv[1]
    with open(file) as f:
        for title in f:
            title = title.strip()
            title = ' '.join(title.rsplit(' ', 1)[0:1])
            url = get_wikipedia_url(title)
            if url:
                print(url)

