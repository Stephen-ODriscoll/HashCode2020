#########################################
#                                       #
#   Authors:                            #
#           Stephen O Driscoll          #
#           Roshan Sreekanth            #
#           Samyukta Sreekanth          #
#           Fabien Sailliet             #
#                                       #
#########################################
# Achieves a score of 16 million. The best score at the moment is 27 mil (there's room for improvement)

# Change target to select input and output files
target = 2

if not 0 <= target < 6:
    print("Target must be between 0 and 5")
    exit()


inputs = ["../a_example.txt", "../b_read_on.txt", "../c_incunabula.txt",
          "../d_tough_choices.txt", "../e_so_many_books.txt", "../f_libraries_of_the_world.txt"]
outputs = ["a.txt", "b.txt", "c.txt", "d.txt", "e.txt", "f.txt"]


def find_best_book(list_of_books, books_to_scan):
    best_score, index = 0, 0
    for i, book in enumerate(list_of_books):
        if book in books_to_scan and best_score < book_scores[book]:
            best_score, index = book_scores[book], i
    return best_score, index


def evaluate_library(num_days_left, books_to_scan, library_details):
    lib_score = 0
    scores_per_books = set()
    library_books_copy = list(library_details["books"])
    for _ in range(num_days_left - library_details["register_days"]):
        for _ in range(library_details["books_per_day"]):
            if len(library_books_copy) == 0:
                return lib_score, scores_per_books

            book_score, index = find_best_book(library_books_copy, books_to_scan)

            if book_score == 0:
                return lib_score, scores_per_books

            scores_per_books.add((book_score, library_books_copy.pop(index)))
            lib_score += book_score
    return lib_score, scores_per_books


def main():
    # Calculate score for each library
    print("Calculating initial scores")
    libraries_scores = dict()
    for i, lib in enumerate(libraries_details):
        lib_score, scores_per_books = evaluate_library(num_days, set(range(num_books)), lib)
        libraries_scores[(lib_score, i)] = [scores_per_books, lib["register_days"], lib["books_per_day"]]

    print("Processing...")
    total_score = 0
    result, num_days_left, books_to_scan, num_lib = "", num_days, set(range(num_books)), 0
    while 0 < num_days_left and libraries_scores:
        # Pick the best library
        best_score_and_index = max(libraries_scores)
        total_score += best_score_and_index[0]

        result += str(best_score_and_index[1]) + " " + str(len(libraries_scores[best_score_and_index][0])) + "\n"
        result += ' '.join(map(str, [i[1] for i in libraries_scores[best_score_and_index][0]])) + "\n"

        # Remove books and update scores
        removed = libraries_scores.pop(best_score_and_index)
        for score_per_book in removed[0]:
            books_to_scan.remove(score_per_book[1])
            for key in list(libraries_scores.keys()):
                new_score = key[0]
                if score_per_book in libraries_scores[key][0]:
                    libraries_scores[key][0].remove(score_per_book)
                    new_score -= score_per_book[0]

                set_length = len(libraries_scores[key][0])
                # (num days left - days to register) * books per day
                while 0 <= (num_days_left - libraries_scores[key][1]) * libraries_scores[key][2] < set_length:
                    minimum = min(libraries_scores[key][0])
                    libraries_scores[key][0].remove(minimum)
                    new_score -= minimum[0]
                    set_length -= 1

                libraries_scores[(new_score, key[1])] = libraries_scores.pop(key)

        num_days_left -= removed[1]
        num_lib += 1

    print("Total Score: " + str(total_score))
    result = str(num_lib) + "\n" + result
    with open(outputs[target], 'w') as output_file:
        output_file.write(result)


if __name__ == "__main__":
    with open(inputs[target], 'r') as file_text:
        splits = list(map(int, file_text.readline().split()))
        num_books, num_libraries, num_days = splits
        book_scores = list(map(int, file_text.readline().split()))

        libraries_details = []
        for _ in range(num_libraries):
            splits = [int(s) for s in file_text.readline().split()]
            libraries_details.append({
                "books_nb": int(splits[0]),
                "register_days": int(splits[1]),
                "books_per_day": int(splits[2]),
                "books": list(map(int, file_text.readline().split()))
            })
    main()
