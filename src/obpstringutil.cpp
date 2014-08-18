/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "obpstringutil.h"

#include <iostream>
#include <sstream>

bool obpStringUtil::stringEndsWith(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}
//--------------------------------------


bool obpStringUtil::stringStartsWith (std::string const &fullString, std::string const &prefix)
{
    return prefix.size() <= fullString.size() && fullString.compare(0, prefix.size(), prefix) == 0;
}
//-----------------------------------

std::string obpStringUtil::toString(const int value) 
{
    std::stringstream st;
    st << value;
    return st.str();
}
//--------------------------------------

std::string obpStringUtil::toString(const bool value) 
{
    std::ostringstream oss;
    oss << std::boolalpha << value;
    return oss.str();
}
//--------------------------------------
