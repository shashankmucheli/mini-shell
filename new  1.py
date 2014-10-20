import urllib
import BeautifulSoup as soup
import urlparse
import mechanize
import os

url = "http://www.faculty.umassd.edu/hong.liu/ece565.html"

soup = mechanize.Browser()
URI = [url]
#soup.open(url)
visited= []
while URI:
    try:
        soup.open(url)
        URI.pop(0)
        for link in soup.links():
            newurl = urlparse.urljoin(link.base_url,link.url)
            hst = urlparse.urlparse(newurl).hostname
            pth = urlparse.urlparse(newurl).path
            print "http://"+hst+pth
            
            if newurl not in visited and urlparse.urlparse(url).hostname in newurl:
                visited.append(newurl)
                URI.append(newurl)
                print newurl
                os.system("wget -i %s -x" %newurl)
        
    except:
        URI.pop(0)
        print "----------------------"