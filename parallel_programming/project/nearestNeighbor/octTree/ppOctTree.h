#include <iostream>
#include <cstdlib>
#include "sequence.h"
#include "cilk.h"
#include "geom.h"
#include "intSort.h"
#include "quickSort.h"
using namespace std;

// *************************************************************
//    QUAD/OCT TREE NODES
// *************************************************************

#define gMaxLeafSize 16  // number of points stored in each leaf
#define GTREE_BASE_CASE 65536
#define GTREE_SEQSORT_BASE 1000
#define GTREE_SUBPROB_POW .92
#define GTREE_ALLOC_FACTOR 2/gMaxLeafSize

// minpt: topleft of the square
// blocksize: length of each edge of a quadrant
// numblocks: number of quadrants in each direction
// p: the point to assign to a block
// return the index of the quadrant for the point p
int ptFindBlock(point2d minpt, double blocksize, int log_numblocks, point2d p) {
  int numblocks = (1 << log_numblocks);
  int xindex = min((int)((p.x - minpt.x)/blocksize),numblocks-1);  
  int yindex = min((int)((p.y - minpt.y)/blocksize),numblocks-1);  
  int result = 0;
  for (int i = 0; i < log_numblocks; i++) {
    int mask = (1 << i);
    result += (xindex & mask) << i;
    result += (yindex & mask) << (i+1);
  }
  return result;
}

// minpt: topleft of the cube
// blocksize: length of each edge of a quadrant
// numblocks: number of quadrants in each direction
// p: the point to assign to a block
// return the index of the quadrant for the point p
int ptFindBlock(point3d minpt, double blocksize, int log_numblocks, point3d p) {
  int numblocks = (1 << log_numblocks);
  int xindex = min((int)((p.x - minpt.x)/blocksize),numblocks-1);  
  int yindex = min((int)((p.y - minpt.y)/blocksize),numblocks-1);  
  int zindex = min((int)((p.z - minpt.z)/blocksize),numblocks-1);  
  int result = 0;
  for (int i = 0; i < log_numblocks; i++) {
    int mask = (1 << i);
    result += (xindex & mask) << (2*i);
    result += (yindex & mask) << (2*i+1);
    result += (zindex & mask) << (2*i+2);
  }
  return result;
}

// minpt: topleft of the square
// blocksize: length of each edge of a quadrant
// log_numblocks: log_2(number of quadrants in each direction)
// index: the index of this quadrant
// return the center point of this quadrant
point2d ptMakeBlock(point2d minpt, double blocksize, int log_numblocks, int index) {
  int xindex = 0;
  int yindex = 0;
  for (int i = 0; i < log_numblocks; i++) {
    xindex |= ((index & 1) << i);
    yindex |= ((index & 2) << i);
    index = (index >> 2);
  }
  yindex = yindex >> 1;
  double xoff = (double)xindex * blocksize;
  double yoff = (double)yindex * blocksize;
  return point2d(minpt.x + xoff,minpt.y +yoff).offsetPoint(7,blocksize/2);
}

// minpt: topleft of the cube
// blocksize: length of each edge of a quadrant
// log_numblocks: log_2(number of quadrants in each direction)
// index: the index of this quadrant
// return the center point of this quadrant
point3d ptMakeBlock(point3d minpt, double blocksize, int log_numblocks, int index) {
  int xindex = 0;
  int yindex = 0;
  int zindex = 0;
  for (int i=0; i<log_numblocks; i++) {
    xindex |= ((index & 1) << i);
    yindex |= ((index & 2) << i);
    zindex |= ((index & 4) << i);
    index = (index >> 3);
  }
  yindex = (yindex >> 1);
  zindex = (zindex >> 2);
  double xoff = (double)xindex * blocksize;
  double yoff = (double)yindex * blocksize;
  double zoff = (double)zindex * blocksize;
  return point3d(minpt.x+xoff,minpt.y+yoff,minpt.z+zoff).offsetPoint(7,blocksize/2);
}

