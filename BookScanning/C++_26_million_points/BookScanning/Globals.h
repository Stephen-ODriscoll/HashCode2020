#pragma once

uint32_t g_numDaysLeft = 0;
std::vector<uint32_t> g_bookScores;

class Library;
std::map<uint32_t, Library> g_librariesUsed;
std::vector<std::pair<bool, uint32_t>> g_bookStatus;

#include "Library.h"
