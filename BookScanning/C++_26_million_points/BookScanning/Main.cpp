#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>

#include "Globals.h"
#include "Library.h"

std::vector<uint32_t> split(const std::string& str, wchar_t delim)
{
    std::vector<uint32_t> result;
    size_t last = 0, next = 0;
    for (; next < str.length(); ++next)
    {
        if (str[next] == delim)
        {
            auto substr = str.substr(last, (next - last));
            if (!substr.empty())
            {
                result.push_back(atoi(substr.c_str()));
            }

            last = (next + 1);
        }
    }

    auto substr = str.substr(last, (next - last));
    if (!substr.empty())
    {
        result.push_back(atoi(substr.c_str()));
    }

    return std::move(result);
}


int main(int argc, char* argv[])
{
    uint8_t target = 6;
    if (argc == 2)
    {
        target = uint8_t(atoi(argv[1]));
    }
    
    if(5 < target)
    {
        std::cout << "Please include target in range 0 to 5 as argument 1" << std::endl;
        exit(1);
    }
    
    std::string inputs[]  = {   "C:\\Users\\steph\\OneDrive\\Desktop\\HashCode2020\\BookScanning\\a_example.txt",
                                "C:\\Users\\steph\\OneDrive\\Desktop\\HashCode2020\\BookScanning\\b_read_on.txt",
                                "C:\\Users\\steph\\OneDrive\\Desktop\\HashCode2020\\BookScanning\\c_incunabula.txt",
                                "C:\\Users\\steph\\OneDrive\\Desktop\\HashCode2020\\BookScanning\\d_tough_choices.txt",
                                "C:\\Users\\steph\\OneDrive\\Desktop\\HashCode2020\\BookScanning\\e_so_many_books.txt",
                                "C:\\Users\\steph\\OneDrive\\Desktop\\HashCode2020\\BookScanning\\f_libraries_of_the_world.txt"};
    std::string outputs[] = { "a.txt", "b.txt", "c.txt", "d.txt", "e.txt", "f.txt"};

    std::ifstream input(inputs[target]);

    std::string line;
    std::getline(input, line);
    auto splits = split(line, ' ');
    uint32_t numLibraries = splits[1], numDays = splits[2];
    g_numDaysLeft = numDays;

    std::getline(input, line);
    g_bookScores = split(line, ' ');

    std::cout << "Calculating Initial Scores" << std::endl;
    std::vector<Library> libraries;
    for (uint32_t i = 0; i < numLibraries; ++i)
    {
        std::getline(input, line);
        splits = split(line, ' ');

        uint32_t numSignUpDays = splits[1], numBooksPerDay = splits[2];
        std::getline(input, line);

        libraries.push_back(Library(i, numSignUpDays, numBooksPerDay, split(line, ' ')));
    }

    input.close();
    g_bookStatus = std::vector<std::pair<bool, uint32_t>>(g_bookScores.size(), std::make_pair(false, 0));

    uint32_t totalScore = 0;
    while (0 < g_numDaysLeft && !libraries.empty())
    {
        std::sort(libraries.begin(), libraries.end());
        const auto plibrary = (libraries.end() - 1);
        const auto library = *plibrary;
        g_librariesUsed[library.getIndex()] = library;
        libraries.erase(plibrary);

        // Validate the score isn't 0
        if (library.getScore() == 0)
        {
            break;
        }

        // Update which library is scanning which book
        const auto libraryIndex = library.getIndex();
        const auto &booksUsed = library.getBooksUsed();
        for (const auto bookUsed : booksUsed)
        {
            g_bookStatus[bookUsed] = std::make_pair(true, libraryIndex);
        }

        // Update changes to other libraries already chosen
        std::set<uint32_t> otherBooksUsed;
        const auto& otherLibraryChanges = library.getOtherLibraryChanges();
        for (const auto& libraryChange : otherLibraryChanges)
        {
            g_librariesUsed[libraryChange[0]].updateBooksUsed(libraryChange[1], libraryChange[2]);
            g_bookStatus[libraryChange[2]] = std::make_pair(true, libraryChange[0]);
            otherBooksUsed.insert(libraryChange[2]);
        }

        // Update total, days left and other libraries
        totalScore += library.getScore();
        g_numDaysLeft -= library.getNumSignUpDays();
        for (auto it = libraries.begin(); it < libraries.end(); ++it)
        {
            it->updateBooks(booksUsed);
            it->updateBooks(otherBooksUsed);
        }

        std::cout << "Days Left: " << g_numDaysLeft << std::endl;
    }

    std::vector<Library> librariesUsed;
    for (const auto& libraryUsed : g_librariesUsed)
    {
        librariesUsed.push_back(libraryUsed.second);
    }

    std::sort(librariesUsed.begin(), librariesUsed.end());
    std::cout << "Total Score: " << totalScore << std::endl;

    // Convert libraries used to string for output
    std::string solution = "";
    for (const auto& libraryUsed : librariesUsed)
    {
        const auto& books = libraryUsed.getBooksUsed();
        solution += std::to_string(libraryUsed.getIndex()) + " " + std::to_string(books.size()) + "\n";
        for (const auto& book : books)
        {
            solution += std::to_string(book) + " ";
        }
        solution += "\n";
    }

    std::ofstream output(outputs[target]);
    output << librariesUsed.size() << "\n";
    output << solution;
    output.close();
}
