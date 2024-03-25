import os, sys

phone_number = ""
if int(os.environ.get("CONTENT_LENGTH", 0)) != 0:
    for _ in range(int(os.environ.get("CONTENT_LENGTH", 0))):
        phone_number += sys.stdin.read(1)

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>POST Request Received</title>")
print("</head>")
print("<body>")
print("<h1>Number received from POST request:</h1>")
phone_number = phone_number[phone_number.rfind("=") + 1 :]
print("<p>{}</p>".format(phone_number))
print("</body>")
print("</html>")
