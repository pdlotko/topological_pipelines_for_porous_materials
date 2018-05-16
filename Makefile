CC = gcc
CXXFLAGS  = -g -Wall -O3 -std=c++11

HEADERS  = blow_up_to_mega_cell.h find_epsilon_net.h common_zeolites.h 
HEADERS += random_subsampling_of_a_list_of_points.h 
HEADERS += compute_distance_matrix_of_points_in_R3.h  
HEADERS += read_zeopp_file.h compute_distance_matrix_relative.h

BINARIES = subsample_pore_surface create_megacell compute_Rips_persistence_periodic_domain

default: all
all: $(BINARIES)

%:: %.cpp $(HEADERS)
	    $(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean

clean:
	    rm -f $(BINARIES)
