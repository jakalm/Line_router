# Line router
A very simple C++ application routing lines to not intersect each other. The Graphical User Interface (GUI) consists of
a 600 x 600 pixel map in which it is possible to draw lines by mouse clicks. The first click will set the start point of
the line and the second click will set the end point and draw the line. The line will be drawn so that it will not
intersect with any other line nor the pixel map border.

## Requirements
This has only been tested on __Ubuntu Linux 18.04 LTS__ running  

* __CMake 3.10.2__
* __QT 5.9.5__
* __libcanberra-gtk-module 0.30__
* __gcc/g++ 7.3.0__
* __GNU Make 4.1__

with common software development packages. It is not configured to build for Windows yet.

## Build
All the commands listed here are assumed to be run in a Linux shell.  
To configure the __CMake__ build environment (or to clean it), go to the `Build` directory under the root path and run  

```
./cmake_build.sh
```

This should only have to be run once initially or when wanting to clean the CMake environment. To make the Line router
application run  
  
```
make line_router
```

On success the binary file will end up in the `Bin/line_router` folder under the root path.

## Unit tests
__Google Test 1.8.0__ has been integrated with the source code and will be built when building the tests. Configure the
CMake build environment if not already done so (see the Build section above)  

```
./cmake_build.sh
```

To build and run the tests run  

```
make all test
```

## Code structure
__CMake__ and __GNU Make__ is used as a build environment to generate the executable. The source code consists of three
main parts

* __Line router main__
* __UI (QT 5)__
* __Path planner (A\*)__

and grid help classes under __Grid__.

### Line router main
The main function decides what kind of path planner to use (currently there is only `A_star_planner` available) and
its grid size. Right now it creates a 600 x 600 `A_star_planner (Path_planner)`. It will also create and start the
UI window `Line_router_window` and pass along the `Path_planner`.

### UI (QT 5)
The UI is using the __QT 5__ toolkit. It consists of one window, the `Line_router_window`. It sets up the window,
creates a `Line_router_paint_widget` and passes along the `Path_planner`.  
The `Line_router_paint_widget` sets up the pixel map environment where it is possible to draw lines. It will wait
for a mouse click on a point that is inside of the pixel map and set that point to be the start point. Then the start
point is marked with a red dot on the pixel map. The second mouse click inside the pixel map will set the end point and
will pass the start and end point to the `Path_planner`. If the `Path_planner` is successful in finding a path from
start to end a line will be drawn on the given path and all the points passed will be set to blocked in the
`Path_planner`. If the `Path_planner` is unsuccessful in finding a path it will just unmark the first point and wait
for the first mouse click again. Would have been nice with a dialog popup in those cases.

### Path planner (A\*)
The algorithm to find the route from start to end is based on the search algorithm __A\*__. It is designed to find the
path from the start point to the given end point which has the smallest cost. In this implementation the cost is the
pixel-distance traveled.  
  
The algorithm has to determine, for each point visited, the cost to the point where it is currently at and an estimate
of the cost to the end point from the current point. More specifically:  

_f(p) = g(p) + h(p)_  

where _g(p)_ is the cost from the start point to the point _p_ and _h(p)_ is the estimated cost from point _p_ to the
end point. _f(p)_ is the total minimal cost from start to end going through the point _p_.  

In this implementation the horizontal and vertical travel of one point increases the _g(p)_ by one. A diagonal move,
however, increases the _g(p,s)_ by  

_sqrt( 1^2 + 1^2 ) ~= 1.4142136_.  

The estimate of the cost to the end point is calculated as the coordinate distance from point _p_ to the end point, such
-that  

_h(p) = sqrt( dx^2 + dy^2 )_  

where _dx_ is the x-coordinate distance from start point to point _p_ and _dy_ is the y-coordinate distance from start
point to point _p_.  

The algorithm will calculate and store the total cost _f(p)_ for the point currently visited, _p_,  and its eight
neighbors (horizontal, vertical and diagonal). Neighbors outside border are not considered. It will then decide what
point to visit by choosing the point with the currently cheapest total minimal cost _f(p)_. It could be a neighbor to
the currently visited point or a point visited previously or its neighbors.  

If a point has another line crossing it, it will not be considered for visit and the cost will remain infinite.  
For more general information, see [A\* search algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm).

## Grid
There are three grids implemented (if not counting the `QPixmap`)

 * __Availability grid__
 * __Path cost grid__
 * __Path grid__

All grids are implemented as flattened fixed sized `std::vector`. In a _width_ x _height_ grid the _index_ in the
`std::vector` for the width coordinate _x_ and height coordinate _y_ is as follows  

_index = x + y \* width_  

where _x_ and _y_ are zero indexed. And for calculating _x_ and _y_ from _index_  

_x = modulus( index, width )_  
_y = floor( index / width )_  

All grids have the same size as the `QPixmap`, right now it is 600 x 600. The total size of all the 600 x 600 grids,
discarding `std::vector` overhead, is  

_sizeof(bool)x600x600 +
sizeof(float)x600x600 +
sizeof(size_t)x600x600_  

On my 64-bit system with g++ 7.3.0 this makes a total cost of  
  
_1x600x600 +
4x600x600 +
8x600x600 ~= 4.46 MB_

### Availability grid
This __bool__ grid is used to set available/blocked grid points. It is initialized with all true values, i.e. available.
Once a line has been drawn all the points it has been passing and all of their neighbors will be marked as false, i.e.
blocked. The neighbors are marked in order to more clearly see that the lines are not intersecting.

### Path cost grid
This __float__ grid is used to keep track of the path cost _g(p)_ (see Path finding algorithm section) for each visited
point. It is initialized to infinity, except for the starting position which has a zero value. The path cost grid will
be updated for the currently visited point and its available neighbors.

### Path grid
This __size\_t__ grid consists of flattened grid indexes that point to a previous neighbor point visited. It is updated
by the currently visited point that sets all its available neighbors to the currently visited flattened grid index. When
the end point has been reached this grid can be used to backtrack the path to the start point.  

The grid is initialized to zero but the initial value does not matter since the path grid won't be used (only updated)
until the end point has been reached.

## Performance
The code has been tested on __Ubuntu Linux 18.04 LTS__ running on a __Dell XPS 15 Infinity (9560)__ with the following
specifications:  

* CPU: Intel Core i7 7th Gen. 2.8 Ghz.
* GPU: Nvidia GeForce GTX 1050 Ti Max-Q.
* RAM: 16GB DDR4/2,400MHz.  

For a __600 x 600__ grid with all points available it takes around __0.4 ms__ to find a path from point (0, 0) to
(599, 599).    
For a __2000 x 2000__ grid with all points available it takes around __2.3 ms__ to find a path from point (0, 0) to
(1999, 1999).  
For a __600 x 600__ grid with a diagonal block from point (1, 598) to (599, 0) it takes around __85 ms__ to find a path
from point (0, 0) o (599, 599).   
For a __2000 x 2000__ grid with a diagonal block from point (1, 1998) to (1999, 0) it takes around __1.1 s__.

The most costly parts of the code is when checking if neighbors are available and when adjusting the heap of the
priority queue where the points to visit are located.
