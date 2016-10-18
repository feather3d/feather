/***********************************************************************
 *
 * Filename: curve.hpp
 *
 * Description: Tools to handle curve calculations.
 *
 * Copyright (C) 2015 Richard Layman, rlayman2000@yahoo.com 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#ifndef CURVE_HPP
#define CURVE_HPP

#include "deps.hpp"

namespace feather
{

    namespace curve
    {
    
        // This is used to get the value on a bezier curve for a given time and is mainly
        // used in the KeyTrack node and other widgets that deal with values in the time line
        FReal get_cubic_bezier_x(FPoint2D p0, FPoint2D p1, FPoint2D p2, FPoint2D p3, FReal time, FReal fps)
        {
            // there's probably a better way of doing this but I'll just do the simple way for now.
            // T will be the a 0.0 to 1.0 value that will tell the point on the curve.
            // compare this to T and, if the value falls within the bounds, we'll output it.
            // If the value doesn't, we'll cut it in half and recalc until it does.
            
            // first we'll do a quick check to see if the value is matches our start or end points.
            if(p0.x == time)
                return p0.y;
            if(p3.x == time)
                return p3.y;
            
            // This should never happen, but let's just make sure the time falls within the points
            if(p0.x > time || p3.x < time) 
                return 0.0;

            // Get the bound value
            double boundRange = 1.0/fps;
            double minBoundTarget = time - (boundRange/2.0);
            double maxBoundTarget = time + (boundRange/2.0);

            // lets start at the halfway point
            double t = 0.5;
            bool inRange = false;

            /*
            std::cout << "get_cubic_bezier_x(p0(" << p0.x << "," << p0.y
                << "), p1(" << p1.x << "," << p1.y
                << "), p2(" << p2.x << "," << p2.y 
                << "), p3(" << p3.x << "," << p3.y
                << ", time(" << time << "), fps(" << fps << ")\n"
                ;

            std::cout << "bounding range " << boundRange << " minBound=" << minBoundTarget << " maxBound=" << maxBoundTarget << std::endl;
            */
            while(!inRange){
                // to save on time, we'll only calculate y once x is in range
                double x = p0.x + 3 * t * (p1.x - p0.x) + 3 * t * t * (p0.x + p2.x - 2 * p1.x) + t * t * t * (p3.x - p0.x + 3 * p1.x - 3 * p2.x);
                //std::cout << "calculated x=" << x << std::endl;
                if ( x > minBoundTarget && x < maxBoundTarget ) {
                    //std::cout << "value found, x=" << x << " t=" << t << std::endl;
                    return p0.y + 3 * t * (p1.y - p0.y) + 3 * t * t * (p0.y + p2.y - 2 * p1.y) + t * t * t * (p3.y - p0.y + 3 * p1.y - 3 * p2.y);
                }
                if ( x > maxBoundTarget )
                    t -= t/2.0; 
                else
                    t += t/2.0;
            }

            // we should never get this far
        };
    } // namespace curve

} // namespace feather

#endif
