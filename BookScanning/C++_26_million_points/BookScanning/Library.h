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
    std::set<uint32_t, compareBooks>::iterator m_nextBestIt;
    std::set<uint32_t> m_unusableBooks;
    std::vector<uint32_t> m_booksUsed;

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


    void updateBooks(const std::vector<uint32_t> &books)
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
        std::map<uint32_t, uint32_t> libraryCounters;

        auto it = m_books.end();
        uint32_t numBooksScanned = (m_numSignUpDays < g_numDaysLeft) ? (g_numDaysLeft - m_numSignUpDays) * m_numBooksPerDay : 0;
        for (uint32_t i = 0; i < numBooksScanned && it != m_books.begin();)
        {
            if (m_unusableBooks.find(*(--it)) == m_unusableBooks.end())
            {
                m_actualScore += g_bookScores[*it];
                m_booksUsed.push_back(*it);
                ++i;
            }
            else
            {
                // Check if the library scanning the book will let another library scan a better book
                const auto libraryIndex = g_bookStatus[*it].second;
                if (libraryCounters.find(libraryIndex) == libraryCounters.end())
                {
                    libraryCounters[libraryIndex] = 0;
                }
                else
                {
                    ++libraryCounters[libraryIndex];
                }

                uint32_t otherLibraryScore = g_bookScores[g_librariesUsed[libraryIndex].getNextBestBook(libraryCounters[libraryIndex])];

                auto it2 = it;
                while (it2 != m_books.begin() && m_unusableBooks.find(*(--it2)) != m_unusableBooks.end()) {}

                if (it2 != m_books.end() && )
                {

                }
            }
        }

        m_nextBestIt == --it;
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

    const std::vector<uint32_t> getBooksUsed() const
    {
        return m_booksUsed;
    }

    const uint32_t getNextBestBook(uint32_t nextBestCount) const
    {
        auto nextBestItCopy = m_nextBestIt;
        for (uint32_t i = 0; i < nextBestCount; ++i)
        {
            --nextBestItCopy;
        }

        return *nextBestItCopy;
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

        m_score = rhs.m_score;
        m_actualScore = rhs.m_actualScore;
    }
};
