# Expose dynamic libraries required at run-time
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/feather

CWD=$CD  # Remember

cd build
cmake ..               # Append new files
make
sudo make install
cd /usr/local/feather  # Feather is installed here
./feather              # Run, or indicate failure

cd $CWD  # Restore
