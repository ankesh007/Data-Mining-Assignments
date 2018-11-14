#ifndef _COMMON_H
#define	_COMMON_H

using namespace std;

#define NULLCODE "NULL"

#define SOURCE_SHIFT 19
#define TYPE_SHIFT 31
#define EDGE_SHIFT 12
#define SOURCE_MASK 0x00000fff
#define DRAIN_MASK 0x00000fff
#define EDGE_MASK 0x0000007f
#define TYPE_MASK 0x00000001
#define INITIAL_M 1 //initial momentum
#define M_GAIN 1    //this must be a positive number; momentum will gain so much if the unit score doesn't decrease
#define M_LOSS 1    //this must be a positive number; momentum will lose so much if the unit score decreases

#endif	/* _COMMON_H */

