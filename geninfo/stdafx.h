// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
//////////////////////////////
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <cctype>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <mutex>
////////////////////////////////
#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>
#include <pplx/pplxtasks.h>
#include <cpprest/http_client.h>
////////////////////////////
#ifndef _MSC_VER
#include <boost/any.hpp>
#else
#include <any>
#endif // _MSC_VER
///////////////////////////
#include <boost/function_output_iterator.hpp>
#include <boost/bind.hpp>
//////////////////////////
