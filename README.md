A planet finder game solution written in C.
It uses Dijktra's algorithm with backtracking and random hops. This allows for hoping to random planets, finding the one which is closest to the desired planet and using Dijktra's algorithm to hop around planets and find the next planet which is closer.
If a deadend is found, the solution backtracks to the next closest planet which was not visited to try and find the desired planet.
