import random
import string

def generate_big_random_letters(filename, size):

    for j in range(300000):
        content = ""
        for k in range(100):
            chars = ''.join([random.choice(string.ascii_letters) for i in range(size)])
            content += chars + " "
        with open(filename, 'a') as f:
            f.write(content)
            f.write("\n")
        pass

for i in range(5, 10):
    generate_big_random_letters("prueba.txt", i)
