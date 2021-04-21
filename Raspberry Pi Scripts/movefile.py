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
            "/home/pi/Desktop/data/database_10.csv"]  # Predefined list of all database files
num = 1  # Iterator

try:
    for item in filelist:  # For items in file list
        shutil.copy2(item, "/media/pi/DATA/database_%s.csv" %
                     num)  # Copy file to USB at path
        num += 1  # Iterate iterator

except shutil.Error as e:
    print("Error: %s" % e)

except IOError as e:
    print("Error: %s" % e.strerror)
