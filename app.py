import math

weight = input("Weight: ")
unit = input("(L)bs or (K)g: ")
if unit == "L" or unit == "l":
    converted_weight = 0.45 * int(weight)
    print(f"You weigh {math.ceil(converted_weight)} kg")
elif unit == "K" or unit == "k":
    converted_weight = 1/0.45 * int(weight)
    print(f"You weigh {math.ceil(converted_weight)} lbs")
else:
    print("Wrong input")

