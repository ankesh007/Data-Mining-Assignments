import sys
from random import shuffle

file_handle = open(sys.argv[1],"r")
lines = file_handle.readlines()

active_file_handle = open(sys.argv[2],"r")
temp_active_molecules = active_file_handle.readlines()

inactive_file_handle = open(sys.argv[3],"r")
temp_inactive_molecules = inactive_file_handle.readlines()

positive_samples=int(sys.argv[4])
shuffle(temp_inactive_molecules)

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
# f.write("Woops! I have deleted the content!")


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

positive_samples=min(positive_samples,len(vertices_list[1]))
print(positive_samples)
counter_vertex=-1
counter_edge=-1
total=len(vertices_list[1])

for i in range(positive_samples):
    counter_vertex+=1
    for vertex in vertices_list[1][i]:
        node_f.write('v '+str(counter_vertex)+' '+vertex)
        node_f.write('\n')

for i in range(positive_samples):
    counter_edge+=1
    for edge in edges_list[1][i]:
        edge_f.write('v '+str(counter_edge)+' '+edge)
        edge_f.write('\n')

print(counter_vertex)

for i in range(positive_samples):
    counter_vertex+=1
    for vertex in vertices_list[0][i]:
        node_f.write('v '+str(counter_vertex)+' '+vertex)
        node_f.write('\n')

for i in range(positive_samples):
    counter_edge+=1
    for edge in edges_list[0][i]:
        edge_f.write('v '+str(counter_edge)+' '+edge)
        edge_f.write('\n')
print(counter_vertex)

"""
Printing Test Samples
"""

for i in range(positive_samples,total):
    counter_vertex += 1
    for vertex in vertices_list[1][i]:
        node_f.write('v '+str(counter_vertex)+' '+vertex)
        node_f.write('\n')

for i in range(positive_samples,total):
    counter_edge += 1
    for edge in edges_list[1][i]:
        edge_f.write('v '+str(counter_edge)+' '+edge)
        edge_f.write('\n')
print(counter_vertex)

for i in range(positive_samples,total):
    counter_vertex += 1
    for vertex in vertices_list[0][i]:
        node_f.write('v '+str(counter_vertex)+' '+vertex)
        node_f.write('\n')

for i in range(positive_samples,total):
    counter_edge += 1
    for edge in edges_list[0][i]:
        edge_f.write('v '+str(counter_edge)+' '+edge)
        edge_f.write('\n')
print(counter_vertex)

reader=open('config.txt','w')
print("Positive:",positive_samples,file=reader)
print("Test Sample:",2*(total-positive_samples),file=reader)
reader.close()
node_f.close()
edge_f.close()
file_handle.close()
active_file_handle.close()
inactive_file_handle.close()
