import requests
import bs4



url = "https://gpsd.gitlab.io/gpsd/AIVDM.html"
page = requests.get(url)
soup = bs4.BeautifulSoup(page.content)
tables = soup.find_all(class_="sect2")
for div in tables:
    if len(div.find_all(id="_type_8_binary_broadcast_message"))  > 0:
        rows = div.find_all("tr")
        for r in rows:
            tds = r.find_all('td')
            if len(tds) == 6:
                print(tds[0])
                print(tds[1])
