conda list --explicit > spec-file.txt
conda create --name orb_env --file spec-file.txt

conda activate orb_env
./build_pybind11.sh
conda deactivate
./build.sh
conda activate orb_env


