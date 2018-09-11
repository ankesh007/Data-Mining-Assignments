import sys
import time

class treeNode:
    def __init__(self, nameValue, numOccur, parentNode):
        self.name = nameValue
        self.count = numOccur
        self.nodeLink = None #nodeLink 
        self.parent = parentNode 
        self.children = {} #

    def inc(self, numOccur):#
        self.count += numOccur


    def disp(self, ind=1):
        print ('  ' * ind, self.name, ' ', self.count)
        for child in self.children.values():
            child.disp(ind + 1)

#
def createTree(dataSet, minSup=1):
    headerTable = {}
    for trans in dataSet: 
        for item in trans:
            headerTable[item] = headerTable.get(item, 0) + dataSet[trans]
            
    for k in list(headerTable):  
        if headerTable[k] < minSup:
            del (headerTable[k])
    freqItemSet = set(headerTable.keys())#

    if len(freqItemSet) == 0:
        return None, None  
    for k in headerTable:
        headerTable[k] = [headerTable[k], None]  #

    retTree = treeNode('Null Set', 1, None)  
    for tranSet, count in dataSet.items():  
        localD = {}
        for item in tranSet:  
            if item in freqItemSet:#
                localD[item] = headerTable[item][0]

        
        if len(localD) > 0:
            orderedItems = [v[0] for v in sorted(localD.items(), key=lambda p: p[1], reverse=True)]
            updateTree(orderedItems, retTree, headerTable, count)  # populate tree with ordered freq itemset
    return retTree, headerTable  #


def createTree1( minSup=1):
    
    headerTable={}
    with open(sys.argv[1]) as f:
        for line in f:
            ha = line.split(" ")
            ha.remove("\n")
            # all.append(ha)
            for item in ha:
                    headerTable[item]=headerTable.get(item,0) + 1
                
    for k in list(headerTable):  
        if headerTable[k] < minSup:
            del (headerTable[k])
  
    freqItemSet = set(headerTable.keys())#

    if len(freqItemSet) == 0:
        return None, None  
    for k in headerTable:
        headerTable[k] = [headerTable[k], None]  #
    
    retTree = treeNode('Null Set', 1, None)


    with open(sys.argv[1]) as f:
        for line in f:
            ha = line.split(" ")
            ha.remove("\n")
            localD = {}
            for item in ha:  
                if item in freqItemSet:#
                    localD[item] = headerTable[item][0]

            
            if len(localD) > 0:
                orderedItems = [v[0] for v in sorted(localD.items(), key=lambda p: p[1], reverse=True)]
                updateTree(orderedItems, retTree, headerTable, 1)  # populate tree with ordered freq itemset
    return retTree, headerTable  #


def updateTree(items, inTree, headerTable, count=1):
    if items[0] in inTree.children:  # 
        inTree.children[items[0]].inc(count)  
    else:  # 
        inTree.children[items[0]] = treeNode(items[0], count, inTree)#
        if headerTable[items[0]][1] == None:  # 
            headerTable[items[0]][1] = inTree.children[items[0]]
        else:
            updateHeader(headerTable[items[0]][1], inTree.children[items[0]])
    if len(items) > 1: 
        updateTree(items[1::], inTree.children[items[0]], headerTable, count)

def updateHeader(nodeToTest, targetNode):
    while (nodeToTest.nodeLink != None):
        nodeToTest = nodeToTest.nodeLink
    nodeToTest.nodeLink = targetNode


def loadSimpDat():
    
    crimefile = open(sys.argv[1], 'r')
    retDict = {}
    for line in crimefile:
        h=line.split(' ')
        for i in h:
            i.pop()
        simpDat=[[int(num) for num in h] ]
        
        for trans in simpDat:
            retDict[frozenset(trans)] = 1

    
    return simpDat

def createInitSet(dataSet):
    retDict = {}
    for trans in dataSet:
        retDict[frozenset(trans)] = 1
    return retDict





def ascendTree(leafNode, prefixPath):  
    if leafNode.parent != None:
        prefixPath.append(leafNode.name)
        ascendTree(leafNode.parent, prefixPath)


def findPrefixPath(basePat, treeNode): 
    condPats = {}
    while treeNode != None:
        prefixPath = []
        ascendTree(treeNode, prefixPath)
        if len(prefixPath) > 1:
            condPats[frozenset(prefixPath[1:])] = treeNode.count 
        treeNode = treeNode.nodeLink
    return condPats


def mineTree(inTree, headerTable, minSup, preFix, freqItemList):
    if headerTable!= None:
        bigL = [v[0] for v in sorted(headerTable.items(), key=lambda p: str(p[1]))]
        # print(headerTable)
        for basePat in bigL:  
            # print(basePat)
            newFreqSet = preFix.copy()
            newFreqSet.add(basePat)
            freqItemList.append(newFreqSet)
            condPattBases = findPrefixPath(basePat, headerTable[basePat][1])


            myCondTree, myHead = createTree(condPattBases, minSup)
            if myHead != None: #3
             
                mineTree(myCondTree, myHead, minSup, newFreqSet, freqItemList)
        
def main():
    st=time.time()
    # supcnt = int(input("Enter Threshold count: "))
    k1=0
    with open(sys.argv[1]) as f:
        for line in f:
            k1+=1
    supcnt = int(sys.argv[2])*k1/100
    myFPtree, myHeaderTab = createTree1( supcnt)
    f = []
    mineTree(myFPtree, myHeaderTab, supcnt, set([]), f)
    # print(f)
    file = open(sys.argv[4],'w')
    for key in f:
        # print(i,list(key),value,sep=('  '))
        for r in list(key):
            file.write(str(r)+' ')
        file.write('\n')
        # i = i+1
    file.close()
    t=time.time()-st
    print (float(t))
if __name__ == '__main__':
    main()