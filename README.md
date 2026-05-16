# TBL CModel

C model generator for hardware table-based function approximation, producing bit-exact matches for 7 elementary functions: reciprocal, square root, inverse square root, power-of-2, log2 (and `_2x` variants).

## Required Tools

### macOS (Homebrew)

```bash
# GNU Octave + minimax optimization packages
brew install octave
octave --eval "pkg install -forge datatypes struct statistics optim"

# PDF to text (for reading the reference paper)
brew install poppler         # provides pdftotext

# C compiler and zlib (for building cmodel)
# Xcode CLT already includes clang/gcc and zlib
```

### Ubuntu

```bash
sudo apt update
sudo apt install octave octave-optim octave-statistics octave-struct
sudo apt install poppler-utils   # pdftotext
sudo apt install build-essential zlib1g-dev
```

## Pipeline

Three phases (see `specs.md` for full details):

### Phase 1 — Generate coefficient tables

```bash
octave generate_tables.m                    # all 7 functions (single-process)
octave generate_tables.m <func>             # single function
./generate_all.sh                           # parallel (all functions at once)
```

Output: `<func_name>.txt` — minimax quadratic coefficients (C0, C1, C2) per segment, stored as fixed-point absolute values with per-column signs. Default bit-width parameters (t=26, p=16/17, q=10, sqr_prec=15, t_bias=34) come from the hardware paper `a-highperformance-areaefficient-multifunction-interpolator.pdf`.

### Phase 2 — C model evaluation

```bash
gcc -O2 -o cmodel cmodel.c -lm -lz
./cmodel <func>            # evaluate against reference .bin.gz
./cmodel --all             # all 7 functions
```

Output: `<func_name>_model.bin` plus match-rate comparison against reference.

### Phase 3 — Coefficient search

```bash
gcc -O2 -o search search.c -lm -lz
./search <func>            # joint C0×C1×C2 per-segment search + bias
./search --all             # all 7 functions
./search --all -j 8        # limit to 8 threads
```

Output: `<func_name>_fixed.txt` — optimized coefficients with per-function bias.

## Key Files

| File | Purpose |
|------|---------|
| `specs.md` | Full project specification (3 phases) |
| `pineiro2005.pdf` | Reference paper — minimax quadratic interpolator (3-pass coefficient quantisation) |
| `a-highperformance-areaefficient-multifunction-interpolator.pdf` | Reference paper — hardware architecture; source of default t/p/q/sqr_prec parameters |
| `sqr_model.c` / `.h` | Hardware bit-interleaving squarer model |