#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>

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

    uint32_t totalScore = 0;
    std::string solution = "";
    uint32_t numLibrariesUsed = 0;
    for (; 0 < g_numDaysLeft && !libraries.empty(); ++numLibrariesUsed)
    {
        std::sort(libraries.begin(), libraries.end());
        auto library = libraries.end() - 1;

        // Validate the score isn't 0
        if (library->getScore() == 0)
        {
            break;
        }

        // Add to solution
        const auto &books = library->getBooksUsed();
        solution += std::to_string(library->getIndex()) + " " + std::to_string(books.size()) + "\n";
        for (const auto& book : books)
        {
            solution += std::to_string(book) + " ";
        }
        solution += "\n";

        // Update total and days left
        totalScore += library->getScore();
        g_numDaysLeft -= library->getNumSignUpDays();

        // Update other library scores
        for (auto it = libraries.begin(); it < (libraries.end() - 1); ++it)
        {
            it->update(books);
        }

        // Erase it
        std::cout << "Days Left: " << g_numDaysLeft <<  std::endl;
        libraries.erase(library);
    }

    std::cout << "Total Score: " << totalScore << std::endl;

    std::ofstream output(outputs[target]);
    output << numLibrariesUsed << "\n";
    output << solution;
    output.close();
}
