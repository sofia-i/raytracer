# simple script that takes in RGB values on the [0, 255] scale and outputs corresponding [0, 1] values
rgb255 = (input("input rgb values in 255: ").replace(",", "")).split(" ")
rgb1 = [float(c) / 255. for c in rgb255]
rgb1_str = [f'{c:.2f}' for c in rgb1]
print(" ".join(rgb1_str))
