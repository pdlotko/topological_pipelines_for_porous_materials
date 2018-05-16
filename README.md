# Topological pipelines for porous materials

This is a code to compute topological invariant of nano porous materials

## Usage

### Subsampling the pore surface 
```
./subsample_pore_surface <.sa file> <accessible surface area> [output .sa filename] [density of sampling]
```

### Create supercell
```
./create_megacell <cssr file>
```

### Compute distance matrix
```
./compute_Rips_persistence_periodic_domain <cssr file>
```

