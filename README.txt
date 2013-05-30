Conway's Game of Life is a model where "cells" interact with eachother in a virtual plane to create or destroy other cells.

The Rules
==============
	(1) Any living cell with less than 2 living neighbors dies.
	(2) Any living cell with two or three neighbors survives to the next generation
	(3) Any living cell with more than three neighbors dies.
	(4) Any dead cell with exactly three living neighbors becomes alive

The rules can be thought of as the effects of under-population (1), overcrowding (3), and reproduction (4).

Each generation is applied uniformly. All at once.

Conway's Game of Life is a specific version of a Game of Life where the "rule" is "23/3" - If a cell has 2 or 3 neighbors, it survives. If a dead cell has 3 neighbors, it will come to life. Changing this rule for a simulation will drastically change how the world behaves.


The project is written in C++11 with SFML 2.0 as the graphics library. Setup with your favorite cmake-supported build system is easy. See http://sfml-dev.org/ for details.
