#pragma once

#include <unordered_map>
#include <memory>
class Process;
extern std::unordered_map<int, std::shared_ptr<Process>> process_table;
