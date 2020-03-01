#pragma once

uint32_t g_numDaysLeft = 0;
std::vector<uint32_t> g_bookScores;

#include "Library.h"

std::vector<Library> g_librariesUsed;
std::vector<std::pair<bool, uint32_t>> g_bookStatus;

bool sortByIndex(const Library& lhs, const Library& rhs)
{
    return lhs.getIndex() < rhs.getIndex();
}

void insertSortByIndex(const Library& library)
{
    g_librariesUsed.insert
    (
        std::lower_bound(g_librariesUsed.begin(), g_librariesUsed.end(), library, sortByIndex),
        library
    );
}
