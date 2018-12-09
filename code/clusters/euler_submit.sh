#! /bin/bash -l

#BSUB -N
#BSUB -q bigmem.4h
#BSUB -o job_%J.out
#BSUB -e job_%J.err
#BSUB -n 36
#BSUB -R "span[ptile=36]"


RUNS=5
MAX_THREAD=32

echo "Hostname=`hostname`"
echo "Number of available cores=`nproc --all`"
echo "Loading modules ..."
module load new gcc/5.2.0 open_mpi/1.6.5 cmake/3.11.4 boost/1.62.0

echo "Running job"


# Erdos-Renyi random graphs
~/PMST/code/bin/exec --algorithm ParallelSollinEL --Erdos-Renyi-graph 100 --Erdos-Renyi-graph 1000 --Erdos-Renyi-graph 10000 --lsb-filename SollinParallelEL-ERG --runs ${RUNS} --max-threads ${MAX_THREAD}
~/PMST/code/bin/exec --algorithm ParallelSollinAL --Erdos-Renyi-graph 100 --Erdos-Renyi-graph 1000 --Erdos-Renyi-graph 10000 --lsb-filename SollinParallelAL-ERG --runs ${RUNS} --max-threads ${MAX_THREAD}
~/PMST/code/bin/exec --algorithm Sollin --Erdos-Renyi-graph 100 --Erdos-Renyi-graph 1000 --Erdos-Renyi-graph 10000 --lsb-filename Sollin-ERG --runs ${RUNS} --max-threads 1
~/PMST/code/bin/exec --algorithm FilterKruskal --Erdos-Renyi-graph 100 --Erdos-Renyi-graph 1000 --Erdos-Renyi-graph 10000 --Erdos-Renyi-graph 100000 --Erdos-Renyi-graph 1000000  --lsb-filename FilterKruskal-ERG --runs ${RUNS} --max-threads ${MAX_THREAD} > ~/times.out
~/PMST/code/bin/exec --algorithm Kruskal --Erdos-Renyi-graph 100 --Erdos-Renyi-graph 1000 --Erdos-Renyi-graph 10000 --lsb-filename Kruskal-ERG --runs ${RUNS} --max-threads ${MAX_THREAD}

# USA graphs
~/PMST/code/bin/exec --algorithm ParallelSollinEL --USA-graph NY d --USA-graph BAY t --lsb-filename SollinParallelEL-USA --runs ${RUNS} --max-threads ${MAX_THREAD}
~/PMST/code/bin/exec --algorithm ParallelSollinAL --USA-graph NY d --USA-graph BAY t --lsb-filename SollinParallelAL-USA --runs ${RUNS} --max-threads ${MAX_THREAD}
~/PMST/code/bin/exec --algorithm Sollin --USA-graph NY d --USA-graph BAY t --lsb-filename Sollin-USA --runs ${RUNS} --max-threads 1
~/PMST/code/bin/exec --algorithm FilterKruskal --USA-graph NY d --USA-graph BAY t --lsb-filename FilterKruskal-USA --runs ${RUNS} --max-threads ${MAX_THREAD}
~/PMST/code/bin/exec --algorithm Kruskal --USA-graph NY d --USA-graph BAY t --lsb-filename Kruskal-USA --runs ${RUNS} --max-threads ${MAX_THREAD}

echo "Job ended"

