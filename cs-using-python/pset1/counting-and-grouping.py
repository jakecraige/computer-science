order = "salad water hamburger salad hamburger"
expected = "salad:2 hamburger:2 water:1"


def item_order(order):
    salads = 0
    hamburgers = 0
    waters = 0
    for item in order.split(" "):
        if item == "salad":
            salads += 1
        elif item == "hamburger":
            hamburgers += 1
        elif item == "water":
            waters += 1
    return ("salad:" + str(salads) + " " +
            "hamburger:" + str(hamburgers) + " " +
            "water:" + str(waters))

print(str(item_order(order)))
