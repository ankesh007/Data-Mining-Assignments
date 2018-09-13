import time
import sys
start_time = time.time()

# with open('chess.dat') as f:
# with open(sys.argv[1]) as f:
# 	for line in f:
# 		ha = line.split(" ")
# 		ha.remove("\n")
# 		all.append(ha)
# print (all)
# print(len(all))
x1=int(sys.argv[2])/100
# I1='I1'
# I2='I2'
# I3 = 'I3'
# I4 = 'I4'
# I5 = 'I5'
# l1 = [I1, I2, I5]; l2=[I2, I4]
# l3= [I2, I3];l4=[I1, I2, I4];l5=[I1, I3];l6=[I2, I3];l7=[I1, I3];l8=[I1, I2, I3, I5];l9=[I1, I2, I3]

# list1 = [l1,l2,l3,l4,l5,l6,l7,l8,l9]
k1=0
with open(sys.argv[1]) as f:
		for line in f:
			k1+=1

	#Generating Frequent Itemset from given dataset & min support 
def frSet(itemset,minSup):
	tab = {}
	frequent={}
	with open(itemset) as f:
		for line in f:
			ha = line.split(" ")
			ha.remove("\n")
			# all.append(ha)
			for item in ha:
				if item in tab:
					# Set = {item}	
					tab[item] += 1
				else:
					tab[item] = 1

	for key,value in tab.items():
		if (value/k1>=minSup):
			a = frozenset({key})
			frequent[a] = value/k1
#	sorted_x = sorted(frequent.items(), key=operator.itemgetter(1))		 				
	return frequent
# print(frSet(all,30/3196).items())


	     
#counts the frequency of items in itemset
def count(fset, minsup):
	fr={}
	freq={}
	t = set()
	i = 0
	with open(sys.argv[1]) as f:
		for line in f:
			ha = line.split(" ")
			ha.remove("\n")
			for se in fset:
				if(se.issubset(ha)):
					if se in fr:
						fr[se]+=1
					else:
						fr[se]=1
	for key,value in fr.items():
		if ((value/k1)>=minsup):
			# a = frozenset({key})
			freq[key] = value/k1
			t.add(key)					
	
		
	return freq,t

# print(count(frozenset({5}),[[2,4],[5,6],[5]]))
	     
def join(Set,length):
	output = set()
	# print (len(Set))
	for i in Set:
		for j in Set:
			if len(i.union(j))==length:
				output.add(i.union(j))
	return output

# print join()
# List = [[1],[2],[3],[4]]
# Set = set(map(frozenset, List))
# print(Set)
# print(join(Set,2))

def anyone(Set,minsup):
	b = False
	for items in Set:
		if (count(items,ItemSet)/k1>=minsup):
			b = True
			break
	return b

# print(anyone({frozenset({'56','40','34','36'}),frozenset({'56','40','34','36','45'})},all,2000))


def frequentItems(ItemSet,minsup):
	frItems = frSet(ItemSet,minsup)
	# print(frItems.items())
	fSet = set()
	for key,value in frItems.items():
		# a = frozenset([key]);
		# a.add(key);
		fSet.add(key)
	# print(fSet)
	length = 2;
	fSet = join(fSet,length)
	frItems1=frItems
	while len(frItems1)>1:
		length = length+1;
		
		tempSet = set()
		# print(fSet)
		frItems1,fSet=count(fSet,minsup)
		
		for key in fSet:
			value=frItems1[key]
			if (value>minsup):
				# print(key)
				frItems[key]=value
		fSet = join(fSet,length)
		# for x in fSet:

		# 	if (count(x)/k1>=minsup):
		# 		frItems[x] = count(x)/k1
		# 		tempSet.add(x)
		# fSet = tempSet
		# print(fSet)
	# print(frItems.items())
	return frItems
	        
# frequentItems(all,.92)

def list_powerset(lst):
    # the power set of the empty set has one element, the empty set
	result = [[]]
	for x in lst:
		result.extend([subset + [x] for subset in result])
	return result
 
# print(list_powerset({1,2}))
 
def subsets(s):
	List = list_powerset(s)
	# print(type(List))
	# print(List)
	List.remove([])
	# List.remove(s)
	# print(List)
	return set(map(frozenset, List))

# print(subsets(frozenset({1,2})))

def ass_rules(Set,ItemSet,minconf):
	tupleSet=set()
	for x in subsets(Set):
		tuple = ()
		if (count(Set,ItemSet)/count(x,ItemSet)>=minconf):
			tuple = tuple+(x,Set.difference(x),count(Set)/count(x))
			tupleSet.add(tuple)
	tupleSet.remove((Set,frozenset(),1.0))
	return tupleSet

# print(ass_rules(frozenset({'I1','I2','I4'}),list,0.5))

def main(gh,minsup,minconf):
	frItems = frequentItems(gh,minsup)
	# print(frItems)
	i = 1
	# print("S.No","Items","Support",sep='   ')
	file = open(sys.argv[4],'w')
	for key,value in frItems.items():
		# print(i,list(key),value,sep=('  '))
		for r in list(key):
			file.write(str(r)+' ')
		file.write('\n')
		i = i+1
	file.close()
	#print(frItems.items())
	# print()
	j = 1
	print((time.time() - start_time))
	# print("S.No","Item1","Item2","Confidence",sep='   ')
	# for key,value in frItems.items():
	# 	if (len(key)!=1):
	# 		z = ass_rules(key,ItemSet,minconf)
	# 		for x,y,w in z:
	# 			print(j,list(x),list(y),w,sep=('   '))
	# 			j=j+1

# main(list1,0.2,0.5)
# main(all,50000/88000,0.4)
if __name__=="__main__":
	main(sys.argv[1],x1,0.01)



# print("--- %s seconds ---" % (time.time() - start_time))

# fSet = {frozenset({'I1'}),frozenset({'I2'}),frozenset({'I3'})}
# print(anyone(fSet,list,8))
# print(join(fSet,2))
