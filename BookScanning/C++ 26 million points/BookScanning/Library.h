#pragma once

#include "Globals.h"

class Library
{
    uint32_t m_myIndex;
    uint32_t m_numSignUpDays;
    uint32_t m_numBooksPerDay;

    uint32_t m_score = 0;
    uint32_t m_actualScore = 0;
    std::set<std::pair<uint32_t, uint32_t>> m_booksAndScore;


public:
    Library(const uint32_t myIndex, const uint32_t numSignUpDays, const uint32_t numBooksPerDay, const std::vector<uint32_t> &bookList) :
        m_myIndex(myIndex),
        m_numSignUpDays(numSignUpDays),
        m_numBooksPerDay(numBooksPerDay)
    {
        std::vector<uint32_t> bookListCopy = bookList;
        uint32_t numBooksScanned = (g_numDaysLeft - numSignUpDays) * numBooksPerDay;
        for (uint32_t i = 0; i < numBooksScanned; ++i)
        {
            uint32_t bestScore = 0;
            std::vector<uint32_t>::iterator bestBookIt;
            for (uint32_t j = 0; j < bookListCopy.size(); ++j)
            {
                if (bestScore < g_bookScores[bookListCopy[j]])
                {
                    bestScore = g_bookScores[bookListCopy[j]], bestBookIt = bookListCopy.begin() + j;
                }
            }

            if (bestScore == 0)
                break;

            m_actualScore += bestScore;
            m_booksAndScore.insert(std::make_pair(*bestBookIt, bestScore));
            bookListCopy.erase(bestBookIt);
        }

        m_score = m_actualScore / m_numSignUpDays;
    }


    void update(const std::set<std::pair<uint32_t, uint32_t>> &booksandScoresToRemove)
    {
        for (const auto& bookandScoreToRemove : booksandScoresToRemove)
        {
            auto it = m_booksAndScore.find(bookandScoreToRemove);
            if (it != m_booksAndScore.end())
            {
                m_actualScore -= it->second;
                m_booksAndScore.erase(it);
            }
        }

        m_score = m_actualScore / m_numSignUpDays;
    }


    const uint32_t getNumSignUpDays() const
    {
        return m_numSignUpDays;
    }

    const uint32_t getNumBooksPerDay() const
    {
        return m_numBooksPerDay;
    }

    const uint32_t getIndex() const
    {
        return m_myIndex;
    }

    const uint32_t getScore() const
    {
        return m_actualScore;
    }

    const std::set<std::pair<uint32_t, uint32_t>> getBooksAndScore() const
    {
        return m_booksAndScore;
    }


    bool operator<(const Library& rhs) const
    {
        return m_score < rhs.m_score;
    }

    void operator=(const Library& rhs)
    {
        m_myIndex = rhs.m_myIndex;
        m_numSignUpDays = rhs.m_numSignUpDays;
        m_numBooksPerDay = rhs.m_numBooksPerDay;

        m_score = rhs.m_score;
        m_actualScore = rhs.m_actualScore;
        m_booksAndScore = rhs.m_booksAndScore;
    }
};
