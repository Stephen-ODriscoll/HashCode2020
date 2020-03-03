#pragma once

#include "Globals.h"


class Library
{
    struct compareBooks {
        bool operator() (const uint64_t &lhs, const uint64_t &rhs) const
        {
            if (g_bookScores[lhs] == g_bookScores[rhs])
            {
                return lhs < rhs;
            }

            return g_bookScores[lhs] < g_bookScores[rhs];
        }
    };

    uint64_t m_myIndex;
    uint64_t m_numSignUpDays;
    uint64_t m_numBooksPerDay;

    std::set<uint64_t, compareBooks> m_books;
    std::set<uint64_t> m_booksUsed;

    uint64_t m_lastBookUsed;                                            // The lowest scoring book that made the cut
    std::vector<std::pair<uint64_t, uint64_t>> m_otherLibraryChanges;   // Library index with it's old book that's no longer scanned

    uint64_t m_score = 0;
    uint64_t m_actualScore = 0;


public:
    Library(const uint64_t myIndex, const uint64_t numSignUpDays, const uint64_t numBooksPerDay, const std::vector<uint64_t> &bookList) :
        m_myIndex(myIndex),
        m_numSignUpDays(numSignUpDays),
        m_numBooksPerDay(numBooksPerDay),
        m_books(bookList.begin(), bookList.end())
    {
        updateScore();
    }


    void updateScore()
    {
        m_actualScore = 0;
        m_booksUsed.clear();
        m_otherLibraryChanges.clear();
        std::map<uint64_t, uint64_t> libraryCounters;

        auto it = m_books.end();
        uint64_t numBooksScanned = (m_numSignUpDays < g_numDaysLeft) ? (g_numDaysLeft - m_numSignUpDays) * m_numBooksPerDay : 0;
        while (it != m_books.begin() && m_booksUsed.size() < numBooksScanned)
        {
            const auto bookStatus = g_bookStatus[*(--it)];
            if (bookStatus.first)
            {
                m_actualScore += g_bookScores[*it];
                m_booksUsed.insert(*it);
            }
            else
            {
                // Find next best book for this library
                auto it2 = it;
                bool found = false;
                while (it2 != m_books.begin())
                {
                    if (g_bookStatus[*(--it2)].first)
                    {
                        found = true;
                        break;
                    }
                }

                // Add counter for library so we don't count the same book twice
                if (libraryCounters.find(bookStatus.second) == libraryCounters.end())
                {
                    libraryCounters[bookStatus.second] = 0;
                }

                // Find next best book for the other library
                const std::pair<bool, uint64_t> otherLibrary = g_librariesUsed.find(bookStatus.second)->second.getNextBestBook(libraryCounters[bookStatus.second]);

                // If either library doesn't have a "next best" then we're done
                if (!found || !otherLibrary.first)
                {
                    continue;
                }

                // See who has the better "next best". The library with the worse alternative scans their common book
                if (g_bookScores[*it2] < g_bookScores[otherLibrary.second])
                {
                    m_otherLibraryChanges.push_back(std::make_pair(bookStatus.second, *it));
                    m_actualScore += g_bookScores[otherLibrary.second];
                    m_booksUsed.insert(*it);

                    ++libraryCounters[bookStatus.second];
                }
            }
        }

        m_lastBookUsed = (it == m_books.end()) ? *m_books.begin() : *it;
        m_score = m_actualScore / m_numSignUpDays;
    }


    const uint64_t getNumSignUpDays() const
    {
        return m_numSignUpDays;
    }

    const uint64_t getIndex() const
    {
        return m_myIndex;
    }

    const uint64_t getScore() const
    {
        return m_actualScore;
    }

    const std::set<uint64_t> getBooksUsed() const
    {
        return m_booksUsed;
    }

    const std::vector<std::pair<uint64_t, uint64_t>> getOtherLibraryChanges() const
    {
        return m_otherLibraryChanges;
    }

    const std::pair<bool, uint64_t> getNextBestBook(const uint64_t nextBestCount) const
    {
        auto it = m_books.find(m_lastBookUsed);
        for (uint64_t i = 0; i < (nextBestCount + 1);)
        {
            if (it == m_books.begin())
            {
                return std::make_pair(false, 0);
            }
            else if (g_bookStatus[*(--it)].first)
            {
                ++i;
            }
        }

        return std::make_pair(true, *it);
    }

    const uint64_t chooseNewBook(uint64_t oldBook)
    {
        const auto nextBestBook = getNextBestBook(0);

        m_booksUsed.erase(m_booksUsed.find(oldBook));
        m_booksUsed.insert(nextBestBook.second);
        m_lastBookUsed = nextBestBook.second;

        return nextBestBook.second;
    }


    bool operator<(const Library &rhs) const
    {
        return m_score < rhs.m_score;
    }

    void operator=(const Library &rhs)
    {
        m_myIndex = rhs.m_myIndex;
        m_numSignUpDays = rhs.m_numSignUpDays;
        m_numBooksPerDay = rhs.m_numBooksPerDay;

        m_books = rhs.m_books;
        m_booksUsed = rhs.m_booksUsed;

        m_lastBookUsed = rhs.m_lastBookUsed;
        m_otherLibraryChanges = rhs.m_otherLibraryChanges;

        m_score = rhs.m_score;
        m_actualScore = rhs.m_actualScore;
    }
};
