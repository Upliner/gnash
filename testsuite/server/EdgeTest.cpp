// 
//   Copyright (C) 2005, 2006, 2007 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gnash.h" // for point !!
#include "shape.h" // for edge
#include <iostream>
#include <sstream>
#include <cassert>

#include "check.h"

using namespace std;
using namespace gnash;

// for double comparison
struct D {
	double _d;
	double _t; // tolerance

	D(double d) : _d(d), _t(1e-4) {}

	// Set tolerance
	D(double d, double t) : _d(d), _t(t) {}

	// Return true if the difference between the two
	// doubles is below the minimum tolerance defined for the two
	bool operator==(const D& d)
	{
		double tol = std::min(_t, d._t);
		double delta = fabs(_d - d._d);
		bool ret = delta < tol;
		//cout << "D " << _d << "operator==(const D " << d._d <<") returning " << ret << " (delta is " << delta << ") " << endl;
		return ret;
	}
};
std::ostream& operator<<(std::ostream& os, const D& d)
{
	return os << d._d << " [tol: " << d._t << "]";
}

int
main(int /*argc*/, char** /*argv*/)
{

	//
	// Test distance
	//

	check_equals(edge::distancePtSeg(point(0,0), point(9, 0), point(9, 0)), 9);

	check_equals(edge::distancePtSeg(point(0,0), point(0, 0), point(3, 0)), 0);

	check_equals(edge::distancePtSeg(point(-5,0), point(0, 0), point(3, 0)), 5);

	check_equals(edge::distancePtSeg(point(5,0), point(0, 0), point(3, 0)), 2);

	check_equals(D(edge::distancePtSeg(point(0,0), point(-10, 0), point(3, 0))), 0);

	check_equals(edge::distancePtSeg(point(0,0), point(-10, 0), point(-10, 30)), 10);

	check_equals(edge::distancePtSeg(point(5,5), point(-10, 0), point(10, 0)), 5);

}

