CC = gcc
CXXFLAGS  = -g -Wall -O3

all: subsample_pore_surface create_megacell compute_Rips_persistence_periodic_domain
subsample_pore_surface:
	$(CXX) $(CXXFLAGS) -o subsample_pore_surface subsample_pore_surface.cpp

create_megacell:
	$(CXX) $(CXXFLAGS) -o create_megacell create_megacell.cpp

compute_Rips_persistence_periodic_domain:
	$(CXX) $(CXXFLAGS) -o compute_Rips_persistence_periodic_domain compute_Rips_persistence_periodic_domain.cpp

.PHONY: clean

clean:
	rm -f subsample_pore_surface
	rm -f create_megacell
	rm -f compute_Rips_persistence_periodic_domain
