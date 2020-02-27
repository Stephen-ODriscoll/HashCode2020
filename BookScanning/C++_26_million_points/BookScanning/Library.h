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

    std::set<uint32_t, compareBooks> m_usableBooks;
    std::set<uint32_t> m_unusableBooks;
    std::vector<uint32_t> m_booksUsed;

    uint32_t m_score = 0;
    uint32_t m_actualScore = 0;


public:
    Library(const uint32_t myIndex, const uint32_t numSignUpDays, const uint32_t numBooksPerDay, const std::vector<uint32_t> &bookList) :
        m_myIndex(myIndex),
        m_numSignUpDays(numSignUpDays),
        m_numBooksPerDay(numBooksPerDay),
        m_usableBooks(bookList.begin(), bookList.end())
    {
        calculateActualScore();
        m_score = m_actualScore / m_numSignUpDays;
    }


    void update(const std::vector<uint32_t> &unusableBooks)
    {
        for (const auto &unusableBook : unusableBooks)
        {
            auto it = m_usableBooks.find(unusableBook);
            if (it != m_usableBooks.end())
            {
                m_usableBooks.erase(it);
                m_unusableBooks.insert(unusableBook);
            }
        }

        calculateActualScore();
        m_score = m_actualScore / m_numSignUpDays;
    }


    void backtrack(const std::vector<uint32_t>& usableBooks)
    {
        for (const auto& usableBook : usableBooks)
        {
            auto it = m_unusableBooks.find(usableBook);
            if (it != m_unusableBooks.end())
            {
                m_unusableBooks.erase(it);
                m_usableBooks.insert(usableBook);
            }
        }

        calculateActualScore();
        m_score = m_actualScore / m_numSignUpDays;
    }


    void calculateActualScore()
    {
        m_actualScore = 0;
        m_booksUsed.clear();

        auto it = m_usableBooks.end();
        uint32_t numBooksScanned = (m_numSignUpDays < g_numDaysLeft) ? (g_numDaysLeft - m_numSignUpDays) * m_numBooksPerDay : 0;
        for (uint32_t i = 0; i < numBooksScanned && it != m_usableBooks.begin(); ++i)
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

        m_usableBooks = rhs.m_usableBooks;
        m_unusableBooks = rhs.m_unusableBooks;
        m_booksUsed = rhs.m_booksUsed;

        m_score = rhs.m_score;
        m_actualScore = rhs.m_actualScore;
    }
};
