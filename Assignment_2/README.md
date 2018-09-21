# Different Clustering Algorithms

Read ***ProblemStatement*** for better understanding of task  

To install dependency, `c++ boost` library, run
`./install.sh`  
The script installs boost, and correctly sets all paths

To run algorithms, change to `src` directory  
`cd src`

To compile all 3 code files, run `./compile.sh`

Otherwise, to compile individually, run `make {DBSCAN,KMEANS,OPTICS}`  
Choose anyone according to requirement(Curly braces are for understanding)

To run code:

KMEANS:
`./KMEANS <filename> <k>`

DBSCAN:
`./DBSCAN <minPts> <epsilon> <filename> <outputfilename>`

OPTICS:
`./OPTICS <filename> <epsilon> <minPts>`

Note: for `DBSCAN`, outputfilename is optional

