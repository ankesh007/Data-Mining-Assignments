import sys
import os
from random import shuffle

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

line = 0
total_lines = len(test_molecules)
while line<total_lines:
    line = line + 1
    total_vertices  = int(test_molecules[line][:-1])
    line = line + 1
    num_vertices = 0
    is_active = 0

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
    
    vertices_list[is_active].append(temp_vert)
    edges_list[is_active].append(temp_edge)


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


total_negative_and_tests=len(vertices_list[0])

for i in range(total_negative_and_tests):
    counter_vertex+=1
    for vertex in vertices_list[0][i]:
        node_f.write('v '+str(counter_vertex)+' '+vertex)
        node_f.write('\n')

for i in range(total_negative_and_tests):
    counter_edge+=1
    for edge in edges_list[0][i]:
        edge_f.write('v '+str(counter_edge)+' '+edge)
        edge_f.write('\n')


node_f.close()
edge_f.close()
file_handle.close()
active_file_handle.close()
inactive_file_handle.close()
test_file_handle.close()

total_tests = total_negative_and_tests - total_negatives

command = "./gaia -v node_file.txt -e edge_file.txt -s 100 -n 10 -p " + str(total_positive)

os.system(command)