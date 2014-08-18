/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  Cesar Pachón cesarpachon@gmail.com http://www.cesarpachon.com

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


#ifndef OBPPULSE_H
#define OBPPULSE_H

/**
 * small utility class that keeps track of elapsed time, flags true when 
 * trigger time is reached or passed.. next trigger will begin not in zero
 * but in the residual time of the previous trigger. 
 */
class obpPulse
{

public:
obpPulse();
virtual ~obpPulse();
void init(float triggerTime);
/**
 * returns true if had passed triggerTime. 
 * see trigged()
 */ 
bool update(float elapsedTime);
/**
 * returns true if the last call to update returned true (passed trigger time)
 */
bool trigged();
private:
  float _triggerTime;
  float _elapsedTime;
  bool _trigged;
};

#endif // OBPPULSE_H
