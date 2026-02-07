height = float(input("height -> "))
weight = float(input("weight -> "))

bmi = weight / (height / 100) ** 2

print(f"The BMI is {bmi}")