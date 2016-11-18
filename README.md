# libspatialindex-bulkloading-points
An example using the libspatialindex library to bulkload points into an **in-memory** R\*-Tree. This example loads points into an R-Tree using the bulkloading STR method. The example was adapted from [here](https://github.com/libspatialindex/libspatialindex/blob/master/test/rtree/RTreeBulkLoad.cc). It contains some unnecessary stuff such as using a custom-made class "Point.h" and assuming that each point belongs to a cluster. 

## The application does the following:
1. It implements a DataStream to handle our input. 
2. It does some text processing to convert the text data to a stream of SpatialIndex::RTree::Data objects. 
3. Each SpatialIndex::RTree::Data object is a region with two coordinates, *low* and *high*
4. Since our data are points, each object in the stream is a region with *low*==*high*. See [here](https://github.com/libspatialindex/libspatialindex/issues/69) for a discussion that clarifies that.
5. It stores a cluster id (as a sequence of bytes) along with each region (i.e. point)
6. Reads the points from a stream and inserts them directly into an R\*-Tree using the STR bulkloading method
7. It selects 600 random points and uses them as query points 
8. It queries the R-Tree and estimates the average time for retrieving a point. It also estimates the error of the measurements 

## Notes:
1. Since the application reads from a stream, it does not create unnecessary objects.  However, the application momentarily uses twice as much memory as the size of the data. This is because the stream is a stream of regions and each region holds two coordinates (*low* and *high* that are basically the same coordinates, i.e. the point's coordinates). As soon as the building is done however, memory usage is equal to the size of the input data (see **massif/12508.png**).  
2. A **sample file** is provided

## Install libspatialindex

Clear instructions for installing the library can be found [here](https://github.com/libspatialindex/libspatialindex/wiki/1.-Getting-Started) 

## How to run on Linux
**Assumes libspatialindex is properly installed**
```bash
mkdir build
cd build
cmake ..
make
./SpatialIndexRtree sample.txt
```
