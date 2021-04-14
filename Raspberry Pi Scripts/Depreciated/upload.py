#!/usr/bin/env python3

import requests

filelist = ["/home/pi/Desktop/data/database_1.csv",
          "/home/pi/Desktop/data/database_2.csv",
          "/home/pi/Desktop/data/database_3.csv",
          "/home/pi/Desktop/data/database_4.csv",
          "/home/pi/Desktop/data/database_5.csv",
          "/home/pi/Desktop/data/database_6.csv",
          "/home/pi/Desktop/data/database_7.csv",
          "/home/pi/Desktop/data/database_8.csv",
          "/home/pi/Desktop/data/database_9.csv",
          "/home/pi/Desktop/data/database_10.csv"]
url = "https://www.ceias.nau.edu/capstone/projects/EE/2021/NAUResearchGreenhouse-F20/Data_Upload.html"
url2 = 'https://www.ceias.nau.edu/capstone/projects/EE/2021/NAUResearchGreenhouse-F20/upload.php'
for file in filelist:
    myFile = open(file,"rb")
    r = requests.post(url2,data={'submit':'Upload'},files={'fileToUpload':myFile})
    print(r.text.find("The uploaded file has been accepted."))
    myFile.close()
