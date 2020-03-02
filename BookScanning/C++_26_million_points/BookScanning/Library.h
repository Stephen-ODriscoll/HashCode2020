#pragma once

#include "Globals.h"

class Library
{
    struct compareBooks {
        bool operator() (const uint32_t &lhs, const uint32_t &rhs) const
        {
            if (g_bookScores[lhs] == g_bookScores[rhs])
            {
                return lhs < rhs;
            }

            return g_bookScores[lhs] < g_bookScores[rhs];
        }
    };

    uint32_t m_myIndex;
    uint32_t m_numSignUpDays;
    uint32_t m_numBooksPerDay;

    std::set<uint32_t, compareBooks> m_books;
    std::set<uint32_t> m_unusableBooks;
    std::set<uint32_t> m_booksUsed;

    std::vector<std::vector<uint32_t>> m_otherLibraryChanges;    // Library index with it's old book and new book to scan
    std::set<uint32_t, compareBooks>::iterator m_nextBestIt;                        // Iterator to the next best book to be used

    uint32_t m_score = 0;
    uint32_t m_actualScore = 0;


public:
    Library(const uint32_t myIndex, const uint32_t numSignUpDays, const uint32_t numBooksPerDay, const std::vector<uint32_t> &bookList) :
        m_myIndex(myIndex),
        m_numSignUpDays(numSignUpDays),
        m_numBooksPerDay(numBooksPerDay),
        m_books(bookList.begin(), bookList.end())
    {
        calculateScore();
    }

    Library()
    {

    }


    void updateBooks(const std::set<uint32_t> &books)
    {
        for (const auto &book : books)
        {
            if (m_books.find(book) != m_books.end())
            {
                m_unusableBooks.insert(book);
            }
        }

        calculateScore();
    }


    void calculateScore()
    {
        m_actualScore = 0;
        m_booksUsed.clear();
        m_otherLibraryChanges.clear();
        std::map<uint32_t, uint32_t> libraryCounters;

        auto it = m_books.end();
        uint32_t numBooksScanned = (m_numSignUpDays < g_numDaysLeft) ? (g_numDaysLeft - m_numSignUpDays) * m_numBooksPerDay : 0;
        for (uint32_t i = 0; i < numBooksScanned && it != m_books.begin();)
        {
            if (m_unusableBooks.find(*(--it)) == m_unusableBooks.end())
            {
                m_actualScore += g_bookScores[*it];
                m_booksUsed.insert(*it);
                ++i;
            }
            else
            {
                // Check if this library scanning the book will let another library scan a better book
                auto it2 = it;
                if (!g_bookStatus[*it2].first)
                {
                    continue;
                }

                const auto libraryIndex = g_bookStatus[*it2].second;
                if (libraryCounters.find(libraryIndex) == libraryCounters.end())
                {
                    libraryCounters[libraryIndex] = 0;
                }
                else
                {
                    ++libraryCounters[libraryIndex];
                }

                bool found = false;
                while (it2 != m_books.begin())
                {
                    if (m_unusableBooks.find(*(--it2)) != m_unusableBooks.end())
                    {
                        found = true;
                        break;
                    }
                }

                const std::pair<bool, uint32_t> otherLibrary = g_librariesUsed[libraryIndex].getNextBestBook(libraryCounters[libraryIndex]);

                if (!found || !otherLibrary.first)
                {
                    continue;
                }

                if (g_bookScores[*it2] < g_bookScores[otherLibrary.second])
                {
                    m_otherLibraryChanges.push_back(std::vector<uint32_t>{libraryIndex, *it, otherLibrary.second});
                    m_actualScore += g_bookScores[otherLibrary.second];
                    m_booksUsed.insert(*it);
                    ++i;
                }
            }
        }

        if (it == m_books.begin())
        {
            m_nextBestIt = it;
        }
        else
        {
            m_nextBestIt = --it;
        }
        
        m_score = m_actualScore / m_numSignUpDays;
    }


    const uint32_t getNumSignUpDays() const
    {
        return m_numSignUpDays;
    }

    const uint32_t getIndex() const
    {
        return m_myIndex;
    }

    const uint32_t getScore() const
    {
        return m_actualScore;
    }

    const std::set<uint32_t> getBooksUsed() const
    {
        return m_booksUsed;
    }

    const std::vector<std::vector<uint32_t>> getOtherLibraryChanges() const
    {
        return m_otherLibraryChanges;
    }

    const std::pair<bool, uint32_t> getNextBestBook(uint32_t nextBestCount) const
    {
        auto nextBestItCopy = m_nextBestIt;
        if (nextBestItCopy == m_books.begin())
        {
            return std::make_pair(false, 0);
        }

        for (uint32_t i = 0; i < nextBestCount; ++i)
        {
            if (nextBestItCopy == m_books.begin())
            {
                return std::make_pair(false, 0);
            }

            --nextBestItCopy;
        }

        return std::make_pair(true, *nextBestItCopy);
    }

    void updateBooksUsed(uint32_t oldBook, uint32_t newBook)
    {
        m_booksUsed.erase(m_booksUsed.find(oldBook));
        m_booksUsed.insert(newBook);
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
        m_unusableBooks = rhs.m_unusableBooks;
        m_booksUsed = rhs.m_booksUsed;

        m_nextBestIt = rhs.m_nextBestIt;
        m_otherLibraryChanges = rhs.m_otherLibraryChanges;

        m_score = rhs.m_score;
        m_actualScore = rhs.m_actualScore;
    }
};
