import sys
file_handle = open(sys.argv[1],"r")
lines = file_handle.readlines()
line = 0
total_lines = len(lines)
atom_to_ind = {}
ind = 0
print_count = 0
while line<total_lines:
	print_line = "t # " + str(print_count)
	print_count = print_count + 1
	print print_line
	line = line + 1
	total_vertices  = int(lines[line][:-1])
	line = line + 1
	num_vertices = 0
	while num_vertices<total_vertices:
		if lines[line][:-1] not in atom_to_ind:
			atom_to_ind[lines[line][:-1]] = ind
			ind = ind + 1
		print_line = "v " + str(num_vertices) + " " + str(atom_to_ind[lines[line][:-1]])
		print print_line
		num_vertices = num_vertices + 1
		line = line + 1
	total_edges = int(lines[line][:-1])
	line = line + 1
	num_edges = 0
	while num_edges<total_edges:
		print_line = "e " + lines[line][:-1]
	 	print print_line
	 	num_edges = num_edges + 1
	 	line = line + 1
