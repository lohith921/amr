/*
{// map_lib
// A simple associative-array library for C
//
// License: MIT / X11
// Copyright (c) 2009 by James K. Lawless
// jimbo@radiks.net http://www.radiks.net/~jimbo
// http://www.mailsend-online.com
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
}
*/

#ifdef SINGLE
#define REAL float
#else
#define REAL double
#endif

#ifndef MMAP_LIB_H
#define MMAP_LIB_H

struct map_node {
   struct map_node *nxt;
   int point;
   REAL *xandy;
};

struct map_neighbor{
	struct map_neighbor *nxt;
	int t_num;
	int* nbs;
	};

struct map_node *create_nodemap();
struct map_neighbor *create_neighbormap();
void map_freenode(struct map_node *map);
void map_freeneighbor(struct map_neighbor *map);
void map_setnode(struct map_node *m,int pt,REAL coords[]);
void map_setneighbor(struct map_neighbor *m, int t, int neighbs[]);
REAL* map_getnode(struct map_node *m,int pt);
int* map_getneighbors(struct map_neighbor *m,int tnum);

#endif
