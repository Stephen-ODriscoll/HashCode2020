

if __name__ == "__main__":
    with open("d_quite_big.in", 'r') as file_text:
        line = file_text.readline().split(' ')
        slices = int(line[0])
        num_pizzas = int(line[1])
        pizzas = file_text.readline().split(' ')

    pizzas = [int(pizza) for pizza in pizzas]

    i, total, indexes = (len(pizzas) - 1), 0, []
    while 0 <= i:
        total += pizzas[i]
        indexes.append(i)
        if total < slices:
            i -= 1
            continue
        else:
            if slices == total:
                break
            else:
                total -= pizzas[i]
                indexes.__delitem__(len(indexes) - 1)
                i -= 1


    print(total)

    string = ""
    for i in indexes:
        string += str(i) + " "
    string = string[:len(string)-1]

    with open("output4.in", 'w') as output_text:
        output_text.write(str(len(indexes)) + "\n")
        output_text.write(string)
