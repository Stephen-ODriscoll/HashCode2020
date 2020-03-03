#pragma once

uint64_t g_numDaysLeft = 0;
std::vector<uint64_t> g_bookScores;

class Library;
std::map<uint64_t, Library> g_librariesUsed;            // Stores the libraries chosen so far with their index as key
std::vector<std::pair<bool, uint64_t>> g_bookStatus;    // Stores a pair of whether the book is usable or if not which library is using it

#include "Library.h"
