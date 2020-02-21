

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
        print(str(i) + " " + str(max(combinations)))
        i -= 1

    best_total = max(combinations)
    return best_total, combinations[best_total]


def main():
    with open("e_also_big.in", 'r') as file_text:
        line = file_text.readline().split(' ')
        slices = int(line[0])
        num_pizzas = int(line[1])
        pizzas = file_text.readline().split(' ')

    pizzas = [int(pizza) for pizza in pizzas]

    best_total, best_indexes = solve(slices, num_pizzas, pizzas)
    print(best_total)

    string = ""
    for i in best_indexes:
        string += str(i) + " "
    string = string[:len(string) - 1]

    with open("output4.in", 'w') as output_text:
        output_text.write(str(len(best_indexes)) + "\n")
        output_text.write(string)


if __name__ == "__main__":
    max_combinations = 500
    main()
