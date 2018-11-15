import sys
import os
from random import shuffle
import numpy as np
from sklearn.metrics import f1_score
import time

file_handle = open(sys.argv[1],"r")
lines = file_handle.readlines()

active_file_handle = open(sys.argv[2],"r")
temp_active_molecules = active_file_handle.readlines()

inactive_file_handle = open(sys.argv[3],"r")
temp_inactive_molecules = inactive_file_handle.readlines()

test_file_handle = open(sys.argv[4],"r")
test_molecules = test_file_handle.readlines()

active_molecules = {}
inactive_molecules = {}

for mol in temp_inactive_molecules:
    inactive_molecules[int(mol)]=1

for mol in temp_active_molecules:
    active_molecules[int(mol)]=1

line = 0
total_lines = len(lines)
atom_to_ind = {}
ind = 1
node_f = open("node_file.txt", "w")
edge_f = open("edge_file.txt", "w")


vertices_list = [[],[]]
edges_list = [[],[]]

while line<total_lines:
    graph_id = int(lines[line][1:-1])
    line = line + 1
    total_vertices  = int(lines[line][:-1])
    line = line + 1
    num_vertices = 0
    is_active = -1
    if graph_id in active_molecules:
        is_active = 1

    if graph_id in inactive_molecules:
        is_active = 0

    temp_vert=[]

    while num_vertices<total_vertices:
        if lines[line][:-1] not in atom_to_ind:
            atom_to_ind[lines[line][:-1]] = ind
            ind = ind + 1
        print_line =  str(num_vertices) + " " + str(atom_to_ind[lines[line][:-1]])
        num_vertices = num_vertices + 1
        line = line + 1
        if is_active!=-1:
            temp_vert.append(print_line)

    temp_edge=[]
    total_edges = int(lines[line][:-1])
    line = line + 1
    num_edges = 0
    while num_edges<total_edges:
        print_line = lines[line][:-1]
        num_edges = num_edges + 1
        line = line + 1
        if is_active!=-1:
            temp_edge.append(print_line)
    
    if(is_active!=-1):
         vertices_list[is_active].append(temp_vert)
         edges_list[is_active].append(temp_edge)


total_negatives = len(vertices_list[0])

test_vertices_list = []
test_edge_list = []

line = 0
total_lines = len(test_molecules)
while line<total_lines:
    line = line + 1
    total_vertices  = int(test_molecules[line][:-1])
    line = line + 1
    num_vertices = 0

    temp_vert=[]

    while num_vertices<total_vertices:
        if test_molecules[line][:-1] not in atom_to_ind:
            atom_to_ind[test_molecules[line][:-1]] = ind
            ind = ind + 1
        print_line =  str(num_vertices) + " " + str(atom_to_ind[test_molecules[line][:-1]])
        num_vertices = num_vertices + 1
        line = line + 1
        temp_vert.append(print_line)

    temp_edge=[]
    total_edges = int(test_molecules[line][:-1])
    line = line + 1
    num_edges = 0
    while num_edges<total_edges:
        print_line = test_molecules[line][:-1]
        num_edges = num_edges + 1
        line = line + 1
        temp_edge.append(print_line)
    
    test_vertices_list.append(temp_vert)
    test_edge_list.append(temp_edge)

total_positive=len(vertices_list[1])
total_negatives=len(vertices_list[0])

fraction_neg=[1,1.5,2,2.5,3,3.5]
best_score=0.0
best_list=[]
best_size=[]
star_time=time.time()
time_lim=12*60

