"""
Author: Stephen O Driscoll
"""

# The best way I can describe this algorithm is brute force with an upper limit.

# A dictionary of all possible combinations of pizzas is created, where the key for this dictionary
# is the number of slices so far and the value is the indexes of the pizzas that make that score.
# E.g if pizzas 2, 3 and 5 = 30 slices and pizzas 1 and 4 = 30 slices we only need one of those combos.
# When max_combinations is reached we add new slices by updating existing combinations until we get as
# close as possible to the target number without going over.


# Change target to select input and output files
target = 0
max_combinations = 500

if not 0 <= target < 5:
    print("Target must be between 0 and 4")
    exit()


inputs = ["a_example.in", "b_small.in", "c_medium.in", "d_quite_big.in", "e_also_big.in"]
outputs = ["a.out", "b.out", "c.out", "d.out", "e.out"]


def solve(slices, num_pizzas, pizzas):
    combinations = dict()
    i = (num_pizzas - 1)
    while 0 <= i:
        for total, indexes in list(combinations.items()):
            if total + pizzas[i] < slices and total + pizzas[i] not in combinations:
                if max_combinations <= len(combinations):
                    combinations.__delitem__(total)
                combinations[total + pizzas[i]] = (indexes + [i])
            elif total + pizzas[i] == slices:
                return total + pizzas[i], (indexes + [i])

        if len(combinations) < max_combinations:
            combinations[pizzas[i]] = [i]

        # The printing is just for fun
        print("Pizzas left: " + str(i) + ", Best Score: " + str(max(combinations)))
        i -= 1

    best_total = max(combinations)
    return best_total, combinations[best_total]


def main():
    with open(inputs[target], 'r') as file_text:
        line = file_text.readline().split(' ')
        slices = int(line[0])
        num_pizzas = int(line[1])
        pizzas = file_text.readline().split(' ')

    pizzas = [int(pizza) for pizza in pizzas]

    best_total, best_indexes = solve(slices, num_pizzas, pizzas)
    print("Best Score: " + str(best_total))

    string = ""
    for i in best_indexes:
        string += str(i) + " "
    string = string[:len(string) - 1]

    with open(outputs[target], 'w') as output_text:
        output_text.write(str(len(best_indexes)) + "\n")
        output_text.write(string)


if __name__ == "__main__":
    main()
