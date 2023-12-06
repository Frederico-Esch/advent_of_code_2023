games = open("input.txt", "r").read()

games = games.split("\n")
games = games[:-1]
print(len(games))

sum = 0
for i, g in enumerate(games):
    aux = g[g.find(":")+2:].split("; ")
    sets = list(map(lambda x: x.split(", "), aux))

    is_valid = True
    for s in sets:
        for vc in s:
            value, color = vc.split(" ")
            value = int(value)
            if (value > 12 and color == "red") or (value > 13 and color == "green") or (value > 14 and color == "blue"):
                is_valid = False
                break
        if not is_valid:
            break
    if is_valid:
        sum += 1

print(sum)
