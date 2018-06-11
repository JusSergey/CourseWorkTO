#ifndef TYPES_H
#define TYPES_H

//universal callback
#include <functional>

// basic I/O
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// containers
#include <map>
#include <list>
#include <vector>
#include <set>

//using without namespace 'std'
using std::stringstream;
using std::string;
using std::cout;
using std::cin;
using std::map;
using std::list;
using std::vector;
using std::set;
using std::pair;
using std::function;

// my typedef's
using StringList     = list     <string>;
using StringVector   = vector   <string>;
using StringPair     = pair     <string, string>;
using ListStringPair = list     <StringPair>;
using StringSet      = set      <string>;
using ListStrVects   = list     <StringVector>;

// my callback's
using CallbackColorByPos = function<std::string(int row, int col)>;

// my enum's
enum class DEVICE : int { UNDEFINED,
                          PC,
                          HARDWARE,
                          OS,
                          CPU,
                          DMI,
                          GPU,
                          USB,
                          NETWORK,
                          HARD_DRIVE,
                          RAM,
                          MEMORY,
                          PROCESSES,
                          MODULES,
                          RAM_MEMORY };



#endif // TYPES_H
