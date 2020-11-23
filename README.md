# Genetic-Algorithms
This repository is all about solving some well known backtracking problems with the help of genetic algorithm.

## Introduction to Genetic Algorithm
According to the article from [towardsdatascience](https://towardsdatascience.com/introduction-to-genetic-algorithms-including-example-code-e396e98d8bf3#:~:text=A%20genetic%20algorithm%20is%20a,offspring%20of%20the%20next%20generation.), a genetic algorithm is a search heuristic that is inspired by Charles Darwin's theory of natural evolution. This algorithm reflects the process of natural selection where the fittest individuals are selected for reproduction in order to produce offspring of the next generation.

## How we solved the assignments using genetic algorithm

### Initializing the population
The intial population is randomply generated and we make sure that all the chromosomes in the population are distinct. Here is the code of how we generated intial population in C++.

Creating Random Permutation
```cpp
/*
  @param int n
  @return vector<int>
*/
vector<int> genrate_random_permutation(int n){
  vector<int>random_permutation;
  for(int i=1;i<=n;i++){
    random_permutation.push_back(i);
  }
  shuffle(random_permutation.begin(),random_permutation.end(),default_random_engine(rand()%100000000));
  return random_permutation;
}
```

Intializing the population with unique random permutations.
```cpp
/*
  @param int size
  @return void
*/
void population_initialization(int size){
  set<vector<int>>population_set;
  for(int i=0;i<size;i++){
    vector<int>random_permutation = genrate_random_permutation(n);
    while(population_set.find(random_permutation)!=population_set.end()){
      random_permutation=genrate_random_permutation(n);
    }
    population.push_back(random_permutation);
    population_set.insert(random_permutation);
  }
}
```

### Fitness Function
### Crossover
### Mutation
### Creating Offsprings
### Selecting best ones
### Repeating the process
