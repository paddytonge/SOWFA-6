### DURHAM PRECURSOR SIMULATION - NEUTRAL ABL ###

This folder contains the necessary files to run a neutral atmospheric boundary layer precursor simulation using SOWFA-6.

Make sure installation locations and paths are correctly set in the environmentScripts/Durham_environment script.

Adjust the slurm text at the top of runscript.preprocess and runscript.solve.1 to make it email you. If a different number of cores is selected in runscript.solve.1,
this needs to be changed in setUp as well as the decomposition order if using simple. 

NOTE: For paraview visualisation, you will need to purge loaded modules, then do 'module load openfoam', 'source $OPENFOAM_HOME/OpenFOAM/etc/bashrc', 
      and finally 'module load paraview'

To run the simulation, excute 

sbatch runscript.preprocess

You may want to then check the mesh with paraview. Once completed, continue the simulation by executing

sbatch runscript.solve.1

To view results at time 21600 for example:

reconstructPar -time 21600
foamToVTK -time 21600
paraFoam

