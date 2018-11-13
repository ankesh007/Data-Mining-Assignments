/* 
 * File:   common.h
 * Author: njin
 *
 * Created on October 10, 2009, 7:15 PM
 */

#ifndef _COMMON_H
#define	_COMMON_H

using namespace std;

#define NULLCODE "NULL"

/*encoding and decoding used in subgraph pattern extension*/
#define SOURCE_SHIFT 19
#define TYPE_SHIFT 31
#define EDGE_SHIFT 12
#define SOURCE_MASK 0x00000fff
#define DRAIN_MASK 0x00000fff
#define EDGE_MASK 0x0000007f
#define TYPE_MASK 0x00000001
#define CODE_TYPE int
//highest bit: whether this is an internal extension; if 1, then lowest 12 bits represent node id; else, node type
//high 12 bits: source node id
//next 7 bits: edge label
//low 12 bits: drain node id
#define EXTCODE(u, x, y, z) ( ( ( (CODE_TYPE)(u) ) << TYPE_SHIFT ) | ( ( (CODE_TYPE)(x) ) << SOURCE_SHIFT ) |  ((CODE_TYPE)(y) << EDGE_SHIFT ) | ((CODE_TYPE)(z)) )
#define EXTSOURCE(x)   ( (CODE_TYPE) ( (x)  >> SOURCE_SHIFT ) & SOURCE_MASK )
#define EXTEDGE(x)  ( (CODE_TYPE) ( ((x)  ) >> EDGE_SHIFT ) & EDGE_MASK )
#define EXTDRAIN(x)   ( ((CODE_TYPE) ((x)  ) ) & DRAIN_MASK )
#define EXTTYPE(x) ((CODE_TYPE) ( (x)  >> TYPE_SHIFT ) & TYPE_MASK)
#define INITIAL_M 1 //initial momentum
#define M_GAIN 1    //this must be a positive number; momentum will gain so much if the unit score doesn't decrease
#define M_LOSS 1    //this must be a positive number; momentum will lose so much if the unit score decreases

#endif	/* _COMMON_H */

