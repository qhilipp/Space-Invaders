from PIL import Image

file = input("File name: ")
path = f"resources/{file}/{file}_texture"
image = Image.open(path + ".bmp")
image = image.convert("RGBA")

output = []

for y in range(image.height):
    line = ""
    for x in range(image.width):
        r, g, b, a = image.getpixel((x, y))
        if a > 0:
            line += "#"
        else:
            line += " "
    output.append(line)

with open(path + ".txt", "w") as f:
    for line in output:
        f.write(line + "\n")

print(f"Done, {path}.txt successfully created!")
