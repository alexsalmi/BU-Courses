// Author: Alexander Salmi
// BUID: UXXXXXXX

#include <vector>
// Your includes here

using namespace std;

class Problem2b {

public:
    bool canFinish(int numJobs, vector<pair<int, int>>& dependencies) {
    	std::vector<std::vector<int>> dependencyList; 	// Vector of jobs a job is dependent on, for each job
    	std::vector<bool> discovered;					// Vector to keep track of which jobs have already been discovered and checked by DFS
    	std::vector<bool> currentRecurse;				// Vector to keep track of which jobs are in the current recursive process
    	int i, j, dependent, on;

    	// Fill discovered and currentRecurse vectors with false values, and dependencyList with empty arrays
    	for(i=0; i<numJobs; i++){
    		discovered.push_back(false);
    		currentRecurse.push_back(false);
    		dependencyList.push_back({});
    	}

    	// Fill each empty array in dependencyList with each job's, dependencies
    	for(i=0; i<dependencies.size(); i++){
    		dependent = dependencies[i].second;
    		on = dependencies[i].first;
    		dependencyList.at(dependent-1).push_back(on-1);
    	}

    	// For each job, do a DFS on it's dependencies
    	for(i=0; i<numJobs; i++)
    		// If the job has not already been discovered and checked by DFS
    		if(!discovered.at(i))
    			// If the function returns true, there is a cycle, meaning the jobs cannot be done as they are dependent on each other
    			if(findCycleDFS(i, &dependencyList, &discovered, &currentRecurse))
    				return false;

    	// If none of the jobs produced a cycle, return true
        return true;
    }

    // DFS to find a cycle in the graph. Returns true if there is a cycle in the graph, indicating that the jobs cannot be done
    // Used pseudocode from class notes and https://en.wikipedia.org/wiki/Depth-first_search to design DFS function
    bool findCycleDFS(int num, vector<vector<int>>* deps, vector<bool>* discovered, vector<bool>* currentRecurse){
    	// Label the job as dsicovered by DFS and in the current recurse
		discovered->at(num) = true;
		currentRecurse->at(num) = true;

		// For each job that job[num] is dependent on
		for(int i=0; i<deps->at(num).size(); i++){
			// If the job has already been visited in this recursive process, there is a cycle, so return true
			if(currentRecurse->at(deps->at(num).at(i)))
				return true;
			// If the job has not been visited in this recurse but has also not been visited by DFS, recursively run DFS on the job and it's dependencies
			else if(!discovered->at(deps->at(num).at(i))){
				// If the function returns true, there is a cycle, so return true
				if(findCycleDFS(deps->at(num).at(i), deps, discovered, currentRecurse))
					return true;
			}
		}

		// If there was not a cycle detected, mark the job as not being in the current recursive process anymore, and return false
    	currentRecurse->at(num) = false;
    	return false;
    }
};
