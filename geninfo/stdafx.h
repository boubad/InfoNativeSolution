// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclure les en-t�tes Windows rarement utilis�s
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
