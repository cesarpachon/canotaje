/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  cesar pachon http://www.cesarpachon.com cesarpachon@gmail.com

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


#include "obpmathutil.h"

using namespace gameplay;

void obpMathUtil::lookAt(Node* node, const Vector3& eye, const Vector3& target, const Vector3& up)
{
    // Create lookAt matrix
    Matrix matrix;
    Matrix::createLookAt(eye, target, up, &matrix);
    matrix.invert();  

    // Pull SRT components out of matrix
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    matrix.decompose(&scale, &rotation, &translation);

    // Set SRT on node
    node->setScale(scale);
    node->setTranslation(translation);
    node->setRotation(rotation);
}