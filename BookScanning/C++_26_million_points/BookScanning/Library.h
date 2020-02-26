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
        calculateActualScore();
        m_score = m_actualScore / m_numSignUpDays;
    }


    void update(const std::vector<uint32_t> &booksToRemove)
    {
        for (const auto &bookToRemove : booksToRemove)
        {
            auto it = m_books.find(bookToRemove);
            if (it != m_books.end())
            {
                m_books.erase(it);
            }
        }

        calculateActualScore();
        m_score = m_actualScore / m_numSignUpDays;
    }


    void calculateActualScore()
    {
        m_actualScore = 0;
        m_booksUsed.clear();
        auto it = m_books.end();
        
        uint32_t numBooksScanned = (m_numSignUpDays < g_numDaysLeft) ? (g_numDaysLeft - m_numSignUpDays) * m_numBooksPerDay : 0;
        for (uint32_t i = 0; i < numBooksScanned && it != m_books.begin(); ++i)
        {
            m_actualScore += g_bookScores[*(--it)];
            m_booksUsed.push_back(*it);
        }
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

        m_score = rhs.m_score;
        m_actualScore = rhs.m_actualScore;
    }
};