template <class vertex>
struct nData {
  int cnt;
  nData(int x) : cnt(x) {}
  nData() : cnt(0) {}
  nData operator+(nData op2) {return nData(cnt+op2.cnt);}
  nData operator+(vertex* op2) {return nData(cnt+1);}
};

template <class pointT, class vectT, class vertexT, class nodeData = nData<vertexT> >
class gTreeNode {
private :

public :
  typedef pointT point;
  typedef vectT fvect;
  typedef vertexT vertex;
  typedef pair<point,point> pPair;

  // used for reduce to find bounding box
  struct minMax {
    pPair operator() (pPair a, pPair b) {
      return pPair((a.first).minCoords(b.first),
		   (a.second).maxCoords(b.second));}};

  struct toPair {
    pPair operator() (vertex* v) {
      return pPair(v->pt,v->pt);}};

  point center; // center of the box
  double size;   // width of each dimension, which have to be the same
  nodeData data; // total mass of vertices in the box
  int count;  // number of vertices in the box
  gTreeNode *children[8];
  vertex **vertices;
  gTreeNode *nodeMemory;

  // wraps a bounding box around the points and generates
  // a tree.
  static gTreeNode* gTree(seq<vertex*> v) {
    pPair minMaxPair = sequence::mapReduce<pPair>(v.S,v.size(),
						  minMax(),toPair());
    point minPt = minMaxPair.first;
    point maxPt = minMaxPair.second;
    fvect box = maxPt-minPt;
    point center = minPt+(box/2.0);

    gTreeNode* result = new gTreeNode(v,center,box.maxDim(),NULL,0);
    return result;
  }

  int IsLeaf() { return (vertices != NULL); }

  void del() {
    if (IsLeaf()) ; //delete [] vertices;
    else {
      for (int i=0 ; i < (1 << center.dimension()); i++) {
	children[i]->del();
	//	delete children[i];
      }
    }
    if (nodeMemory != NULL) free(nodeMemory);
  }

  // Returns the depth of the tree rooted at this node
  int Depth() {
    int depth;
    if (IsLeaf()) depth = 0;
    else {
      depth = 0;
      for (int i=0 ; i < (1 << center.dimension()); i++)
	depth = max(depth,children[i]->Depth());
    }
    return depth+1;
  }

  // Returns the size of the tree rooted at this node
  int Size() {
    int sz;
    if (IsLeaf()) {
      sz = count;
      for (int i=0; i < count; i++) 
	if (vertices[i] < ((vertex*) NULL)+1000) 
	  cout << "oops: " << vertices[i] << "," << count << "," << i << endl;
    }
    else {
      sz = 0;
      for (int i=0 ; i < (1 << center.dimension()); i++)
	sz += children[i]->Size();
    }
    return sz;
  }

  template <class F>
  void applyIndex(int s, F f) {
    if (IsLeaf())
      for (int i=0; i < count; i++) f(vertices[i],s+i);
    else {
      int ss = s;
      for (int i=0 ; i < (1 << center.dimension()); i++) {
	cilk_spawn children[i]->applyIndex(ss,f);
	ss += children[i]->count;
      }
      cilk_sync;
    }
  }

  struct flatten_FA {
    vertex** _A;
    flatten_FA(vertex** A) : _A(A) {}
    void operator() (vertex* p, int i) {_A[i] = p;}
  };

  seq<vertex*> flatten() {
    vertex** A = new vertex*[count];
    applyIndex(0,flatten_FA(A));
    return seq<vertex*>(A,count);
  }

  // Returns the child the vertex p appears in
  int findQuadrant (vertex* p) {
    return (p->pt).quadrant(center); }

// A hack to get around Cilk shortcomings
  static gTreeNode *newTree(seq<vertex*> S, point cnt, double sz, gTreeNode* newNodes, int numNewNodes) {
    utils::myAssert(numNewNodes > 0, "numNewNodes == 0\n");
    return new(newNodes) gTreeNode(S,cnt,sz,newNodes+1,numNewNodes-1); }

