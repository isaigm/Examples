import requests
import bs4

url = "https://gpsd.gitlab.io/gpsd/AIVDM.html"
page = requests.get(url)
soup = bs4.BeautifulSoup(page.content)
tables = soup.find_all(class_="sect2")
mode = True
for div in tables:
    if len(div.find_all(id="_type_6_binary_addressed_message"))  > 0:
        rows = div.find_all("tr")
        for r in rows:
            tds = r.find_all('td')
            if len(tds) == 6:
                bits = tds[1].find(class_="tableblock")
                name =  tds[3].find(class_="tableblock")
                type_ = tds[4].find(class_="tableblock")
                if bits != None and name != None and type_ != None:
                    bits = bits.text
                    name = name.text
                    type_ = type_.text
                    if name == 'type':
                        print("----------------")
                    if mode:
                        line = "private "
                        if type_ == 't':
                            line += "String"
                        elif 'U' in type_ or 'I' in type_:
                            line += "float"
                        else:
                            line += "int"
                        line += ' ' + name + ";"
                
                    else:
                        line = name + " = payload.getNextNbits(" + bits + ")."
                        if type_ == 't':
                            line += "toSixBitAscii();"
                        elif 'I' in type_:
                            line += "toSignedInt();"
                        else:
                            line += "toInteger();"
             
                    print(line)
