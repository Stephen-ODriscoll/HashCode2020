

if __name__ == "__main__":
    with open("e_also_big.in", 'r') as file_text:
        line = file_text.readline().split(' ')
        slices = int(line[0])
        num_pizzas = int(line[1])
        pizzas = file_text.readline().split(' ')

    pizzas = [int(pizza) for pizza in pizzas]

    best_total = 0
    best_indexes = []
    i = (num_pizzas - 1)
    while best_total != slices and i != 0:
        j, total, indexes = i, 0, []
        while 0 <= j:
            indexes.append(j)
            total += pizzas[j]
            if total == slices:
                break
            elif slices < total:
                indexes.pop(-1)
                total -= pizzas[j]
            j -= 1

        if best_total < total:
            best_total = total
            best_indexes = indexes
        i -= 1

    print(best_total)
    string = ""
    for i in best_indexes:
        string += str(i) + " "
    string = string[:len(string) - 1]

    with open("output4.in", 'w') as output_text:
        output_text.write(str(len(best_indexes)) + "\n")
        output_text.write(string)