def getFscore(tot):
    a = np.genfromtxt('./pred.txt')
    b = np.ones((tot))
    b[tot//2:] *= -1
    score=f1_score(b, a, average='macro')
    return score

list_indices = []
for i in range(0, total_negatives):
    list_indices.append(i)
best_list=list(list_indices)


for frac in fraction_neg:
    cur_time=time.time()
    if(cur_time-star_time>time_lim):
        break
    experiments=5
    for k in range(experiments):
        cur_time = time.time()
        if(cur_time-star_time > time_lim):
            break
        score=0
        counter_vertex=-1
        counter_edge=-1

        keep_positive=int(0.85*total_positive)
        test_positive=total_positive-keep_positive
        keep_negative=int(frac*keep_positive)
        test_negative=test_positive

        if(test_negative+keep_negative>total_negatives):
            break
        
        shuffle(list_indices)

        os.system("rm -rf node_file.txt")
        os.system("rm -rf edge_file.txt")
        node_f = open("node_file.txt", "w")
        edge_f = open("edge_file.txt", "w")

        for i in range(keep_positive):
            counter_vertex += 1
            for vertex in vertices_list[1][i]:
                node_f.write('v '+str(counter_vertex)+' '+vertex)
                node_f.write('\n')

        for i in range(keep_positive):
            counter_edge += 1
            for edge in edges_list[1][i]:
                edge_f.write('v '+str(counter_edge)+' '+edge)
                edge_f.write('\n')

        for i in range(keep_negative):
            counter_vertex += 1
            for vertex in vertices_list[0][list_indices[i]]:
                node_f.write('v '+str(counter_vertex)+' '+vertex)
                node_f.write('\n')

        for i in range(keep_negative):
            counter_edge += 1
            for edge in edges_list[0][list_indices[i]]:
                edge_f.write('v '+str(counter_edge)+' '+edge)
                edge_f.write('\n')
        """
        Priniting test
        """
        for i in range(keep_positive,keep_positive+test_positive):
            counter_vertex += 1
            for vertex in vertices_list[1][i]:
                node_f.write('v '+str(counter_vertex)+' '+vertex)
                node_f.write('\n')

        for i in range(keep_positive, keep_positive+test_positive):
            counter_edge += 1
            for edge in edges_list[1][i]:
                edge_f.write('v '+str(counter_edge)+' '+edge)
                edge_f.write('\n')

        for i in range(keep_negative,keep_negative+test_negative):
            counter_vertex += 1
            for vertex in vertices_list[0][list_indices[i]]:
                node_f.write('v '+str(counter_vertex)+' '+vertex)
                node_f.write('\n')

        for i in range(keep_negative,keep_negative+test_negative):
            counter_edge += 1
            for edge in edges_list[0][list_indices[i]]:
                edge_f.write('v '+str(counter_edge)+' '+edge)
                edge_f.write('\n')

        node_f.close()
        edge_f.close()
        total_train=keep_positive+keep_negative
        total_test=test_positive+test_negative

        command = "./GAIA " + str(keep_positive) + " " + "10" + \
            " " + "100" + " " + str(total_test)
        os.system(command)

        os.system("head -n"+str(total_positive)+" train_test.svm > libsvm-3.22/train.txt")
        os.system("tail -n"+str(total_test)+" train_test.svm > libsvm-3.22/test.txt")
        os.chdir('./libsvm-3.22')
        os.system("./svm-train -t 0 train.txt save.model.svm")
        os.system("./svm-predict test.txt save.model.svm pred.txt")
        os.system("rm -rf save.model.svm")
        val = getFscore(total_test)
        score=val
        print(val,k,frac,"***************************")
        os.chdir('../')

        if(score>best_score):
            best_score=score
            best_size=keep_negative
            best_list=list(list_indices)
            # best_neg_data=[]
            # for i in range(keep_negative):
            #     best_neg_data.append(list_indices[i])


os.system("rm -rf node_file.txt")
os.system("rm -rf edge_file.txt")
# times_positive=frac
print(best_size)
print(best_score)
# exit(0)
node_f = open("node_file.txt", "w")
edge_f = open("edge_file.txt", "w")

counter_vertex=-1
counter_edge=-1
total_positive=len(vertices_list[1])

for i in range(total_positive):
    counter_vertex+=1
    for vertex in vertices_list[1][i]:
        node_f.write('v '+str(counter_vertex)+' '+vertex)
        node_f.write('\n')

for i in range(total_positive):
    counter_edge+=1
    for edge in edges_list[1][i]:
        edge_f.write('v '+str(counter_edge)+' '+edge)
        edge_f.write('\n')

total_negatives = len(vertices_list[0])

# printed_negatives = min(int(total_positive*times_positive),total_negatives)
printed_negatives=max(best_size,min(total_positive,total_negatives))
list_indices = best_list

for i in range(0,total_negatives):
    counter_vertex+=1
    for vertex in vertices_list[0][list_indices[i]]:
        node_f.write('v '+str(counter_vertex)+' '+vertex)
        node_f.write('\n') 
    

for i in range(0,total_negatives):
    counter_edge+=1
    for edge in edges_list[0][list_indices[i]]:
        edge_f.write('v '+str(counter_edge)+' '+edge)
        edge_f.write('\n')

total_tests = len(test_vertices_list)

for i in range(0,total_tests):
    counter_vertex+=1
    for vertex in test_vertices_list[i]:
        node_f.write('v '+str(counter_vertex)+' '+vertex)
        node_f.write('\n') 
    

for i in range(0,total_tests):
    counter_edge+=1
    for edge in test_edge_list[i]:
        edge_f.write('v '+str(counter_edge)+' '+edge)
        edge_f.write('\n')



node_f.close()
edge_f.close()
file_handle.close()
active_file_handle.close()
inactive_file_handle.close()
test_file_handle.close()


assumed_tests = total_tests + total_negatives - printed_negatives

iter_count = 10
candidate_list_size = 100

command = "./GAIA " + str(total_positive) + " " + str(iter_count) +" " + str(candidate_list_size) + " " + str(assumed_tests)

os.system(command)

total_train = total_positive + total_negatives
svm_result_file = open("train_test.svm","r")
svm_lines = svm_result_file.readlines()

train_file_handle = open("train.txt","w")

for i in range(0,total_train):
    train_file_handle.write(svm_lines[i])

train_file_handle.close()



test_file_handle = open("test.txt", "w")

for i in range(total_train,len(svm_lines)):
    test_file_handle.write(svm_lines[i])
test_file_handle.close()
