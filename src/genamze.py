from random import shuffle, randrange
import sys
 
def make_maze(w = 16, h = 8):
    vis = [[0] * w + [1] for _ in range(h)] + [[1] * (w + 1)]
    ver = [[u"\u2588  "] * w + [u'\u2588'] for _ in range(h)] + [[]]
    hor = [[u"\u2588\u2588\u2588"] * w + [u'\u2588'] for _ in range(h + 1)]
 
    def walk(x, y):
        vis[y][x] = 1
 
        d = [(x - 1, y), (x, y + 1), (x + 1, y), (x, y - 1)]
        shuffle(d)
        for (xx, yy) in d:
            if vis[yy][xx]: continue
            if xx == x: hor[max(y, yy)][x] = u"\u2588  "
            if yy == y: ver[y][max(x, xx)] = "   "
            walk(xx, yy)
 
    walk(randrange(w), randrange(h))
 
    s = ""
    for (a, b) in zip(hor, ver):
        s += ''.join(a + ['\n'] + b + ['\n'])
    return s
 
if __name__ == '__main__':
    print("\x1b[34m")
    print(make_maze(int(sys.argv[1]), int(sys.argv[2])))
    print("\x1b[0m")
