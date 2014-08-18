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


#ifndef OBPMATHUTIL_H
#define OBPMATHUTIL_H

#include "gameplay.h"

class obpMathUtil
{
public:
  /**
   *  based on gameplays forum entry:
   * http://www.gameplay3d.org/forums/viewtopic.php?f=3&t=315
   */
  static void lookAt(gameplay::Node* node, const gameplay::Vector3& eye, const gameplay::Vector3& target, const gameplay::Vector3& up);

};

#endif // OBPMATHUTIL_H
