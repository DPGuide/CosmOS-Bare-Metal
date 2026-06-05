import urllib.request
import os

os.chdir(r"d:\coding\MeinOS\test_folder")

print("Downloading test.png...")
urllib.request.urlretrieve("https://upload.wikimedia.org/wikipedia/commons/4/47/PNG_transparency_demonstration_1.png", "test.png")

print("Downloading test.jpg...")
urllib.request.urlretrieve("https://upload.wikimedia.org/wikipedia/commons/thumb/a/a7/React-icon.svg/256px-React-icon.svg.png", "test.jpg")

print("Downloading test.bmp...")
# Since finding a clean BMP online is hard, we can just save the JPG as BMP using PIL if available, or just fetch one
urllib.request.urlretrieve("https://file-examples.com/wp-content/storage/2017/10/file_example_BMP_500kB.bmp", "test.bmp")

html_content = """
<h1>MeinOS Browser Test</h1>
<p>Hier sind einige Links zu Bildern. Klicke darauf, um sie herunterzuladen und anzuzeigen!</p>
<br>
<ul>
<li><a href="test.png">Test PNG Image</a></li>
<li><a href="test.jpg">Test JPG Image</a></li>
<li><a href="test.bmp">Test BMP Image</a></li>
</ul>
<br>
<p>Fortschrittsbalken-Test: Versuch, diese Bilder auch mit GET herunterzuladen!</p>
"""

with open("index.html", "w") as f:
    f.write(html_content)

print("Done! You can now run: python -m http.server 8000")