  struct compare {
    gTreeNode *tr;
    compare(gTreeNode *t) : tr(t) {}
    int operator() (vertex* p, vertex *q) {
      int x = tr->findQuadrant(p);
      int y = tr->findQuadrant(q);
      return (x < y);
    }
  };

  gTreeNode(point cnt, double sz) 
  {
    count = 0;
    size = sz;
    center = cnt;
    vertices = NULL;
    nodeMemory = NULL;
  }


void buildRecursiveTree(seq<vertex*> S, int* offsets, int quadrants, gTreeNode *newNodes, gTreeNode* parent, int nodesToLeft, int height, int depth) {
  parent->count = 0;

  if (height == 1) {
    for (int i=0; i<(1<<center.dimension()); i++) {
      point newcenter = (parent->center).offsetPoint(i,parent->size/4.0);
      int q = (nodesToLeft<<center.dimension()) + i;

      /*
      {
	int inquad = ptFindBlock(center.offsetPoint(0,size/2.0),size/(1<<(height+depth-1)),height+depth-1,newcenter);
	if (inquad != q) {
	  printf("this guy should be in quadrant %d, but he is in quadrant %d\n", q, inquad);
	}
      }
      */

      int l = ((q==quadrants-1) ? S.size() : offsets[q+1]) - offsets[q];
      seq<vertex*> A = seq<vertex*>(S.S+offsets[q],l);
      parent->children[i] = newNodes+q;
      cilk_spawn newTree(A,newcenter,parent->size/2.0,newNodes+q,1);
    } 
  } else {
    for (int i=0; i< (1<<center.dimension()); i++) {
      point newcenter = (parent->center).offsetPoint(i, parent->size/4.0);
      parent->children[i] = new(newNodes + i + 
				nodesToLeft*(1<<center.dimension())) gTreeNode(newcenter,parent->size/2.0);
      cilk_spawn buildRecursiveTree(S, offsets, quadrants, newNodes + (1<<(depth*center.dimension())), parent->children[i], (nodesToLeft << center.dimension())+i, height-1,depth+1);
    }
  }
  cilk_sync;
  for (int i=0; i<(1<<center.dimension()); i++) {
    parent->data = parent->data + (parent->children[i])->data;
    parent->count += (parent->children[i])->count;
  }
  if (parent->count == 0) {
    // make it look like a leaf
    parent->vertices = S.S;
  }
}

typedef pair<int,vertex*> pintv;

struct compFirst {
  bool operator()(pintv A, pintv B) {return A.first<B.first; }
};

static void sortBlocksBig(vertex** S, int count, int quadrants, 
			      int logdivs, 
			      double size, point center, int* offsets) {
  pintv* blk = newA(pintv,count);
  double blocksize = size/(double)(1 << logdivs);
  point minpt = center.offsetPoint(0,size/2);
  {cilk_for (int i=0;i< count; i++) 
      blk[i] = pintv(ptFindBlock(minpt,blocksize,logdivs,S[i]->pt),
		    S[i]);}
  intSort::iSort(blk,offsets,count,quadrants,utils::firstF<int,vertex*>());
  {cilk_for (int i=0;i< count; i++) 
      S[i] = blk[i].second;}
  free(blk);
}

static void sortBlocksSmall(vertex** S, int count,point center, int* offsets) {
    vertex* start = S[0];
    int quadrants = 1 << center.dimension();
    pintv* blk = newA(pintv,count);
    for (int i=0;i< count; i++) {
      //cout << i << " : " << S[i]-start << endl;
      blk[i] = pintv((S[i]->pt).quadrant(center),S[i]);
    }
    compSort(blk,count,compFirst());
    int j = -1;
    for (int i=0; i< count; i++) {
      S[i] = blk[i].second;
      while (blk[i].first != j) {
	offsets[++j] = i;
      }
    }
    while (++j < quadrants) offsets[j] = count;
    free(blk);
}

/* newNodes is the memory to use for allocated gTreeNodes.  this is
   currently ignored for the upper levels of recursion (where we build
   more than 4 or 8 "quadrants" at a time).  It is used at the lower
   levels where we build just a single level of the tree.  If space is
   exhausted, more memory is allocated as needed.  The hope is that
   the number of reallocations are reduced  */
  gTreeNode(seq<vertex*> S, point cnt, double sz, gTreeNode* newNodes, int numNewNodes)
  {
    count = S.size();
    size = sz;
    center = cnt;
    vertices = NULL;
    nodeMemory = NULL;

    // divide the space into  ~n**POW "quadrants"
    int logdivs = (int)(log2(count)*GTREE_SUBPROB_POW/(double)center.dimension());
    if (logdivs > 1 && count > GTREE_BASE_CASE) {
      int divisions = (1<<logdivs); // number of quadrants in each dimension
      //if (count > 1000000) cout << "divisions=" << divisions << endl;
      int quadrants = (1<<(center.dimension() * logdivs)); // total number
      int *offsets = newA(int,quadrants);

      //nextTime("before sort");
      sortBlocksBig(S.S, count, quadrants, logdivs, 
		    this->size, this->center,offsets);
      //nextTime("sort time");

      numNewNodes = (1<<center.dimension());
      for (int i=0; i<logdivs;i++) {
	numNewNodes = ((numNewNodes << center.dimension())
		       + (1<<center.dimension()));
      }
	
      nodeMemory = newA(gTreeNode, numNewNodes);
      buildRecursiveTree(S,offsets,quadrants,nodeMemory,this,0,logdivs,1);
      free(offsets);
    } else if (count > gMaxLeafSize) {
      if (numNewNodes < (1<<center.dimension())) { 
	// allocate ~ count/gMaxLeafSize gTreeNodes here
	numNewNodes = max(GTREE_ALLOC_FACTOR*
			  max(count/gMaxLeafSize, 1<<center.dimension()),
			  1<<center.dimension());
	nodeMemory = newA(gTreeNode,numNewNodes);
	newNodes = nodeMemory;
      }
      //      newNodes = (nodeMemory = newA(gTreeNode,(1<<center.dimension())));

      int quadrants = ( 1<< center.dimension());
      int offsets[8];

      if (1) {
        sortBlocksSmall(S.S, S.sz, center, offsets);
      } else {
      compSort(S.S, S.sz, compare(this));
      for (int q=0; q<quadrants; q++) {
	int f = 0;
	int l = S.sz;
	while (f < l) {
	  int g = (f+l)/2;
	  if (findQuadrant(S[g]) < q) {
	    f = g+1;
	  } else {
	    l = g;
	  }
	}
	offsets[q] = f;
      }
      }
	
      // Give each child its appropriate center and size
      // The centers are offset by size/4 in each of the dimensions
      int usedNodes = 0;
      for (int i=0 ; i < quadrants; i++) {
	int l = ((i == quadrants-1) ? S.sz : offsets[i+1]) - offsets[i];
	seq<vertex*> A = seq<vertex*>(S.S+offsets[i],l);
	point newcenter = center.offsetPoint(i, size/4.0);
       	children[i] = newTree(A,newcenter,size/2.0,newNodes+usedNodes,(numNewNodes - (1<<center.dimension()))*l/count + 1);
	usedNodes += (numNewNodes - (1<<center.dimension()))*l/count + 1;
      }
      for (int i=0 ; i < quadrants; i++) 
	data = data + children[i]->data;
    } else {
      vertices = S.S;
      for (int i=0; i < count; i++) {
	data = data + S[i];
      }
      //S.del();
    }
  }

};
