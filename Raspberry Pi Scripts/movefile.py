import shutil

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
num=1

try:
   for item in filelist:
      shutil.copy2(item, "/media/pi/DATA/database_%s.csv" % num)
      num+=1
   
except shutil.Error as e:
   print("Error: %s" % e)
   
except IOError as e:
   print("Error: %s" % e.strerror)
