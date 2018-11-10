import sys
file_handle = open(sys.argv[1],"r")
lines = file_handle.readlines()

active_file_handle = open(sys.argv[2],"r")
temp_active_molecules = active_file_handle.readlines()

inactive_file_handle = open(sys.argv[3],"r")
temp_inactive_molecules = inactive_file_handle.readlines()

active_molecules = []
inactive_molecules = []

for mol in temp_inactive_molecules:
	inactive_molecules.append(int(mol))

for mol in temp_active_molecules:
	active_molecules.append(int(mol))

line = 0
total_lines = len(lines)
atom_to_ind = {}
ind = 1
node_f = open("node_file.txt", "w")

edge_f = open("edge_file.txt", "w")
# f.write("Woops! I have deleted the content!")


vertices_list = [[],[]]

edges_list = [[],[]]

print_count = -1

while line<total_lines:
	graph_id = int(lines[line][1:-1])
	line = line + 1
	total_vertices  = int(lines[line][:-1])
	line = line + 1
	num_vertices = 0
	is_active = -1
	if graph_id in active_molecules:
		is_active = 1
		print_count = print_count + 1

	if graph_id in inactive_molecules:
		is_active = 0
		print_count = print_count + 1

	graph_id = print_count

	while num_vertices<total_vertices:
		if lines[line][:-1] not in atom_to_ind:
			atom_to_ind[lines[line][:-1]] = ind
			ind = ind + 1
		print_line = "v" + str(graph_id) + " " + str(graph_id) +" " +str(num_vertices) + " " + str(atom_to_ind[lines[line][:-1]])
		num_vertices = num_vertices + 1
		line = line + 1
		if is_active!=-1:
			vertices_list[is_active].append(print_line)

	total_edges = int(lines[line][:-1])
	line = line + 1
	num_edges = 0
	while num_edges<total_edges:
		print_line = "v" + str(graph_id) + " " + str(graph_id) + " " +lines[line][:-1]
	 	num_edges = num_edges + 1
	 	line = line + 1
	 	if is_active!=-1:
	 		edges_list[is_active].append(print_line)


for vertex in vertices_list[1]:
	node_f.write(vertex)
	node_f.write('\n')

for vertex in vertices_list[0]:
	node_f.write(vertex)
	node_f.write('\n')

for edge in edges_list[1]:
	edge_f.write(edge)
	edge_f.write('\n')

for edge in edges_list[0]:
	edge_f.write(edge)
	edge_f.write('\n')

node_f.close()
edge_f.close()
file_handle.close()
